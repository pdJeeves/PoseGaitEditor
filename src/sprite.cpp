#include "body/body.h"
#include "sprite.h"
#include "byte_swap.h"
#include "squish.h"

Sprite::Sprite(FILE * file, const std::string & filename, int body_part, bool gender, int age) :
	filename(filename),
	m_part(body_part),
	m_gender(gender),
	m_age(age)
{
	uint16_t no_frames;
	fread(&no_frames, 2, 1, file);
	no_frames = byte_swap(no_frames);

	m_images.resize(no_frames);

	for(size_t i = 0; i < m_images.size(); ++i)
	{
		uint16_t width, height;
		uint32_t offset[5];
		fread(&width, 2, 1, file);
		fread(&height, 2, 1, file);
		fread(offset, 1, sizeof(offset), file);

		if(offset[0])
		{
			fpos_t pos;
			fgetpos(file, &pos);
			fseek(file, byte_swap(offset[0]), SEEK_SET);
			m_images[i] = readImage(file, byte_swap(width), byte_swap(height));
			fsetpos(file, &pos);
		}
	}
}

QPoint Sprite::size(uint16_t frame) const
{
	assert(frame < m_images.size());
	return QPoint(m_images[frame].width(), m_images[frame].height());
}

QSize Sprite::qsize(uint16_t frame) const
{
	assert(frame < m_images.size());
	return m_images[frame].size();
}

QImage Sprite::readImage(FILE * file, short w, short h)
{
	uint8_t compression_type;
	fread(&compression_type, 1, 1, file);
	switch(compression_type)
	{
	default:
		compression_type = squish::kDxt1;
		break;
	case 3:
		compression_type = squish::kDxt3;
		break;
	case 5:
		compression_type = squish::kDxt5;
		break;
	}

	uint32_t size;
	fread(&size, 4, 1, file);
	size = byte_swap(size);

	std::vector<uint8_t> compressed_image(size, 0);

	for(int i = 0; i < size; )
	{
		uint32_t length;
		fread(&length, sizeof(length), 1, file);
		length = byte_swap(length);

		switch(compression_type)
		{
		case 1:
			for(; i+8 <= size && length; length -= 8, i += 8)
			{
				memset(compressed_image.data() + (i+4), 0xFF, 4);
			}
			break;
		case 3:
			break;
		case 5:
			for(; i+16 <= size && length; length -= 16, i += 16)
			{
				compressed_image[i+1] = 0x05;
			}
			break;
		}

		i += length;

		if(i >= size)
		{
			break;
		}

		fread(&length, 4, 1, file);
		length = byte_swap(length);
		fread(compressed_image.data() + i, 1, length, file);
		i += length;
	}

	size = w*h;
	std::vector<QRgb> uncompressed_image(size);
	squish::DecompressImage((uint8_t*) uncompressed_image.data(), w, h, compressed_image.data(), compression_type);
	compressed_image.clear();

	QImage image = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	for(int i = 0; i < size; ++i)
	{
		QRgb c = uncompressed_image[i];
		c = packBytes(qRed(c), qGreen(c), qBlue(c), qAlpha(c));

		image.setPixel(i % w, i / w, c);
	}

	return image;
}

uint32_t Sprite::packBytes(uint32_t a, uint32_t b, uint32_t c,uint32_t d)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return (d << 24) | (c  << 16) | (b << 8) | a;
#else
	return (a << 8) | (b << 8) | (c << 8) | d;
#endif
}

QPoint Sprite::getAttachment(uint16_t attachment, uint16_t frame)
{
	if(m_part == bp_body && attachment > 0)
	{
		if(attachment == 1)
		{
			return size(frame)/2;
		}

		--attachment;
	}

	return BodyData::get_attachment(attachment, frame);
}
