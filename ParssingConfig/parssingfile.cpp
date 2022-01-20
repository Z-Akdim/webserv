#include "parssingfile.hpp"
#include "location.hpp"

ParssFile::ParssFile(int ac, char **av)
{
    this->check_argument(ac, av);
    this->fill_file_content();
}

void    ParssFile::check_argument(int ac, char **argv)
{
    if (ac < 1)
        throw std::runtime_error("Error: bad argument");
    else if (argv[1])
        this->file_name = argv[1];
    else
        this->file_name = "../confg/config.conf";
    this->extention = &this->file_name[static_cast<int>(this->file_name.find('.') + 1)];
}

void    ParssFile::remove_spaces(std::string &refline)
{
    while (isspace(refline.front()))
        refline.erase(0, 1);
    while (isspace(refline.back()))
        refline.erase(refline.length() - 1, 1);
}

void    ParssFile::find_OpenClose_EachServer()
{
    int i = 0;
    if (content_file[0] != SERVER || content_file[1] != OPEN_BRACKET)
        throw std::runtime_error("Error: heck you config.conf");
    while (++i < content_file.size())
    {
        if (content_file[i].find(OPEN_BRACKET))
            index_server.push_back(i);
        if (content_file[i].find(CLOSE_BRACKET))
            index_server.push_back(i);
    }
}

void    ParssFile::delete_cmments(std::string &line, char c)
{
    int i = 0;

    while (line[i])
    {
        if (line[i] == c)
        {
            line.erase(i, line.length() - 1);
            break ;
        }
        i++;
    }
}

// void    ParssFile::_print(std::vector<std::string> line)
// { 
//     for (std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
//     {
//         std::cout << *it << std::endl;
//     }
    
// }

// void    ParssFile::_print(std::string type)
// {
  
//     for (int i = 0; i < type.size(); i++)
//     {
//         std::cout << "======>" << type[i] << std::endl;
//     }  
// }

void    ParssFile::fill_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string line;

    while(std::getline(file, line))
    {
        remove_spaces(line);
        if (line.empty() || line == "\n")
            continue;
        if (line[0] == COMMENT2 || isspace(line[0]))
            continue;
        if (line.find(COMMENT2) != std::string::npos)
            delete_cmments(line, COMMENT2);
        if (line.find(COMMENT1) != std::string::npos)
            delete_cmments(line, COMMENT1);
        std::cout << line << std::endl;
        this->content_file.push_back(line);
    }
    // _print(this->content_file);
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
    if ((open_bracket != close_bracket) || ((open_bracket = 0) && (close_bracket = 0)))
        throw std::runtime_error("Error: Cheke Your Bracket");
    if ((open_brace != close_brace) || ((open_brace = 0) && (close_brace = 0)))
        throw std::runtime_error("Error: Cheke Your Brace");
}

void    ParssFile::take_port(std::string &ptr, dataserver& dataSr)
{
    int lenght = strlen(SERVER);
    ptr.erase(remove_if(ptr.begin(), ptr.end(), isspace), ptr.end());
    if (ptr.empty())
        throw std::runtime_error("Error: Cheke Your Port");
    ptr.erase(0, lenght);
    if (ptr.empty())
        throw std::runtime_error("Error: Cheke Your Port");
    dataSr.addListen(atoi(ptr.c_str()));
    // _print(ptr);
}

void    ParssFile::take_host(std::string & strhost, dataserver& dataHost)
{
    int lenght = strlen(HOST);
    strhost.erase(remove_if(strhost.begin(), strhost.end(), isspace), strhost.end());
    if (strhost.empty())
        throw std::runtime_error("Error: Cheke Your Host");
    strhost.erase(0, lenght);
    if (strhost.empty())
        throw std::runtime_error("Error: Cheke Your Host");
    dataHost.setHost(strhost);
}

void    ParssFile::take_server_name(std::string & str, dataserver& dataServN)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Server Name");
    str.erase(0, strlen(SERVER_NAME));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Server Name");
    dataServN.setServer_name(str);
}

void    ParssFile::take_C_M_B_S(std::string &str, dataserver&dataCMBS)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Client_Max_Body_Size ");
    str.erase(0, strlen(CLIENT_MAX_BODY_SIZE));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Client_Max_Body_Size ");
    dataCMBS.setClient_max_body_size(atoi(str.c_str()));
}

int lenght_int(int nbr)
{
    int i = 0;
    while (nbr != 0)
    {
        nbr = nbr / 10;
        i++;
    }
    i++;
    return i;
}

void    ParssFile::take_Error_Page(std::string &str, dataserver& dataEPage)
{
    int nub_error = 0;
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    str.erase(0, strlen(ERROR_PAGE));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    nub_error = atoi(str.c_str());
    str.erase(0, lenght_int(nub_error));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    dataEPage.setError_page(nub_error, str);
}

void    ParssFile::take_Root(std::string &str, dataserver& dataRoot)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Root");
    str.erase(0, strlen(ROOT));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Root");
    dataRoot.setRoot(str);
}

void    ParssFile::take_L_autoindex(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Autoindex In Your Location Server");
    str.erase(0, strlen(AUTOINDEX));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Autoindex In Your Location Server");
    str.compare("on") == 0 ? loc.setL_AutoIndex(1): str.compare("off") == 0 ? loc.setL_AutoIndex(0): loc.setL_AutoIndex(-1);   
}

void    ParssFile::take_L_index(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Index In Your Location Server");
    str.erase(0, strlen(INDEX));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Index In Your Location Server");
    loc.setL_Index(str);
}

void    ParssFile::take_L_fastcgi_pass(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Fastcgi_Pass In Your Location Server");
    str.erase(0, strlen(FASTCGI_PASS));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Fastcgi_Pass In Your Location Server");
    loc.setL_Fastcgi_Pass(str);
}

void    ParssFile::take_L_Allow_Methods(std::string &str, location &loc)
{
    std::map<std::string, int> methodes;
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    str.erase(0, strlen(ALLOW_METHODS));
    if (str.empty())
        throw std::runtime_error("Error: Cheke allow_methods In Your Location Server");
    if (str.find("GET") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("GET", 1));
    if (str.find("POST") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("POST", 1));
    if (str.find("DELETE") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("DELETE", 1));
    loc.setL_Allowed_Methods(methodes);
}

ParssFile::~ParssFile(){};