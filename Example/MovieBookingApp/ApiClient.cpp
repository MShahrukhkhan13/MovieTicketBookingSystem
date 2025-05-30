// ApiClient.cpp
#include "ApiClient.h"
#include <QUrl>
#include <QUrlQuery>

ApiClient::ApiClient(QObject* parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &ApiClient::onReplyFinished);
}

void ApiClient::fetchMovies()
{
    QNetworkRequest request(QUrl("http://localhost:8080/movies"));
    networkManager->get(request);
}

void ApiClient::fetchTheaters(int id)
{
    QString urlStr = QString("http://localhost:8080/theaters/%1").arg(id);
    QUrl url(urlStr);
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ApiClient::fetchSeats(int movieId, int theaterId) {
    QString urlStr = QString("http://localhost:8080/seats/%1/%2").arg(movieId).arg(theaterId);
    QUrl url(urlStr);
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ApiClient::bookSeat(int movieId, int theaterId, const QStringList &seats) {
    QString urlStr = QString("http://localhost:8080/book/%1/%2").arg(movieId).arg(theaterId);
    QNetworkRequest request(urlStr);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject rootObj;
    rootObj["sessionId"] = sessionId;
    QJsonArray seatArray;
    for (const auto& seat : seats) {
        seatArray.append(seat);
    }
    rootObj["seats"] = seatArray;

    QJsonDocument doc(rootObj);
    networkManager->post(request, doc.toJson());
}


void ApiClient::cancelSeat(int movieId, int theaterId, const QStringList &seats) {
    QString urlStr = QString("http://localhost:8080/cancel/%1/%2").arg(movieId).arg(theaterId);
    QNetworkRequest request(urlStr);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject rootObj;
    rootObj["sessionId"] = sessionId;
    QJsonArray seatArray;
    for (const auto& seat : seats) {
        seatArray.append(seat);
    }
    rootObj["seats"] = seatArray;

    QJsonDocument doc(rootObj);
    networkManager->post(request, doc.toJson());
}


void ApiClient::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error()) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    QUrl requestUrl = reply->request().url();
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QString path = requestUrl.path();

    if (path == "/movies") {
        if (jsonDoc.isObject()) {
            QVariantList movieList;
            auto obj = jsonDoc.object();
            if (obj.contains("movies") && obj["movies"].isArray()) {
                for (const QJsonValue &value : obj["movies"].toArray()) {
                    if (value.isObject())
                        movieList.append(value.toObject().toVariantMap());
                }
                emit moviesReceived(movieList);
            }

            if (obj.contains("sessionId")) {
                sessionId = obj["sessionId"].toString();
                // Optionally emit signal if you want to notify session updated
            }
        }
    }

    else if (path.startsWith("/theaters/")) {
        if (jsonDoc.isArray()) {
            QVariantList theaterList;
            for (const QJsonValue &value : jsonDoc.array()) {
                if (value.isObject())
                    theaterList.append(value.toObject().toVariantMap());
            }
            emit theatersReceived(theaterList);
        }
    }
    else if (path.startsWith("/seats/")) {
        if (jsonDoc.isObject()) {
            QVariantMap seatsMap = jsonDoc.object().toVariantMap();
            emit seatsReceived(seatsMap);
        }
    }
    else if (path.startsWith("/book/")) {
        if (jsonDoc.isObject()) {
            bool success = jsonDoc.object().value("success").toBool();
            QString bookingId = jsonDoc.object().value("bookingId").toString();

            if (success && !bookingId.isEmpty()) {
                emit bookingConfirmed(bookingId);
            } else {
                emit errorOccurred("Booking failed or bookingId missing.");
            }
        }
    }
    else if (path.startsWith("/cancel/")) {
        if (jsonDoc.isObject()) {
            bool success = jsonDoc.object().value("success").toBool();
            QString cancelId = jsonDoc.object().value("cancelId").toString();

            if (success && !cancelId.isEmpty()) {
                emit cancelConfirmed(cancelId);
            } else {
                emit errorOccurred("Cancel failed or cancelId missing.");
            }
        }
    }

    reply->deleteLater();
}
