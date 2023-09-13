#include "RtspDataSource.h"

#include <QJsonObject>
#include <utility>

RtspDataSource::RtspDataSource(QByteArray url)
	: url(std::move(url))
{

}

QJsonObject RtspDataSource::toJson() const
{
	QJsonObject result {};

	result["url"] = url.data();

	return result;
}

RtspDataSource RtspDataSource::fromJson(QJsonObject jsonObject)
{
	RtspDataSource result {};

	if(QJsonValue val = jsonObject["url"]; val.isString())
		result.url = val.toString().toUtf8();

	return {};
}
