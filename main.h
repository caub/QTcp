

#include <QtNetwork>

class MyTcpServer : public QTcpServer
{
	Q_OBJECT

public:
	MyTcpServer();

public slots:
	void slotNewConnection();
	void parseData();

private:
	QTcpSocket* connection;

};

