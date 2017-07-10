#ifndef _DIRECTORY_RANGE_H_
#define _DIRECTORY_RANGE_H_
#include <cstdint>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <string>
#include <vector>
#include <dirent.h>

class DirectoryRange
{
private:
    DIR * dp;
    dirent *ep;
	const std::string _path;

public:
	DirectoryRange(const std::string & _path, const std::string & file);
    DirectoryRange(const std::string & _path);
    virtual ~DirectoryRange();

    bool empty() const;
	bool isDirectory() const;
	bool isFile() const;
	bool isExtension(const std::string & ext) const;
    virtual void popFront();

	virtual const std::string & directory() const;
    virtual std::string fullname() const;

	virtual int depth() const { return 0; }

	std::string path() const;
	std::string syspath() const;
	std::string filename() const;
	std::string extension() const;
};

class RecursiveDirectoryRange : public DirectoryRange
{
typedef DirectoryRange super;
    RecursiveDirectoryRange * child;

public:
	RecursiveDirectoryRange(const std::string & _path, const std::string & file);
    RecursiveDirectoryRange(const std::string & _path);
    ~RecursiveDirectoryRange();

    void popFront();


	int depth() const
	{
		if(child)
		{
			return child->RecursiveDirectoryRange::depth() + 1;
		}

		return 0;
	}



#define recursive(function)\
function() const\
{\
	return child? child->RecursiveDirectoryRange::function() : super::function();\
}

	const std::string & recursive(directory)
	std::string recursive(fullname)

#undef recursive
};

template<class T>
class FileRange : public T
{
typedef T super;

public:
    FileRange(const std::string & path) :
	super(path) {}

    void popFront()
    {
		for(super::popFront(); !super::empty() && !super::isFile(); super::popFront()) ;
    }
};

template<class T>
class FileTypeRange : public T
{
typedef T super;
	const std::string ext;

	void pop()
    {
        for(; !super::empty(); super::popFront())
        {
			if(!super::isFile())
			{
				continue;
			}

			if(super::isExtension(ext))
			{
				break;
			}
        }
    }

public:
	FileTypeRange(const std::string & dir, const std::string & file, const std::string & ext) :
		super(dir, file),
		ext(ext)
    {
		pop();
    }

    FileTypeRange(const std::string & path, const std::string & ext) :
		super(path),
		ext(ext)
    {
		pop();
    }


    void popFront()
    {
        super::popFront();
		pop();
    }
};

template<class T>
class FileTypesRange : public T
{
typedef T super;
	const std::vector<std::string> extensions;

public:
	FileTypesRange(std::string path, std::initializer_list<std::string> args) :
		super(path),
		extensions(args)
    {
    }


    void popFront()
    {
        for(super::popFront(); !super::empty(); super::popFront())
        {
			if(!super::isFile())
			{
				continue;
			}

			for(auto i = extensions.begin(); i != extensions.end(); ++i)
			{
				if(super::isExtension(*i))
				{
					return;
				}
			}
        }

		return;
    }
};
/*
struct Directories
{
    static FileRange<RecursiveDirectoryRange> appletData()
    {
        return FileRange<RecursiveDirectoryRange>("Applet Data");
    }

    static FileTypeRange<DirectoryRange> aphrodisiacs()
    {
        static const char * extensions[] = {".obj"};
        return FileTypeRange<DirectoryRange>("Applet Data/Aphrodisiacs", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> medicine()
    {
        static const char * extensions[] = {".obj"};
        return FileTypeRange<DirectoryRange>("Applet Data/Medicine", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> eggs()
    {
        static const char * extensions[] = {".egg"};
        return FileTypeRange<DirectoryRange>("Eggs", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> genetics()
    {
        static const char * extensions[] = {".gen", ".gno"};
        return FileTypeRange<DirectoryRange>("Genetics", 2, extensions);
    }

    static FileTypeRange<DirectoryRange> objects()
    {
        static const char * extensions[] = {".obj"};
        return FileTypeRange<DirectoryRange>("Objects", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> backgrounds()
    {
        static const char * extensions[] = {".blk"};
        return FileTypeRange<DirectoryRange>("Backgrounds", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> images()
    {
        static const char * extensions[] = {".f32"};
        return FileTypeRange<DirectoryRange>("Images", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> sounds()
    {
        static const char * extensions[] = {".wav", ".smf", "flac", ".mng"};
        return FileTypeRange<DirectoryRange>("Sounds", 4, extensions);
    }

    static FileTypeRange<DirectoryRange> exports()
    {
        static const char * extensions[] = {".exp"};
        return FileTypeRange<DirectoryRange>("Exports", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> worlds()
    {
        static const char * extensions[] = {".sfc"};
        return FileTypeRange<DirectoryRange>("Worlds", 1, extensions);
    }

    static FileTypeRange<DirectoryRange> saves()
    {
        static const char * extensions[] = {".mmc"};
        return FileTypeRange<DirectoryRange>("Save Data", 1, extensions);
    }
};
*/

#endif // _DIRECTORY_RANGE_H_
