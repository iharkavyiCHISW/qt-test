#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QUrl>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(const QUrl& url, QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void autorized(const QString& token);

private:
    void setError(const QString& error);
    void resetError();
    bool checkEmail(const QString& email);
    bool checkPassword(const QString& pass);
    bool getAuthToken(const std::string& rawReplyText);

private slots:
    void on_loginButton_clicked();
    void on_loginRequest_finished(QNetworkReply *reply);

private:
    Ui::LoginDialog *ui;

    QUrl url_;
    QString authToken_; // result of login
    QNetworkAccessManager* manager_;
    QByteArray data_; // used only for QNetworkAccessManager::post()
};

#endif // LOGINDIALOG_H
