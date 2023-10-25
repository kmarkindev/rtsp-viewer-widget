#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include "RtspDataSource.h"

class Project
{
public:

	QString name {};
	QDateTime createdAt {};
	QDateTime updatedAt {};

	QVector<RtspDataSource> rtspDataSources {};

	[[nodiscard]] QJsonObject toJson() const;
	static Project fromJson(QJsonObject jsonObject);
};
