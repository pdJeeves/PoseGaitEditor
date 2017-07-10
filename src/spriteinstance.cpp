#include "skell.h"
#include "spriteinstance.h"
#include "spritemanager.h"
#include <QPainter>

SpriteInstance::SpriteInstance() :
	m_sprite(0L),
	m_part(0),
	m_gender(0),
	m_age(0)
{
}

SpriteInstance::SpriteInstance(uint8_t part, bool gender, uint8_t age, const std::string & name) :
	m_part(part),
	m_gender(gender),
	m_age(age)
{
	m_sprite = getSprite(part, gender, age, name);
}

bool SpriteInstance::frameFromFrame(uint8_t & i) const
{
	if(mirror())
	{
		if(i & 0x02)
		{
			i ^= 0x01;
		}
	}

	if(m_sprite->valid(i))
	{
		return mirror();
	}

//default to false for reverse sprites
	bool r = false;

//L and R
	if(i % 4 >= 2)
	{
//if mirrored we need to mirror it again, probably this will always be true
//set 2 to 3 and 3 to 2
		i ^= 0x01;

		if(m_sprite->valid(i))
		{
			return !mirror();
		}

		r = !mirror();
	}

//if head then check if we need a blinking sprite
	if(m_part == 0)
	{
		while(i > 8)
		{
			i -= 8;
			if(m_sprite->valid(i)) return r;
		}
	}

	while(i >= 4)
	{
		i -= 4;
		if(m_sprite->valid(i))	return r;
	}

	throw std::runtime_error("unable to find missing mirror sprite");
}

float SpriteInstance::getRotation(uint8_t frame, float value)
{
	if(!m_sprite) return 0;

	bool mirror = frameFromFrame(frame);
	auto p = m_sprite->getAttachment(0, frame);

	if(mirror)
	{
		return -((p.y()) * value + p.x());
	}

	return ((p.y()) * value + p.x());
}

QPoint SpriteInstance::getAttachment(int att_point, uint8_t frame)
{
	if(!m_sprite) return QPoint();

	bool mirror = frameFromFrame(frame);
	auto p = m_sprite->getAttachment(att_point+1, frame);

	if(mirror)
	{
		return QPoint(m_sprite->size(frame).x() - p.x(), p.y());
	}

	return p;
}

QPoint SpriteInstance::size(uint8_t direction)
{
	if(!m_sprite) return QPoint(0,0);
	frameFromFrame(direction);
	return m_sprite->size(direction);
}

void SpriteInstance::render(QPainter & painter, uint8_t frame)
{
	bool mirror = frameFromFrame(frame);
	QPoint p =	m_sprite->getAttachment(1, frame);

	if(mirror)
	{
		painter.scale(-1, 1);
	}

	painter.drawImage(QRect(-p, m_sprite->qsize(frame)), m_sprite->getImage(frame));
}

