#include "userinfo.h"

#include <QJsonObject>

UserInfo::UserInfo(const QJsonObject &jsonUserInfo)
{
    id_ = jsonUserInfo["id"].toInt();
    companyId_ = jsonUserInfo["id_company"].toInt();
    role_ = jsonUserInfo["role"].toString();
    name_ = jsonUserInfo["name"].toString();
    avatarUrl_= jsonUserInfo["avatar_url"].toString();
    isOnline_ = jsonUserInfo["is_online"].toBool();
}

int UserInfo::getUserID() const
{
    return id_;
}

int UserInfo::getCompanyID() const
{
    return companyId_;
}

const QString &UserInfo::getRole() const
{
    return role_;
}

const QString &UserInfo::getName() const
{
    return name_;
}

const QString &UserInfo::getAvatarUrl() const
{
    return avatarUrl_;
}

bool UserInfo::isOnline() const
{
    return isOnline_;
}

void UserInfo::setAvatar(const QPixmap& avatar)
{
    avatar_ = avatar;
}

const QPixmap &UserInfo::getAvatar() const
{
    return avatar_;
}
