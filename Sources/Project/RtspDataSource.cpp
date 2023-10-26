#include "RtspDataSource.h"

#include <QJsonObject>

QJsonObject RtspDataSource::toJson() const
{
	QJsonObject result {};

	result["url"] = url.data();
	result["name"] = result;

	return result;
}

RtspDataSource RtspDataSource::fromJson(QJsonObject jsonObject)
{
	RtspDataSource result {};

	if(QJsonValue val = jsonObject["url"]; val.isString())
		result.url = val.toString().toUtf8();

	if(QJsonValue val = jsonObject["name"]; val.isString())
		result.name = val.toString();

	return result;
}
