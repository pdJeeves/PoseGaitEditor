#ifndef BODYDATA_H
#define BODYDATA_H
#include <QPoint>
#include <string>
#include <vector>
#include <cassert>

class BodyData
{
	std::vector<std::vector<QPoint> > m_data;


	static bool getNumber(const char * buffer, uint16_t & i, int16_t & x);
	static bool getPair(const char * buffer, uint16_t & i, int16_t & x, int16_t & y);

public:
	void addAttachments(const std::string & filename);

	QPoint get_attachment(uint16_t attachment, uint16_t frame);
	int getMinAngle(uint16_t frame);
	int getMaxAngle(uint16_t frame);

	float getRotation(uint16_t frame, float rotation) const;

};

#endif // BODYDATA_H
