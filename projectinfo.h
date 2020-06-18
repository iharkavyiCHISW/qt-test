#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QString>
#include <QVector>
#include <QPixmap>

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
    QString timeWeek_ = "00:00:00";
    QString timeMonth_ = "00:00:00";
    QString timeTotal_ = "00:00:00";
    QVector<int> users_;

    QPixmap logo_;
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
    void addUser(int id);
    const QVector<int>& getUsers() const;
    QVector<int> &getUsers();

    void setLogo(const QPixmap& logo);
    const QPixmap& getLogo() const;
};

#endif // PROJECTINFO_H
