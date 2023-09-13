#pragma once

#include <QObject>

class RtspDataSource
{
public:

	RtspDataSource() = default;
	explicit RtspDataSource(QByteArray url);

	QJsonObject toJson() const;
	static RtspDataSource fromJson(QJsonObject jsonObject);

private:

	QByteArray url {};

};
