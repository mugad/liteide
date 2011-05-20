/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: makefilefile.cpp
// Creator: visualfc <visualfc@gmail.com>
// date: 2011-3-26
// $Id: makefilefile.cpp,v 1.0 2011-5-12 visualfc Exp $

#include "makefilefile.h"
#include <QStandardItem>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "fileutil/fileutil.h"

//lite_memory_check_begin
#if defined(WIN32) && defined(_MSC_VER) &&  defined(_DEBUG)
     #define _CRTDBG_MAP_ALLOC
     #include <stdlib.h>
     #include <crtdbg.h>
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
#endif
//lite_memory_check_end

MakefileFile::MakefileFile(QObject *parent)
    : ModelFileImpl(parent)
{
}

bool MakefileFile::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return false;
    }
    QMap<QString,QStringList> context = FileUtil::readFileContext(&file);
    file.close();
    if (context.isEmpty()) {
        return false;
    }
    m_context = context;
    if (m_context.contains("GOFILES") ||
            m_context.contains("CGOFILES")) {
        m_mimeType = "text/x-gomake";
    }
    return true;
}

void MakefileFile::updateModel()
{
    m_model->clear();
    m_fileList.clear();

    QStandardItem *item = new QStandardItem(QFileInfo(m_fileName).fileName());
    item->setData(ItemProFile);
    item->setIcon(QIcon(":/images/projectitem.png"));
    m_model->appendRow(item);
    m_fileList.append(m_fileName);

    QMap<QString,QString> fileMap;
    fileMap.insert("GOFILES",tr("GOFILES"));
    fileMap.insert("CGOFILES",tr("CGOFILES"));
    fileMap.insert("HEADERS",tr("Headers"));
    fileMap.insert("SOURCES",tr("Sources"));

    QMapIterator<QString,QString> i(fileMap);
    while(i.hasNext()) {
        i.next();
        QStringList files = this->values(i.key());
        if (!files.isEmpty()) {
            QStandardItem *folder = new QStandardItem(i.value());
            folder->setData(ItemFolder);
            folder->setIcon(QIcon(":/images/folderitem.png"));
            m_model->appendRow(folder);
            foreach(QString file, files) {
                QStandardItem *fileItem = new QStandardItem(file);
                fileItem->setData(ItemFile);
                fileItem->setIcon(QIcon(":/images/fileitem.png"));
                folder->appendRow(fileItem);
                m_fileList.append(fileNameToFullPath(file));
            }
        }
    }
}

QString MakefileFile::target() const
{
    QString target = QFileInfo(m_fileName).fileName();
    QString val = value("TARG");
    if (!val.isEmpty()) {
        target = val;
    }
    QString workPath = QFileInfo(m_fileName).absolutePath();
    if (!workPath.isEmpty()) {
        target = QFileInfo(QDir(workPath),target).absoluteFilePath();
    }
    return target;
}
