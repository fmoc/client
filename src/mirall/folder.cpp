#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QIcon>
#include <QMutexLocker>
#include <QUrl>

#include "mirall/constants.h"
#include "mirall/folder.h"
#include "mirall/folderwatcher.h"

namespace Mirall {

Folder::Folder(const QString &path, QObject *parent)
    : QObject(parent),
      _path(path)
{
    _openAction = new QAction(QIcon(FOLDER_ICON), path, this);
    _openAction->setIconVisibleInMenu(true);
    _openAction->setIcon(QIcon(FOLDER_ICON));

    QObject::connect(_openAction, SIGNAL(triggered(bool)), SLOT(slotOpenFolder()));

    _watcher = new Mirall::FolderWatcher(path, this);
    QObject::connect(_watcher, SIGNAL(folderChanged(const QString &)),
                     SLOT(slotChanged(const QString &)));

    QObject::connect(this, SIGNAL(syncStarted()), SLOT(slotSyncStarted()));
    QObject::connect(this, SIGNAL(syncFinished()), SLOT(slotSyncFinished()));
}

QAction * Folder::openAction() const
{
    return _openAction;
}

Folder::~Folder()
{
}

QString Folder::path() const
{
    return _path;
}

void Folder::slotChanged(const QString &path)
{
    //qDebug() << "path " << path << " changed";
}

void Folder::slotOpenFolder()
{
    QDesktopServices::openUrl(QUrl(_path));
}

void Folder::slotSyncStarted()
{
    _openAction->setIcon(QIcon(FOLDER_SYNC_ICON));
}

void Folder::slotSyncFinished()
{
    _openAction->setIcon(QIcon(FOLDER_ICON));
}

} // namespace Mirall

#include "folder.moc"
