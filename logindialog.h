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

    const QString& getAuthToken() const;

signals:
    void autorized(const QString& token);

private:
    void setError(const QString& error);
    void resetError();
    bool checkEmail(const QString& email);
    bool checkPassword(const QString& pass);

private slots:
    void on_loginButton_clicked();
    void on_loginRequest_finished();

private:
    Ui::LoginDialog *ui;

    QUrl url_;
    QString authToken_; // result of login
    QNetworkAccessManager* manager_;
};

#endif // LOGINDIALOG_H
