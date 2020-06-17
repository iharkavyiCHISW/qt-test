#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QString>
#include <QVector>
#include <QBitmap>

#include "userinfo.h"

class QJsonObject;

class ProjectInfo
{
    int id_;
    QString name_;
    QString uid_;
    QString logoUrl_;
    int position_;
    bool isActive_;
    bool isOwnerWatcher_;
    QString timeWeek_;
    QString timeMonth_;
    QString timeTotal_;
    QVector<UserInfo> users_;

    QBitmap logo_;
public:
    ProjectInfo(const QJsonObject& jsonProjectInfo);

    int getId() const;
    const QString& getName() const;
    const QString& getUid() const;
    const QString& getLogoUrl() const;
    int getPosition() const;
    bool isActive() const;
    const QString& getTimeWeek() const;
    const QString& getTimeMonth() const;
    const QString& getTimeTotal() const;
    const QVector<UserInfo>& getUsers() const;

    void setLogo(const QBitmap& logo);
    const QBitmap& getLogo() const;
};

#endif // PROJECTINFO_H
