/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2014  Patrick von Reth <vonreth@kde.org>

    SnoreNotify is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreNotify is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreNotify.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SNORECOREPRIVATE_H
#define SNORECOREPRIVATE_H

#include "snore.h"
#include "version.h"
#include "plugins/snorebackend.h"

#include <QPointer>
#include <QApplication>
#include <QCryptographicHash>

namespace Snore
{
class SNORE_EXPORT SnoreCorePrivate : public QObject
{
    Q_DECLARE_PUBLIC(SnoreCore)
    Q_OBJECT

public:
    static inline QString computeHash(const QByteArray &data)
    {
        return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
    }

    static void registerMetaTypes();

    /**
     * @brief tempPath
     * @return the path of a QTemporaryDir, this location is used to store images.
     */
    static QString tempPath();

public:
    static SnoreCorePrivate *instance();
    ~SnoreCorePrivate();
    const Application defaultApplication() const;

    void notificationActionInvoked(Notification notification) const;

    bool setBackendIfAvailible(const QString &backend);

    /**
     *
     * @return whether the backend can update a notification
     */
    bool primaryBackendCanUpdateNotification() const;

    bool initPrimaryNotificationBackend();

    inline QString versionSchema() const
    {
        return "v1";
    }

    void syncSettings();

    QString normalizeKey(const QString &key, SettingsType type) const
    {
        if (type == LOCAL_SETTING) {
            return QString("%1/LocalSettings/%2/%3").arg(versionSchema(),m_localSettingsPrefix, key);
        } else {
            return QString("%1/%2").arg(versionSchema(), key);
        }
    }

    //TODO: rename
    QStringList knownClients();

    void setLocalSttingsPrefix(const QString &prefix);

signals:
    void applicationRegistered(const Snore::Application &);
    void applicationDeregistered(const Snore::Application &);
    void notify(Snore::Notification noti);

private slots:
    void slotNotificationClosed(Snore::Notification);
    void slotAboutToQuit();

private:
    SnoreCorePrivate();
    SnoreCore *q_ptr;

    QHash<QString, Application> m_applications;

    QHash<SnorePlugin::PluginTypes, QStringList> m_pluginNames;
    QHash<QString, SnorePlugin *> m_plugins;

    QPointer<SnoreBackend> m_notificationBackend;

    Application m_defaultApp;

    QString m_localSettingsPrefix = QString("%1.%2").arg(qApp->organizationName(), qApp->applicationName());

    QSettings *m_settings;
};
}

#endif // SNORECOREPRIVATE_H
