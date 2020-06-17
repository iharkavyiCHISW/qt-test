#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkReply>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QPalette>
#include <QPaintEngine>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , url_("https://api.quwi.com/v2/")
{
    ui->setupUi(this);
    ui->projectInfo->setHidden(true);
    ui-
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    loginDial_ = new LoginDialog(url_, this);

    manager_ = new QNetworkAccessManager(this);

    QMetaObject::invokeMethod(this, &MainWindow::updateInterface, Qt::ConnectionType::QueuedConnection);
    if (!QObject::connect(loginDial_, &LoginDialog::autorized, this, &MainWindow::authFinished))
        throw;

    if (!QObject::connect(ui->logoutButton_, &QPushButton::pressed, this, &MainWindow::logout))
        throw;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loginDial_;
}

void MainWindow::updateInterface()
{
    if (authToken_.isEmpty())
    {
        if (!initiateAuth())
        {
            QApplication::quit();
            return;
        }
    }
    loadProjectList();

}

bool MainWindow::initiateAuth()
{
    return loginDial_->exec();
}

void MainWindow::loadProjectList()
{
    const QUrl url(url_.url() + "projects-manage/index");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + authToken_.toUtf8());
    currentNetManagerConnection_ = QObject::connect(manager_, &QNetworkAccessManager::finished, this, &MainWindow::loadedProjectList);
    manager_->get(request);
}

void MainWindow::fillData()
{
    for (const auto& project : projects_) {
        ui->projectList_->addItem(project.getName());
    }
}

void MainWindow::clearUI()
{
    ui->projectList_->clear();
    ui->projectName_->clear();
    {
        QPainter painter;
    }
}

void MainWindow::clearPixmap(QWidget *widget)
{
    QPixmap empty(widget->size());
    empty.fill();
    drawPixmap(widget, empty);
}
void MainWindow::hideUI()
{
    ui->projectInfo->setHidden(true);
    ui->projectList_->setHidden(true);
}

void MainWindow::showUI()
{
    ui->projectInfo->setHidden(false);
    ui->projectList_->setHidden(false);
}

void MainWindow::drawPixmap(QWidget *widget, const QPixmap &pixmap)
{
    QPainter painter(widget);
    QRect rect = painter.viewport();
    QSize size = pixmap.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(pixmap.rect());
    painter.drawPixmap(0, 0, pixmap);
}

void MainWindow::authFinished(const QString& token)
{
    authToken_ = token;
}

void MainWindow::logout()
{
    authToken_ = "";
    updateInterface();
}

void MainWindow::loadedProjectList(QNetworkReply *reply)
{
    reply->deleteLater();
    QObject::disconnect(currentNetManagerConnection_);

    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox errorBox(this);
        errorBox.setText(reply->errorString());
        errorBox.exec();
        return;
    }
    auto data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    auto jsonProjects = doc["projects"];
    if (!jsonProjects.isArray())
        throw;
    auto jsonProjectsArray = jsonProjects.toArray();
    for (const auto& projectJson : jsonProjectsArray)
    {
        if (!projectJson.isObject())
            throw;
        projects_.push_back(ProjectInfo(projectJson.toObject()));
    }

    fillData();
}


void MainWindow::on_projectList__currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{

}
