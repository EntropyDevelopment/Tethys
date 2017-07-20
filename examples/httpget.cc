/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include "Application.hh"
#include "UV/TcpClient.hh"

using namespace std;
using namespace Entropy;
using namespace Entropy::Asio;

using Entropy::Asio::UV::TcpClient;

class Usage :
	public Task
{
	public:
		Usage(const string &);
		void operator () ();
	private:
		string _name;
};

class Client :
	public TcpClient
{
	public:
		Client(const string &, const string &);
	private:
		void onConnect();
		void onData();
	private:
		string _host;
};

class Application :
	public Asio::Application
{
	public:
		Application(int, char *[]);
	private:
		shared_ptr<Usage> _usage;
		shared_ptr<TcpClient> _client;
};

int main(int ArgC, char *ArgV[])
{
	try
	{
		::Application app(ArgC, ArgV);
		app();

		return EXIT_SUCCESS;
	}
	catch(exception &e)
	{
		cerr << e << endl;
		return EXIT_FAILURE;
	}
}

::Application::Application(int argc, char *argv[])
	: Asio::Application(argc, argv), _usage(), _client()
{
	if(ArgC() != 3) {
		_usage = make_shared<Usage>(ArgV()[0]);

		Add(*_usage);
	} else {
		_client = make_shared<Client>(ArgV()[1], ArgV()[2]);

		Add(*_client);
	}
}

Client::Client(const string &host, const string &service)
	: TcpClient(host, service), _host(host)
{}

void Client::onConnect()
{
	*this << "GET / HTTP/1.1" << endl
		<< "Host: " << _host << endl
		<< "User-Agent: Entropy Asio Example" << endl
		<< "Accept: */*" << endl << endl;
}

void Client::onData()
{
	while(!this->eof()) {
		string line;

		std::getline(*this, line);
		cout << line << std::flush;
	}
}

Usage::Usage(const string &name)
	: _name(name)
{}

void Usage::operator () ()
{
	cout << _name << ": host port" << endl;
}