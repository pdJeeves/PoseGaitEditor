#include "skell.h"
#include "sprite.h"
#include <map>
#include <unordered_map>
#include <array>
#include <vector>
#include <iostream>
#include <mutex>
#include <string>
#include "byte_swap.h"
#include "directoryrange.h"

#define StringTableEntry std::string

#ifndef SLASH
#ifdef _WIN32
#define SLASH '\\'
#define SLASH_STR "\\"
#define BAD_SLASH '/'
#define BAD_SLASH_STR "/"
#else
#define SLASH '/'
#define SLASH_STR "/"
#define BAD_SLASH '\\'
#define BAD_SLASH_STR "\\"
#endif
#endif

const static std::string smSpriteFolder("Images");
const static std::string smSpriteExtension("c32");

const static std::string smBodyDataFolder("Body Data");
const static std::string smBodyDataExtension("att");

typedef std::vector<Sprite>				byAge;
typedef std::array<byAge, 2>			byGender;
typedef std::map<StringTableEntry, byGender> bySpecies;

typedef std::array<bySpecies, TOTAL_PARTS> skell_t;

std::string & removeSlashes(std::string & str)
{
	for(auto it = str.begin(); it != str.end(); ++it)
	{
		if(*it == BAD_SLASH)
		{
			*it = SLASH;
		}
	}

	return str;
}


std::string getLower(std::string str)
{
	for(auto it = str.begin(); it != str.end(); ++it)
	{
		if(*it == '\\')
		{
			*it = '/';
		}

		*it = tolower(*it);
	}

	return str;
}

std::wstring getLower(std::wstring str)
{
	for(auto it = str.begin(); it != str.end(); ++it)
	{
		*it = tolower(*it);
	}

	return str;
}

int letter_from_part(int i)
{
	switch(i)
	{
	case HEAD:		return 'a';
	case BODY:		return 'b';
	case RIGHT_LEG:
	case LEFT_LEG:	return 'c';
	case RIGHT_CALF:
	case LEFT_CALF: return 'd';
	case RIGHT_FOOT:
	case LEFT_FOOT: return 'e';
//	case RIGHT_TOES:
//	case LEFT_TOES: return 'f';
	case RIGHT_ARM:
	case LEFT_ARM:	return 'i';
	case RIGHT_HAND:
	case LEFT_HAND: return 'j';
	case TAIL_ROOT: return 'm';
	case TAIL_TIP:	return 'n';
	}

	throw std::logic_error("tried to get nonexistant part");
	return 0;
}

int part_from_letter(char c)
{
	switch(tolower(c))
	{
	case 'a': return HEAD;
	case 'b': return BODY;
	case 'c': return LEFT_LEG;
	case 'd': return LEFT_CALF;
	case 'e': return LEFT_FOOT;
//	case 'f': return LEFT_TOES;
	case 'i': return LEFT_ARM;
	case 'j': return LEFT_HAND;
	case 'm': return TAIL_ROOT;
	case 'n': return TAIL_TIP;
	}

	throw std::logic_error("tried to get nonexistant part");
	return 0;
}

static
skell_t	& skell_db()
{
	static skell_t v;
	return v;
}

static
bool getClosestAge(Sprite *& closest, byAge & db, uint8_t age)
{
	for(auto i = db.begin(); i != db.end(); ++i)
	{
		if(i->age() == age)
		{
			closest =  &(*i);
			return true;
		}
		if(!closest
		|| age - closest->age() > age - i->age() )
		{
			closest =  &(*i);
		}

		if(i->age() > age)
		{
			break;
		}
	}

	return false;
}

static
bool getClosestGender(Sprite *& closest, byGender & db, bool gender, uint8_t age)
{
	if(db[gender].size())
	{
		return getClosestAge(closest, db[gender], age);
	}

	getClosestAge(closest, db[1-gender], age);
	return false;
}

static
bool getClosestSpecies(Sprite *& closest, bySpecies & db, const std::string & name, bool gender, uint8_t age)
{
	auto i = db.lower_bound(name);

	if(i == db.end())
	{
		return false;
	}

	auto result = getClosestGender(closest, i->second, gender, age);
	return i->first == name? result : false;
}

extern "C" int isRightSide(int);

static
bool getClosestPart(Sprite *& closest, uint8_t part, std::string name, bool gender, uint8_t age)
{
	return getClosestSpecies(closest, skell_db()[part], name, gender, age);
}


Sprite * getSprite(uint8_t part, bool gender, uint8_t age, std::string name)
{
	name = getLower(name);

	if(isRightSide(part))
	{
		part = getMirror(part);
	}

	Sprite * closest = 0L;

	if(getClosestPart(closest, part, name, gender, age))
	{
		return closest;
	}

	if(skell_db()[part].find(name) != skell_db()[part].end() && closest)
	{
		return closest;
	}

//no exact match, look for new sprites
	for(DirectoryRange range(smSpriteFolder); !range.empty(); range.popFront())
	{
		if(!range.isDirectory())
		{
			continue;
		}

		if(getLower(range.fullname()) != name)
		{
			continue;
		}

		if(skell_db()[part].find(name) == skell_db()[part].end())
		{
			for(int i = 0; i < TOTAL_PARTS; ++i)
			{
				skell_db()[i].insert(std::pair<StringTableEntry, byGender>(name, byGender()));
			}
		}


		for(FileTypeRange<DirectoryRange> files(range.directory(), range.fullname(), smSpriteExtension); !files.empty(); files.popFront())
		{
			auto fname = files.filename();

			if(!(fname.size() == 3
			//body part
			&& 'a' <= tolower(fname[0]) && tolower(fname[0]) <= 'z'
			&& ('m' == tolower(fname[1]) || tolower(fname[1]) == 'f')
			&& '0' <= fname[2] && fname[2] <= '9') )
			{
				continue;
			}

			FILE * file = fopen(files.syspath().c_str(), "rb");
			if(!file)
			{
				continue;
			}

			int _one;
			fread(&_one, 4, 1, file);
			if(byte_swap(_one) != 1)
			{
				fclose(file);
				continue;
			}

			auto spr = Sprite(file, files.syspath(), part_from_letter(fname[0]), 'm' == tolower(fname[1]), fname[2] - '0');
			fclose(file);

			auto & v = skell_db()[part_from_letter(fname[0])][name][tolower(fname[1]) == 'm'];
			for(auto i = v.begin(); i != v.end(); ++i)
			{
				if(i->age() > fname[2] - '0')
				{
					v.emplace(i, std::move(spr));
					goto continue_loop;
				}
				else if(i->age() == fname[2] - '0')
				{
					goto continue_loop;
				}
			}

			v.emplace_back(std::move(spr));
continue_loop: (void)0;
		}
		break;
	}

//now load body data
	for(DirectoryRange range(smBodyDataFolder); !range.empty(); range.popFront())
	{
		if(!range.isDirectory())
		{
			continue;
		}

		if(getLower(range.fullname()) != name)
		{
			continue;
		}

	//no image loaded, confusing...
		if(skell_db()[part].find(name) == skell_db()[part].end())
		{
			break;
		}

		for(FileTypeRange<DirectoryRange> files(range.directory(), range.fullname(), smBodyDataExtension); !files.empty(); files.popFront())
		{
			auto fname = files.filename();

			if(!(fname.size() == 3
			//body part
			&& 'a' <= tolower(fname[0]) && tolower(fname[0]) <= 'z'
			&& ('m' == tolower(fname[1]) || tolower(fname[1]) == 'f')
			&& '0' <= fname[2] && fname[2] <= '9'))
			{
				continue;
			}

			auto & v = skell_db()[part_from_letter(fname[0])][name][tolower(fname[1]) == 'm'];

			for(auto i = v.begin(); i != v.end(); ++i)
			{
				if(i->age() == fname[2] - '0')
				{
					i->addAttachments(files.syspath());
					break;
				}
			}
		}

		break;
	}

	getClosestPart(closest, part, name, gender, age);
	return closest;
}
