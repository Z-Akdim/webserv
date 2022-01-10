#include "parssingfile.hpp"

ParssFile::ParssFile(int ac, char **av)
{
    this->check_argument(ac, av);
}

void    ParssFile::check_argument(int ac, char **argv)
{
    if (ac < 1)
        throw std::runtime_error("Error: bad argument");
    else if (argv[1])
        this->file_name = argv[1];
    else
        this->file_name = "confg/config.conf";
    this->extention = &this->file_name[static_cast<int>(this->file_name.find('.') + 1)];
}

void    ParssFile::fill_file_content()
{
    std::string f
}

void    ParssFile::check_bracket_brace_file()
{
    std::string ptr;
    int open_bracket = 0;
    int close_bracket = 0;
    int open_brace = 0;
    int close_brace = 0;

    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        open_bracket = open_bracket + std::count(ptr.begin(), ptr.end(), '[');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        close_bracket = close_bracket + std::count(ptr.begin(), ptr.end(), ']');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        open_brace = open_brace + std::count(ptr.begin(), ptr.end(), '{');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        close_brace = close_brace + std::count(content_file.begin(), content_file.end(), '}');
    }
    if (open_bracket != close_bracket)
        throw std::runtime_error("error: messing a open_bracket or close_bracket");
    if (open_brace != close_brace)
        throw std::runtime_error("error: messing a open_brace or close_brace");
}

// ParssFile::~ParssFile();