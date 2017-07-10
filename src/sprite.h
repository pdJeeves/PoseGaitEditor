#ifndef SPRITE_H
#define SPRITE_H
#include "bodydata.h"
#include <QImage>
#include <vector>

class Sprite : public BodyData
{
	std::vector<QImage> m_images;

	static QImage readImage(FILE * file, short w, short h);
	static uint32_t packBytes(uint32_t a, uint32_t b, uint32_t c,uint32_t d);
	bool frameFromFrame(uint16_t & i) const;

	std::string filename;
	int m_part;
	bool m_gender;
	int m_age;

public:
	Sprite(FILE * file, const std::string & filename, int body_part, bool gender, int age );

	int part() const { return m_part; }
	int age() const { return m_age; }
	QPoint size(uint16_t frame) const;
	bool valid(uint32_t i) { return i < m_images.size() && !m_images[i].isNull(); }
	QSize qsize(uint16_t frame) const;
	const QImage & getImage(uint16_t frame) const { return m_images[frame]; }
	QPoint getAttachment(uint16_t attachment, uint16_t frame);
};

#endif // SPRITE_H
