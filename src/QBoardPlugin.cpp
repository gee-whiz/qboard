/*
 * This file is (or was, at some point) part of the QBoard project
 * (http://code.google.com/p/qboard)
 *
 * Copyright (c) 2008 Stephan Beal (http://wanderinghorse.net/home/stephan/)
 *
 * This file may be used under the terms of the GNU General Public
 * License versions 2.0 or 3.0 as published by the Free Software
 * Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
 * included in the packaging of this file.
 *
 */

#include <QtGui>
#include <QDebug>
#include <qboard/QBoardPlugin.h>
#include <qboard/GameState.h>


struct QBoardBasePlugin::Impl
{
    GameState * gs;
    Impl() :
	gs(0)
    {
    }
    ~Impl()
    {
    }
};

QBoardBasePlugin::QBoardBasePlugin() :
    QObject(),
    QBoardPlugin(),
    impl(new Impl)
{
    qDebug() << "QBoardBasePlugin()";
}

QBoardBasePlugin::~QBoardBasePlugin()
{
    qDebug() << "~QBoardBasePlugin()";
    delete impl;
}

QWidget * QBoardBasePlugin::widget()
{
    return 0;
}

void QBoardBasePlugin::setGameState(GameState &s)
{
    impl->gs = &s;
}

GameState * QBoardBasePlugin::gameState()
{
    return impl->gs;
}

Q_EXPORT_PLUGIN2(QBoardBasePlugin, QBoardBasePlugin);
