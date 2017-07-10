#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H
#include "sprite.h"
#include <string>
#include <QPoint>

class Sprite;

class SpriteInstance
{
	Sprite      *	m_sprite;
	bool			m_gender;
	uint8_t			m_age;
	uint8_t			m_part;

	bool frameFromFrame(uint8_t & frame) const;

public:
	SpriteInstance();
	SpriteInstance(uint8_t part, bool gender, uint8_t age, const std::string & name);

	float getRotation(uint8_t direction, float);
	QPoint getAttachment(int part, uint8_t direction);
	QPoint size(uint8_t direction);

	bool isValid() const { return m_sprite; }
	bool mirror() const { return m_part != m_sprite->part(); }
	void render(QPainter & painter, uint8_t frame);
};

#endif // SPRITEINSTANCE_H
