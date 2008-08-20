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

#include "PieceAppearanceWidget.h"

#include <s11n.net/s11n/s11nlite.hpp>
#include <QDebug>
#include <QGridLayout>
#include <QGraphicsView>
#include "S11nQt.h"
#include "QBoardView.h"
#include "QGIPiece.h"

struct PieceAppearanceWidget::Impl
{
    GameState gs;
    QGraphicsView * gv;
    QGIPiece * pc;
    Impl() : gs(), gv(0),pc(0)
    {
    }
    ~Impl()
    {
    }
};

void PieceAppearanceWidget::setupDefaultTemplates()
{
    typedef QList<QColor> QCL;
    QCL cl;
    cl << QColor(255,0,0)
       << QColor(0,255,0)
       << QColor(0,0,255)
       << QColor(255,255,0)
       << QColor(255,255,255)

       << QColor(255,127,127)
       << QColor(127,255,127)
       << QColor(127,127,255)
       << QColor(127,127,0)
       << QColor(127,127,127)

       << QColor(127,0,0)
       << QColor(0,127,0)
       << QColor(0,0,127)
       << QColor(64,64,0)
       << QColor(0,0,0,0)

	;
    int step = 24;
    int space = 2;
    int x = space;
    int y = space;
    int count = cl.size();
    int pos = 0;
    int rows = 3;
    for( QCL::iterator it = cl.begin();
	 cl.end() != it; ++it )
    {
	QGIPiece * pc = new QGIPiece;
	if( ! this->impl->pc ) this->impl->pc = pc;
	this->impl->gs.addItem( pc );
	pc->setProperty("size",QSize(step,step));
	pc->setProperty("pos",QPoint(x,y));
	pc->setProperty("dragDisabled",int(1));
	x += step + space;
	if( ++pos >= (count/rows) )
	{
	    y += step + space;
	    pos = 0;
	    x = space;
	}
	pc->setProperty("color",*it);
	pc->setProperty("borderSize",1);
	pc->setProperty("borderColor",QColor(0,0,0));
    }

}
void PieceAppearanceWidget::setupUI()
{
    QLayout * lay = new QGridLayout( this );
    lay->setSpacing(0);
    lay->setContentsMargins(0,0,0,0);
    QGraphicsView * v = this->impl->gv =
	//new QGraphicsView( impl->gs.scene() )
	new QBoardView( impl->gs )
	;

    v->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    v->setInteractive(true); // required to get mouse events to the children
    v->setTransformationAnchor(QGraphicsView::NoAnchor);
    //v->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    v->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    v->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    v->setDragMode(QGraphicsView::NoDrag);
    v->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    v->setBackgroundBrush(QColor("#abb8fb"));
    v->viewport()->setObjectName( "PieceAppearanceWidget");

    lay->addWidget( v );

    QStringList tip;
    tip << "<html><body>These widgets are appearance templates for game pieces.\n"
	<< "Edit them by right-clicking them.\n"
	<< "Select a template by left-clicking it. "
	<< "QBoard will use the selected template as the default look\n"
	<< "for newly-added pieces which are loaded from image files.\n"
	<< "This list will be saved automatically when QBoard exits,\n"
	<< "and loaded when QBoard starts up.</body></html>"
	;
    impl->gv->setToolTip( tip.join("") );

}
PieceAppearanceWidget::PieceAppearanceWidget(QWidget * parent, Qt::WindowFlags flags) 
    : QWidget(parent,flags), Serializable("PieceAppearanceWidget"),
      impl(new Impl)
{
    this->setupUI();
}

PieceAppearanceWidget::~PieceAppearanceWidget()
{
    //qDebug() << "~PieceAppearanceWidget()";
    delete impl;
}

QGraphicsView * PieceAppearanceWidget::view()
{
    return this->impl->gv;
}
#include <QDynamicPropertyChangeEvent>
bool PieceAppearanceWidget::event( QEvent * e )
{
	return QObject::event(e);
}

GameState & PieceAppearanceWidget::state()
{
    return this->impl->gs;
}
GameState const & PieceAppearanceWidget::state() const
{
    return this->impl->gs;
}


bool PieceAppearanceWidget::serialize( S11nNode & dest ) const
{
    if( ! this->Serializable::serialize( dest ) ) return false;
    //typedef S11nNodeTraits NT;
    return s11n::serialize_subnode( dest, "gamestate", this->impl->gs );
}

bool PieceAppearanceWidget::deserialize(  S11nNode const & src )
{
    if( ! this->Serializable::deserialize( src ) ) return false;
    //typedef S11nNodeTraits NT;
    return s11n::deserialize_subnode( src, "gamestate", this->impl->gs );
}
void PieceAppearanceWidget::clear()
{
    this->impl->gs.clear();
}

void PieceAppearanceWidget::applyCurrentTemplate( QObject * tgt )
{
    if( !tgt ) return;
    typedef QList<QGraphicsItem*> QIL;
    QIL li( impl->gv->scene()->selectedItems() );
    impl->pc = 0;
    // If there are multiple items selected, simply pick the first
    // selected item which has an associated piece (the order has no
    // meaning, however).
    for( QIL::iterator it = li.begin();
	 li.end() != it;
	 ++it )
    {
	QGIPiece * pvi = dynamic_cast<QGIPiece*>(*it);
	if( pvi )
	{
	    impl->pc = pvi;
	}
    }
    if( ! impl->pc ) return;

    /**
       We want to keep certain properties intact:
    
       pixmap: b/c the game client normally sets this

       pos: we don't want to use the template's pos

       dragDisabled: that property was developed to support
       this class. The client will almost always expect
       his pieces to be draggable.
    */
    QVariant pix( tgt->property("pixmap") );
    QVariant pos( tgt->property("pos") );
    QVariant dragDisabled( tgt->property("dragDisabled") );
    {
	QObject * src = impl->pc;
	typedef QList<QByteArray> QL;
	QL ql( src->dynamicPropertyNames() );
	QL::const_iterator it( ql.begin() );
	QL::const_iterator et( ql.end() );
	for( ; et != it; ++it )
	{
	    char const * key = it->constData();
	    if( !key || (*key == '_') ) continue; // Qt reserves the "_q_" prefix, so we'll elaborate on that.
	    tgt->setProperty( key, src->property(key) );
	}
    }
    tgt->setProperty("pos", pos );
    tgt->setProperty("pixmap", pix );
    tgt->setProperty("dragDisabled", dragDisabled );
}
