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
    isActive_ = jsonProjectInfo["is_active"].toInt();
    isOwnerWatcher_ = jsonProjectInfo["is_owner_watcher"].toInt();
    timeWeek_ = jsonProjectInfo["spent_time_week"].toString();
    timeMonth_ = jsonProjectInfo["spent_time_month"].toString();
    timeTotal_ = jsonProjectInfo["spent_time_all"].toString();
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

void ProjectInfo::addUser(int id)
{
    users_.push_back(id);
}

const QVector<int> &ProjectInfo::getUsers() const
{
    return users_;
}

QVector<int> &ProjectInfo::getUsers()
{
    return users_;
}

void ProjectInfo::setLogo(const QPixmap& logo)
{
    logo_ = logo;
}

const QPixmap& ProjectInfo::getLogo() const
{
    return logo_;
}
