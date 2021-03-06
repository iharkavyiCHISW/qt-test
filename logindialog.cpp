#include "logindialog.h"
#include "ui_logindialog.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <string>

LoginDialog::LoginDialog(const QUrl &url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    url_(url.url() + "auth/login")
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setModal(true);

    resetError();

    manager_ = new QNetworkAccessManager(this);

    // TODO: remove this
    ui->emailLine_->setText("vitaliibondtest@gmail.com");
    ui->passLine_->setText("vitaliibondtest");
}

LoginDialog::~LoginDialog()
{
    delete ui;
    delete manager_;
}

void LoginDialog::setError(const QString &error)
{
    ui->errorText_->setHidden(false);
    ui->errorText_->setPlainText(error);
}

void LoginDialog::resetError()
{
    ui->errorText_->setHidden(true);
    ui->errorText_->setPlainText("");
}

bool LoginDialog::checkEmail(const QString &email)
{
    if (email.isEmpty())
    {
        setError("Email can't be empty");
        return false;
    }
    if (!email.contains('@') || !email.contains('.'))
    {
        setError("Incorrect email");
        return false;
    }
    return true;
}

bool LoginDialog::checkPassword(const QString &pass)
{
    if (pass.isEmpty())
    {
        setError("Password can't be empty");
        return false;
    }
    return true;
}

void LoginDialog::on_loginButton_clicked()
{
    resetError();
    auto email = ui->emailLine_->text();
    auto pass = ui->passLine_->text();

    if (!checkEmail(email) || !checkPassword(pass))
        return;

    QNetworkRequest request(url_);
    QJsonObject json;
    json["email"] = email;
    json["password"] = pass;
    QJsonDocument doc;
    doc.setObject(json);
    auto data = doc.toJson();
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request.setRawHeader("Client-Timezone-Offset", "60");
    request.setRawHeader("Client-Language", "ru-RU");
    request.setRawHeader("Client-Company", "udimiteam");
    request.setHeader(QNetworkRequest::UserAgentHeader, "desktop");

    auto reply = manager_->post(request, data);

    auto result = connect(reply, &QNetworkReply::finished, this, &LoginDialog::on_loginRequest_finished);
    assert(result);
}


void LoginDialog::on_loginRequest_finished()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        throw;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        setError((reply->errorString()));
        return;
    }
    auto data = reply->readAll();
    if (data.isEmpty()) {
        setError("Unknown Error");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(data);
    auto value = doc["token"];
    QString token = value.toString();
    if (token.isEmpty())
    {
        setError("Error: no token");
        return;
    }
    emit autorized(token);
    done(1);
}
