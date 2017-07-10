#ifndef SKELL_H
#define SKELL_H
#include "spriteinstance.h"
#include "body/body.h"
#include <string>
#include <QPoint>
#include <array>

extern "C"
{
typedef struct GeneticCreature GeneticCreature;
typedef struct CreatureSkell CreatureSkell;
}


class Skell
{
	QPointF			position[bp_totalParts];
	float			rotation[bp_totalParts];
	SpriteInstance  sprites[bp_totalParts];

	GeneticCreature * _this;

	int current_age;
	uint8_t direction;

	void ageChanged();
	void computePartRotations(std::array<float, bp_totalParts> &rot);

public:
	CreatureSkell  * m_skell;
	void * m_adapter;

	Skell();
	~Skell();

	bool loadGenome(const std::string & genome);
	void update(float dt);
	void render(QPainter & painter);
};

#endif // SKELL_H
