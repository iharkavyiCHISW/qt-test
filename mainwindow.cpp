#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkReply>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QPalette>
#include <QPaintEngine>
#include <QLabel>

#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , url_("https://api.quwi.com/v2/")
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    ui->projectLogo_->setScaledContents(true);
    ui->userAvatar_->setScaledContents(true);
    hideUI();

    loginDialog_ = new LoginDialog(url_, this);

    manager_ = new QNetworkAccessManager(this);

    QMetaObject::invokeMethod(this, &MainWindow::updateInterface, Qt::ConnectionType::QueuedConnection);

    auto connection = QObject::connect(loginDialog_, &LoginDialog::autorized, this, &MainWindow::authFinished);
    assert(connection);

    connection = QObject::connect(ui->logoutButton_, &QPushButton::pressed, this, &MainWindow::logout);
    assert(connection);

    connection = QObject::connect(ui->userList_, &QListWidget::currentItemChanged, this, &MainWindow::on_UserList__currentItemChanged);
    assert(connection);

    connection = QObject::connect(ui->projectNameApplyButton_, &QPushButton::clicked, this, &MainWindow::applyProjectChanges);
    assert(connection);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete loginDialog_;
}

void MainWindow::updateInterface()
{
    if (authToken_.isEmpty())
    {
        if (!loginDialog_->exec())
        {
            QApplication::quit();
            return;
        }

    }
    loadProjectList();
}

void MainWindow::loadProjectList()
{
    QUrl url(url_.url() + "projects-manage/index");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + authToken_.toUtf8());
    auto reply = manager_->get(request);
    auto connection = QObject::connect(reply, &QNetworkReply::finished, this, &MainWindow::loadedProjectList);
    assert(connection);
}

void MainWindow::loadProjectLogo(const ProjectInfo &project)
{
    QNetworkRequest request(QUrl(project.getLogoUrl()));
    request.setRawHeader("Authorization", "Bearer " + authToken_.toUtf8());
    auto reply = manager_->get(request);
    auto connection = QObject::connect(reply, &QNetworkReply::finished, this, &MainWindow::loadedProjectLogo);
    assert(connection);
}

void MainWindow::loadUserAvatar(const UserInfo &info)
{
    QNetworkRequest request(QUrl(info.getAvatarUrl()));
    request.setRawHeader("Authorization", "Bearer " + authToken_.toUtf8());
    auto reply = manager_->get(request);
    auto connection = QObject::connect(reply, &QNetworkReply::finished, this, &MainWindow::loadedUserAvatar);
    assert(connection);
}

void MainWindow::addProjectsToUI()
{
    for (const auto& project : projects_) {
        ui->projectList_->addItem(project.getName());
    }
}

void MainWindow::clearUI()
{
    ui->projectList_->clear();
    ui->projectName_->clear();
    ui->projectStatus_->clear();
    ui->projectNameField_->clear();
    ui->userList_->clear();
    ui->userName_->clear();
    clearPixmap(ui->projectLogo_);
    clearPixmap(ui->userAvatar_);
    
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

void MainWindow::clearPixmap(QLabel*widget)
{
    QPixmap empty(widget->size());
    empty.fill();
    drawPixmap(widget, empty);
}
QPixmap MainWindow::createMockIcon(const QString& name)
{
    QPixmap result(100, 100);
    result.fill(Qt::transparent);

    QPainter painter(&result);
    QColor color(rand() % 256, rand() % 256, rand() % 256);
    painter.setBrush(QBrush(color));
    painter.drawEllipse(5, 5, 90, 90);


    color.setRed(255 - color.red());
    color.setGreen(255 - color.green());
    color.setBlue(255 - color.blue());
    painter.setPen(color);
    auto words = name.split(" ");
    QString shortName;
    if (words.size() == 0)
    {
        shortName += (char)65 + rand() % (90 - 65); // random upper letter 
    }
    else if (words.size() == 1)
    {
        shortName += words[0][0].toUpper();
    }
    else
    {
        shortName += words[0][0].toUpper();
        shortName += words[1][0].toUpper();
    }

    auto font = painter.font();
    font.setPixelSize(50);
    painter.setFont(font);
    painter.drawText(25, 65, shortName);

    return result;
}

void MainWindow::displayProject(ProjectInfo &project)
{
    // name
    ui->projectName_->setText(project.getName());
    ui->projectNameField_->setText(project.getName());

    // status
    QPalette palette = ui->projectStatus_->palette();
    if (project.isActive())
    {
        ui->projectStatus_->setText("Active");
        ui->projectIsActiveCheckBox_->setChecked(true);
        palette.setColor(QPalette::WindowText, Qt::green);
    }
    else
    {
        ui->projectStatus_->setText("Inactive");
        ui->projectIsActiveCheckBox_->setChecked(false);
        palette.setColor(QPalette::WindowText, Qt::red);
    }
    ui->projectStatus_->setPalette(palette);

    // logo
    if (project.getLogo().isNull())
    {
        if (project.getLogoUrl().isEmpty())
        {
            project.setLogo(createMockIcon(project.getName()));
            drawPixmap(ui->projectLogo_, project.getLogo());
        }
        else
            loadProjectLogo(project);
    }
    else
    {
        drawPixmap(ui->projectLogo_, project.getLogo());

    }

    // times
    ui->timeWeek_->setText(project.getTimeWeek());
    ui->timeMonth_->setText(project.getTimeMonth());
    ui->timeTotal_->setText(project.getTimeTotal());

    ui->userList_->clear();
    ui->userName_->setText("");
    clearPixmap(ui->userAvatar_);

    for (auto userId : project.getUsers())
    {
        ui->userList_->addItem(users_[userId].getName());
    }
}

void MainWindow::displayUser(UserInfo &user)
{
    ui->userName_->setText(user.getName());

    // avatar
    if (user.getAvatar().isNull())
    {
        if (user.getAvatarUrl().isEmpty())
        {
            user.setAvatar(createMockIcon(user.getName()));
            drawPixmap(ui->userAvatar_, user.getAvatar());
        }
        else
            loadUserAvatar(user);
    }
    else
    {
        drawPixmap(ui->userAvatar_, user.getAvatar());
    }
}

void MainWindow::drawPixmap(QLabel *widget, const QPixmap & pixmap)
{
    widget->setPixmap(pixmap);
}

void MainWindow::authFinished(const QString& token)
{
    authToken_ = token;
}

void MainWindow::logout()
{
    authToken_ = "";
    hideUI();
    projects_.clear();
    users_.clear();
    clearUI();
    updateInterface();
}

void MainWindow::loadedProjectList()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        throw;

    reply->deleteLater();

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
    for (const auto& jsonProjectVal : jsonProjectsArray)
    {
        if (!jsonProjectVal.isObject())
            throw;
        auto jsonProjectObj = jsonProjectVal.toObject();

        ProjectInfo project(jsonProjectObj);

        auto jsonUsers = jsonProjectObj["users"];
        if (!jsonUsers.isArray())
            throw;
        auto jsonUsersArray = jsonUsers.toArray();
        for (const auto& jsonUser : jsonUsersArray)
        {
            if (!jsonUser.isObject())
                throw;
            UserInfo user(jsonUser.toObject());
            users_[user.getUserID()] = user;
            project.addUser(user.getUserID());
        }

        projects_.push_back(project);
    }
    showUI();
    addProjectsToUI();
}

void MainWindow::loadedProjectLogo()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        throw;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox errorBox(this);
        errorBox.setText(reply->errorString());
        errorBox.exec();
        return;
    }

    auto it = std::find_if(projects_.begin(), projects_.end(), [=](ProjectInfo& project) { return QUrl(project.getLogoUrl()) == reply->request().url(); });
    if (it == projects_.end())
        throw;

    QPixmap logo;
    auto data = reply->readAll();
    if (data.isEmpty())
        logo = createMockIcon(it->getName());
    else
        logo.loadFromData(data);

    it->setLogo(logo);

    if (ui->projectList_->currentItem()->text() == it->getName())
        displayProject(*it);
}

void MainWindow::loadedUserAvatar()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        throw;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox errorBox(this);
        errorBox.setText(reply->errorString());
        errorBox.exec();
        return;
    }

    auto it = std::find_if(users_.begin(), users_.end(), [=](UserInfo& user) { return QUrl(user.getAvatarUrl()) == reply->request().url(); });
    if (it == users_.end())
        return;

    QPixmap avatar;
    auto data = reply->readAll();
    if (data.isEmpty())
        avatar = createMockIcon(it->getName());
    else
        avatar.loadFromData(data);

    it->setAvatar(avatar);

    if (ui->userList_->currentItem()->text() == it->getName())
        displayUser(*it);
}

void MainWindow::applyProjectChanges()
{
    auto newName = ui->projectNameField_->text();

    if (newName.isEmpty())
    {
        QMessageBox errorBox(this);
        errorBox.setText("Project name can't be empty");
        errorBox.exec();
        return;
    }

    auto selectedProjectName = ui->projectList_->currentItem()->text();
    auto it = std::find_if(projects_.begin(), projects_.end(), [=](ProjectInfo& info) { return info.getName() == selectedProjectName; });
    if (it == projects_.end())
        throw;

    QNetworkRequest request(url_.url() + "projects-manage/update?id=" + QString().setNum(it->getId()));
    QJsonObject json;
    QJsonDocument doc;
    json["name"] = newName;
    doc.setObject(json);
    auto data = doc.toJson();
    request.setRawHeader("Authorization", "Bearer " + authToken_.toUtf8());
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    auto reply = manager_->post(request, data);

    auto result = connect(reply, &QNetworkReply::finished, this, &MainWindow::projectChangesResult);
    assert(result);
    
}

void MainWindow::projectChangesResult()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        throw;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox errorBox(this);
        errorBox.setText(reply->errorString());
        errorBox.exec();
        return;
    }

    clearUI();
    hideUI();
    projects_.clear();
    users_.clear();

    updateInterface();
}


void MainWindow::on_projectList__currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
    if (!current || ui->projectList_->count() == 0)
        return;

    auto it = std::find_if(projects_.begin(), projects_.end(), [=](ProjectInfo& project) { return project.getName() == current->text(); });
    if (it == projects_.end())
        throw;

    displayProject(*it);
}

void MainWindow::on_UserList__currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
    if (!current || ui->userList_->count() == 0)
        return;

    auto it = std::find_if(users_.begin(), users_.end(), [=](UserInfo& user) { return user.getName() == current->text(); });
    if (it == users_.end())
        throw;

    displayUser(*it);
}
