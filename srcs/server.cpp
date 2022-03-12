#include "../headers/server.hpp"
#include <stdio.h>

namespace SERVER
{
	void ASERVER::launch(std::vector<dataserver> servers)
	{
		data_servers = servers;
		FD_ZERO(&_masterRFDs);
		FD_ZERO(&_masterWFDS);
		FD_ZERO(&_readFDs);
		bool alreadyInUse = false;
		std::vector<dataserver>::iterator itt;
		std::cout << "Begin setup ...  " << std::endl;
		for (std::vector<dataserver>::iterator it = servers.begin(); it != servers.end(); it++)
		{
			for (itt = servers.begin(); itt != it; itt++)
			{
				if (it->getListen() == itt->getListen())
				{
					alreadyInUse = true;
					break;
				}
			}

				this->_data_server = *it;
			if (alreadyInUse == false)
			{
				_socket.SetupSocket(it->getListen());
				this->_masterRFDs = _socket._masterRFDs;
				this->_masterWFDS = _socket._masterWFDS;
				this->_maxSockFD = _socket._maxSockFD;
				this->_masterSockFDs = _socket._masterSockFDs;
				this->_ports = _socket._ports;
				this->_port = _socket._port;
				this->_Adrress = _socket._Adrress;
				this->_addrLen = _socket._addrLen;
			}
		}
		std::cout << "End setup " << std::endl;
		waitClients();
	}

	size_t getContentLen(std::string request)
	{
		size_t pos = 0;
		size_t find_pos = 0;
		size_t end_pos = request.find("\n");

		while (end_pos != npos)
		{
			std::string l = request.substr(pos, end_pos - pos);

			if ((find_pos = l.find("Content-Length:")) != npos && find_pos == 0)
				return (atoi(l.substr(l.find(":") + 2, l.length()).c_str()));
			else if ((find_pos = l.find("\r\n\r\n")) != npos && find_pos == 0)
				return (0);

			pos = end_pos + 1;
			end_pos = request.find("\n", pos);
		}
		return (0);
	}

	int checkContent(std::string request)
	{
		size_t begin = 0;
		size_t find_pos = 0;
		size_t end = request.find("\r\n");
		while (end != npos)
		{
			std::string header = request.substr(begin, end - begin);
			if ((find_pos = header.find("Content-Length")) != npos && find_pos == 0)
				return (1);
			else if ((find_pos = header.find("\r\n\r\n")) != npos && find_pos == 0)
				return (0);
			begin = end + 1;
			end = request.find("\n", begin);
		}
		return (0);
	}

	bool checkReq(Client client)
	{
		int status;
		std::string request;
		size_t pos;
		std::string body;

		request = client.getRequest();
		status = checkContent(request);
		pos = request.find("\r\n\r\n");
		if (status == 0 && pos != npos)
		{
			return (true);
		}
		if (status == 1)
		{
			body = request.substr(pos + 4, request.length() - (pos + 4));
			if (client.GetRecRetSnd() - (pos + 4) >= getContentLen(request))
			{
				return (true);
			}
		}
		return (false);
	}

	void ASERVER::newClient(int &sockFD)
	{
		int accptSockFD = accept(sockFD, (struct sockaddr *)&_Adrress, &_addrLen);
		if (accptSockFD == -1)
			std::cerr << "[ERROR] Socket" << std::endl;
		std::cout << "New connection: Master socket " << std::to_string(sockFD) << ". Accept socket " + std::to_string(accptSockFD) << ", address " << inet_ntoa(_Adrress.sin_addr) << ":" << std::to_string(ntohs(_Adrress.sin_port)) << std::endl;
		std::string test = inet_ntoa(_Adrress.sin_addr);

		if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
			std::cerr << "ERROR] fcntl" << std::endl;
		FD_SET(accptSockFD, &_socket._masterRFDs);
		FD_SET(accptSockFD, &_socket._masterWFDS);
		if (accptSockFD > _maxSockFD)
			_maxSockFD = accptSockFD;

		std::ofstream outfile;
		Client *newClient = new Client(accptSockFD, "", inet_ntoa(_Adrress.sin_addr));
		_clients.push_back(newClient);
	}

	void ASERVER::waitClients()
	{
		char message[70] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\nhello\n";
		std::cout << "\n"
				  << "+++++++ Waiting for new connection +++++++"
				  << "\n";
		FD_ZERO(&_socket._writeFDs);
		while (running)
		{
			_socket._readFDs = _socket._masterRFDs;
			_socket._writeFDs = _socket._masterWFDS;
			_activity = select(_maxSockFD + 1, &_socket._readFDs, &_socket._writeFDs, NULL, NULL);
			if (_activity == -1)
				std::cerr << "[ERROR] SELECT" << std::endl;
			if (_activity > 0)
			{
				bool _treat = false;
				std::vector<int>::iterator it;
				for (it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
				{

					if (FD_ISSET(*it, &_socket._readFDs))
					{
						this->newClient(*it);
						break;
					}
				}

				for (int CurrentCli = 0; CurrentCli < _clients.size(); CurrentCli++)
				{

					Client &client = *_clients[CurrentCli];

					bool bool_treat = false;
					int sockFD = client.getSockFd();
					if (FD_ISSET(sockFD, &_socket._readFDs) && client.getEndofReq() == false)
					{
						char _buffRes[BUFFER_SIZE + 1];
						bzero(_buffRes, sizeof(_buffRes));
						int valRead = recv(sockFD, _buffRes, BUFFER_SIZE, 0);
						client.setRecRetSnd(client.GetRecRetSnd() + valRead);
						if (valRead < 1)
						{
							std::cout << "Disconnected socket: " << std::to_string(sockFD) << std::endl;
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._readFDs) == false)
									_maxSockFD--;
							_clients.erase(_clients.begin() + CurrentCli);
							CurrentCli--;
							if (CurrentCli < 0)
							{
								CurrentCli = 0;
								break;
							}
						}
						else if (valRead > 1)
						{
							client.appendReq(_buffRes, valRead);
							client.setReceived(checkReq(client));
							if (client.getReceived())
							{
								Request r(client.getRequest(), data_servers[0].getClient_max_body_size() * 1024 * 1024);
								r.parseRequest();
								_requset = r;
	
								Response resp(data_servers, _requset, 80);
								resp.init_response();
								client.setRequest(resp.getHeader());
							}
						}
					}

					if (FD_ISSET(sockFD, &_socket._writeFDs) && client.getReceived())
					{
						int SendRet = 0;
						std::string respStr = client.getRequest();
						client.setLenReq(respStr.length());
						int leng = client.getLenReq() > 294000 ? 294000 : client.getLenReq();
						;
						if (leng > 0)
						{
							SendRet = send(sockFD, respStr.c_str(), leng, 0);

							client.SendRetSnd(client.GetRetSnd() + SendRet);
							try
							{
								client.setRequest(client.getRequest().substr(SendRet));
							}
							catch (std::exception &e)
							{
								SendRet = -1;
							}
						}
						if (SendRet < 1)
						{
							std::cout << "Disconnected socket: " << std::to_string(sockFD) << std::endl;
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._masterRFDs) == false)
									_maxSockFD--;
							_clients.erase(_clients.begin() + CurrentCli);
							CurrentCli--;
							if (CurrentCli < 0)
							{
								CurrentCli = 0;
								break;
							}
						}
						else if (client.getRequest().length() == SendRet)
						{
							std::cout << "Disconnected socket: " << std::to_string(sockFD) << std::endl;
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._masterRFDs) == false)
									_maxSockFD--;
							_clients.erase(_clients.begin() + CurrentCli);

							CurrentCli--;
							if (CurrentCli < 0)
							{
								CurrentCli = 0;
								break;
							}
						}
					}
				}
			}
		}
	}
	Request ASERVER::getRequest()
	{
		return this->_requset;
	}

	dataserver ASERVER::getDataServer()
	{
		return this->_data_server;
	}
	ASERVER::~ASERVER(void)
	{
	}
}
