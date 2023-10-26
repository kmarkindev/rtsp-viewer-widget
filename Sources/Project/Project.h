#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include "RtspDataSource.h"
#include <deque>

class Project
{
public:

	QString name {};
	QDateTime createdAt {};
	QDateTime updatedAt {};

	std::deque<RtspDataSource> rtspDataSources {};

	[[nodiscard]] QJsonObject toJson() const;
	static Project fromJson(QJsonObject jsonObject);
};
