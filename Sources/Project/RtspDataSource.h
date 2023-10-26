#pragma once

#include <QObject>

class RtspDataSource
{
public:

	QString name {};

	QByteArray url {};

	[[nodiscard]] QJsonObject toJson() const;
	static RtspDataSource fromJson(QJsonObject jsonObject);

};
