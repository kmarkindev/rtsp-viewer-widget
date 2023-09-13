#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>

class RtspDataSource;

class Project
{
public:

	QString name {};
	QDateTime createdAt {};
	QDateTime updatedAt {};

	QVector<RtspDataSource> rtspDataSources {};

	Project() = default;

	[[nodiscard]] QJsonObject toJson() const;
	static Project fromJson(QJsonObject jsonObject);
};
