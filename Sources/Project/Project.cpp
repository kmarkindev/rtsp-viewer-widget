#include "Project.h"

#include <QJsonObject>
#include <QJsonArray>
#include "Project/RtspDataSource.h"


QJsonObject Project::toJson() const
{
	QJsonObject result {};

	result["name"] = name;
	result["created_at"] = createdAt.toUTC().toString("yyyy-mm-dd hh:mm:ss");
	result["updated_at"] = updatedAt.toUTC().toString("yyyy-mm-dd hh:mm:ss");

	QJsonArray rtspSources {};

	for (const auto& rtspDataSource : rtspDataSources)
		rtspSources.append(rtspDataSource.toJson());

	result["rtsp_data_sources"] = rtspSources;

	return result;
}

Project Project::fromJson(QJsonObject jsonObject)
{
	Project project {};

	if(QJsonValue val = jsonObject["name"]; val.isString())
		project.name = val.toString();

	if(QJsonValue val = jsonObject["created_at"]; val.isString())
		project.createdAt = QDateTime::fromString(val.toString(), "yyyy-mm-dd hh:mm:ss");

	if(QJsonValue val = jsonObject["updated_at"]; val.isString())
		project.updatedAt = QDateTime::fromString(val.toString(), "yyyy-mm-dd hh:mm:ss");

	if(QJsonValue val = jsonObject["rtsp_data_sources"]; val.isArray())
	{
		for(const auto& jsonValue : val.toArray())
		{
			if(jsonValue.isObject())
			{
				project.rtspDataSources.append(
					RtspDataSource::fromJson(jsonValue.toObject())
				);
			}
		}
	}

	return project;
}
