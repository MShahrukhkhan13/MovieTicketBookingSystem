#ifndef APICLIENT_H
#define APICLIENT_H
#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class ApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ApiClient(QObject* parent = nullptr);

    Q_INVOKABLE void fetchMovies();
    Q_INVOKABLE void fetchTheaters(int id);
    Q_INVOKABLE void bookSeat(int movieId, int theaterId, const QStringList &seats);
    Q_INVOKABLE void fetchSeats(int movieId, int theaterId);
    Q_INVOKABLE void cancelSeat(int movieId, int theaterId, const QStringList &seats);

signals:
    void moviesReceived(QVariantList movies);
    void theatersReceived(QVariantList theaters);
    void bookingConfirmed(const QString &bookingId);
    void cancelConfirmed(const QString &cancelId);
    void seatsReceived(QVariantMap seatsMap);
    void errorOccurred(QString error);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;
      QString sessionId;
};

#endif
