#include "projectinfo.h"

#include <QJsonArray>
#include <QJsonObject>

ProjectInfo::ProjectInfo(const QJsonObject &jsonProjectInfo)
{
    id_ = jsonProjectInfo["id"].toInt();
    name_ = jsonProjectInfo["name"].toString();
    uid_ = jsonProjectInfo["uid"].toString();
    logoUrl_ = jsonProjectInfo["logo_url"].toString();
    position_ = jsonProjectInfo["position"].toInt();
    isActive_ = jsonProjectInfo["is_active"].toBool();
    isOwnerWatcher_ = jsonProjectInfo["is_owner_watcher"].toBool();
    timeWeek_ = jsonProjectInfo["spent_time_week"].toString();
    timeMonth_ = jsonProjectInfo["spent_time_month"].toString();
    timeTotal_ = jsonProjectInfo["spent_time_all"].toString();
    auto jsonUsers = jsonProjectInfo["users"];
    if (!jsonUsers.isArray())
        throw;
    auto jsonUsersArray = jsonUsers.toArray();
    for (const auto& jsonUser : jsonUsersArray)
    {
        if (!jsonUser.isObject())
            throw;
        users_.push_back(UserInfo(jsonUser.toObject()));
    }
}

int ProjectInfo::getId() const
{
    return id_;
}

const QString &ProjectInfo::getName() const
{
    return name_;
}

const QString &ProjectInfo::getUid() const
{
    return uid_;
}

const QString &ProjectInfo::getLogoUrl() const
{
    return logoUrl_;
}

int ProjectInfo::getPosition() const
{
    return position_;
}

bool ProjectInfo::isActive() const
{
    return isActive_;
}

const QString &ProjectInfo::getTimeWeek() const
{
    return timeWeek_;
}

const QString &ProjectInfo::getTimeMonth() const
{
    return timeMonth_;
}

const QString &ProjectInfo::getTimeTotal() const
{
    return timeTotal_;
}

const QVector<UserInfo> &ProjectInfo::getUsers() const
{
    return users_;
}

void ProjectInfo::setLogo(const QBitmap &logo)
{
    logo_ = logo;
}

const QBitmap &ProjectInfo::getLogo() const
{
    return logo_;
}
