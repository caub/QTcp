
#include "main.h"
#include <QCoreApplication>

#include <iostream>
#include <sstream>
#include <vector> 

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	MyTcpServer server;

	if(server.listen(QHostAddress::QHostAddress("127.0.0.1"), 5557)) {
		std::cout << "Listening Port 5557"<< std::endl;
	}

	return a.exec();
}


MyTcpServer::MyTcpServer() {
	connect(this, SIGNAL(newConnection()), SLOT(slotNewConnection()));
}

void MyTcpServer::slotNewConnection()
{
	connection = nextPendingConnection();
	connect(connection, SIGNAL(readyRead()), SLOT(parseData()));
	connect(connection, SIGNAL(disconnected()), connection, SLOT(deleteLater()));
}

void MyTcpServer::parseData()
{
	QByteArray received = connection->readAll();

	std::string p;
	std::istringstream iss(received.data());
	std::vector<std::string> params; 
	
	// parsing action/name/value
	while(getline(iss, p, '/')){
		params.push_back(p);
	}
	int size = params.size();
	if (size>=2){
		double v = size>2 ? atof(params.at(2).c_str()) : 1;

		p = "processed: "+std::string(params.at(0))+" "+std::string(params.at(1))+" "+std::to_string(v);
		std::cout << p << " v: " << v << std::endl;
		connection->write(p);
	
	} else 
		connection->write("echo: "+received);

	connection->disconnectFromHost();
}