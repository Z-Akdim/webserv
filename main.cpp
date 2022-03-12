#include "headers/server.hpp"

int main(int ac, char **av)
{
    try
    {

        ParssFile parss(ac, av);
        signal(SIGPIPE, SIG_IGN);
        std::vector<dataserver> CurrrentServer = parss.SplitServers();
        SERVER::ASERVER runServer;
        runServer.launch(CurrrentServer);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}