#include "ProjectSaveLoad.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "Project.h"

Project loadFromFile(QString filePath, bool& error)
{
	error = false;

	QFile file { filePath };

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		error = true;
		return {};
	}

	QByteArray fileContent = file.readAll();

	QJsonParseError jsonError {};
	QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &jsonError);

	if(jsonError.error != QJsonParseError::NoError)
	{
		error = true;
		return {};
	}

	if(!jsonDoc.isObject())
	{
		error = true;
		return {};
	}

	return Project::fromJson(jsonDoc.object());
}

void saveToFile(const Project& project, QString filePath, bool& error)
{
	error = false;

	QFile file { filePath };

	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		error = true;
		return;
	}

	QJsonObject jsonObj = project.toJson();
	QJsonDocument jsonDoc {jsonObj};

	file.write(jsonDoc.toJson());
}