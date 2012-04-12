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
// Module: golangast.h
// Creator: visualfc <visualfc@gmail.com>
// date: 2011-3-26
// $Id: golangast.h,v 1.0 2011-5-12 visualfc Exp $

#ifndef GOLANGAST_H
#define GOLANGAST_H

#include "liteapi/liteapi.h"
#include "golangastapi/golangastapi.h"
#include "symboltreeview/symboltreeview.h"
#include <QProcess>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class QStackedWidget;
class AstWidget;
class GolangAst : public LiteApi::IGolangAst
{
    Q_OBJECT
public:
    explicit GolangAst(LiteApi::IApplication *app, QObject *parent = 0);
    ~GolangAst();
    virtual QIcon iconFromTag(const QString &tag, bool pub) const;
    virtual QIcon iconFromTagEnum(LiteApi::ASTTAG_ENUM tag, bool pub) const;
public:
    void setEnable(bool b);
    void updateModel(const QByteArray &data);
    QWidget *widget();
    void loadProject(LiteApi::IProject *project);
public slots:
    void projectReloaded();
    void projectChanged(LiteApi::IProject*);
    void editorCreated(LiteApi::IEditor*);
    void editorAboutToClose(LiteApi::IEditor *editor);
    void editorChanged(LiteApi::IEditor*);
    void editorSaved(LiteApi::IEditor*);
    void finishedProcess(int,QProcess::ExitStatus);
    void updateAst();
    void updateAstNow();
    void doubleClickedTree(QModelIndex);
protected:
    LiteApi::IApplication *m_liteApp;
    QWidget *m_widget;
    QTimer  *m_timer;
    QProcess *m_process;
    QStringList m_updateFileNames;
    QStringList m_updateFilePaths;
    QStringList m_processFiles;
    QString m_workPath;
    QStackedWidget *m_stackedWidget;
    AstWidget *m_projectAstWidget;
    LiteApi::IEditor *m_currentEditor;
    QMap<LiteApi::IEditor*,AstWidget*> m_editorAstWidgetMap;
};

#endif // GOLANGAST_H
