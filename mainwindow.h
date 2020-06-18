#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidgetList>
#include <QListWidgetItem>

#include <QJsonObject>
#include <QNetworkAccessManager>

#include "logindialog.h"
#include "projectinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void updateInterface();
    void authFinished(const QString& token);
    void logout();
    void loadedProjectList();
    void loadedProjectLogo();
    void loadedUserAvatar();
    void applyProjectChanges();
    void projectChangesResult();

    void on_projectList__currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_UserList__currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:

    void loadProjectList();
    void loadProjectLogo(const ProjectInfo& project);
    void loadUserAvatar(const UserInfo& info);

    void addProjectsToUI();
    void clearUI();
    void clearPixmap(QLabel* widget);
    QPixmap createMockIcon(const QString& name);
    void showUI();
    void displayProject(ProjectInfo& project);
    void displayUser(UserInfo& user);
    void drawPixmap(QLabel* widget, const QPixmap& pixmap);
    void hideUI();

private:
    Ui::MainWindow *ui;
    LoginDialog* loginDialog_;

    QNetworkAccessManager* manager_;

    QUrl url_;
    QString authToken_;

    QVector<ProjectInfo> projects_;
    QMap<int, UserInfo> users_;
};
#endif // MAINWINDOW_H
