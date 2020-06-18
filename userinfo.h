#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QPixmap>

class QJsonObject;

class UserInfo
{
    int id_;
    int companyId_;
    QString role_;
    QString name_;
    QString avatarUrl_;
    bool isOnline_;
    QString dtaActivity_;
    bool isTimerOnline_;
    QString dtaTimerActivity_;
    QString timeZone_;
    QString timeLast_;

    QPixmap avatar_;

public:
    UserInfo(const QJsonObject& jsonUserInfo);
    UserInfo() {}

    int getUserID() const;
    int getCompanyID() const;
    const QString& getRole() const;
    const QString& getName() const;
    const QString& getAvatarUrl() const;
    bool isOnline() const;

    void setAvatar(const QPixmap& avatar);
    const QPixmap& getAvatar() const;

};

#endif // USERINFO_H
