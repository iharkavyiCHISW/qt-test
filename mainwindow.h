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
    void loadedProjectList(QNetworkReply* reply);

    void on_projectList__currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    QVector<QJsonObject> getProjects();
    bool initiateAuth();
    void loadProjectList();
    void fillData();
    void clearUI();
    void clearImage(QWidget* widget);
    void showUI();
    void drawPixmap(QWidget* widget, const QPixmap& pixmap);
    void hideUI();

private:
    Ui::MainWindow *ui;
    LoginDialog* loginDial_;

    QNetworkAccessManager* manager_;
    QMetaObject::Connection currentNetManagerConnection_;

    QUrl url_;
    QString authToken_;

    QVector<ProjectInfo> projects_;
};
#endif // MAINWINDOW_H
