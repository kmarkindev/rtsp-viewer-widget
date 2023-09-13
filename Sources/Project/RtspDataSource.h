#pragma once

#include <QObject>

class RtspDataSource
{
public:

	QByteArray url {};

	[[nodiscard]] QJsonObject toJson() const;
	static RtspDataSource fromJson(QJsonObject jsonObject);

};
