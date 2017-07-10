#include "bodydata.h"
#include <cassert>

bool BodyData::getNumber(const char * buffer, uint16_t & i, int16_t & x)
{
	while(isspace(buffer[i]) && i < 1024) { ++i; }

	if(i == 1024 || !(('0' <= buffer[i] && buffer[i] <= '9') || buffer[i] == '-'))
	{
		return false;
	}

	int8_t _one = 1;
	if(buffer[i] == '-')
	{
		_one = -1;
		++i;
	}

	for(x = 0; '0' <= buffer[i] && buffer[i] <= '9' && i < 1024; ++i)
	{
		x = x * 10 + (buffer[i] - '0');
	}

	x *= _one;

	return true;
}

bool BodyData::getPair(const char * buffer, uint16_t & i, int16_t & x, int16_t & y)
{
	return getNumber(buffer, i, x) && getNumber(buffer, i, y);
}

void BodyData::addAttachments(const std::string & filename)
{
	FILE * file = fopen(filename.c_str(), "r");
	if(!file)
		return;

	while(!feof(file))
	{
		char buffer[1024];
		while(fgets(buffer, 1024, file))
		{
			if(m_data.size() == 0
			|| m_data.back().size() != 0)
			{
				m_data.emplace_back();
			}

			int16_t x = 0, y = 0;
			uint16_t i = 0;

//rotation
			while(getPair(buffer, i, x, y))
			{
				m_data.back().emplace_back(x, y);
			}
		}
	}

	fclose(file);
}


QPoint BodyData::get_attachment(uint16_t attachment, uint16_t frame)
{
	assert(frame < m_data.size());
	assert(attachment < m_data[frame].size());
	return m_data[frame][attachment];
}
