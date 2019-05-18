#include <mainWindow.h>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(signs);

	MainWindow window;

	window.show();

	return app.exec();
}