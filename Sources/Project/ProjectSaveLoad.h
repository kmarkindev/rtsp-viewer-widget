#pragma once

#include <QString>

class Project;

Project loadFromFile(QString filePath, bool& error);

void saveToFile(const Project& project, QString filePath, bool& error);