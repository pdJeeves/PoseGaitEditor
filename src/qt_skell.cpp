#include "qt_skell.h"
#include "creature.h"
#include "genetics.h"
#include "spritemanager.h"
#include <cmath>
#include <QPainter>

Skell::Skell() :
	_this(0L),
	m_skell(0L),
	m_adapter(0)
{
}

Skell::~Skell()
{
	if(_this) free_creature(_this);
}

bool Skell::loadGenome(const std::string & genome)
{
	GenomeData * genomeData = loadGenomeFromPath(genome.c_str());

	if(genomeData)
	{
		if(_this) free_creature(_this);
		memset(rotation, 0, sizeof(rotation));
		current_age = 255;

		_this = construct_creature(genomeData);
		m_skell = creature_get_skell(_this);
		direction = skellGetDirection(m_skell);
		genomeDestruct(genomeData);
		update(0);
		return true;
	}

	return false;
}

void Skell::update(float dt)
{
	std::array<float, bp_totalParts> rot;

	if(m_skell)
	{
		skellInterpolateTick((m_skell), m_adapter, dt*2);
		skellGetRotation((m_skell), (float*) rot.data());

		if(skellGetAge(m_skell) != current_age)
		{
			ageChanged();
		}

		direction = skellGetDirection(m_skell);
	}
	else
	{
		rot.fill(0);
	}

	computePartRotations(rot);
}

void Skell::ageChanged()
{
	if(!m_skell)
	{
		return;
	}

	uint8_t gender = skellGetGender(m_skell);
	current_age   = skellGetAge(m_skell);

	const char * name = 0L;

	for(int i = 0; i < bp_totalParts; ++i)
	{
		switch(i)
		{
		case bp_head:
			name = skellGetSprite(m_skell, bc_head);
			break;
		case bp_body:
			name = skellGetSprite(m_skell, bc_body);
			break;
		case bp_leftLeg:
			name = skellGetSprite(m_skell, bc_legs);
			break;
		case bp_leftArm:
			name = skellGetSprite(m_skell, bc_arms);
			break;
		case bp_tailRoot:
			name = skellGetSprite(m_skell, bc_tail);
			break;
		}

		sprites[i] = SpriteInstance(i, gender, current_age, name);
	}
}

QPointF rotate(QPoint p, float rotation)
{
#ifndef M_PI
#define M_PI 3.14159265358
#endif
	rotation = rotation*M_PI/180;
	const float sin = std::sin(rotation);
	const float cos = std::cos(rotation);

	return QPointF(p.x()*cos - p.y()*sin, p.x()*sin + p.y()*cos);
}

void Skell::computePartRotations(std::array<float, bp_totalParts> &rot)
{
	if(sprites[bp_body].isValid())
	{
		rotation[bp_body] = sprites[bp_body].getRotation(direction, rot[bp_body]);
	}

	int j = 1;
	for(auto i = getSkellComputationOrder(); *i != -1; ++i, ++j)
	{
		if(sprites[bp_body].isValid())
		{
			position[*i] = rotate(sprites[bp_body].getAttachment(j, direction) - sprites[bp_body].size(direction)/2, rotation[bp_body]);
		}

		float angle = rotation[bp_body];

		for(; *i != -1; ++i)
		{
			if(!sprites[*i].isValid())
				continue;

			angle += sprites[*i].getRotation(direction, rot[*i]);
			rotation[*i] = angle;

			if(*(i+1) == -1)
				continue;

			position[*(i+1)] = position[*i] + rotate(sprites[*i].getAttachment(1, direction) - sprites[*i].getAttachment(0, direction), rotation[*i]);
		}
	}
}

void Skell::render(QPainter & painter)
{
	if(!m_skell) return;

	for(auto i = getSkellDrawOrder(direction); *i != -1; ++i)
	{
		if(!sprites[*i].isValid())
			continue;

		painter.save();
		painter.translate(position[*i]);
		painter.rotate(rotation[*i]);
		sprites[*i].render(painter, direction);

		painter.restore();
	}
}

