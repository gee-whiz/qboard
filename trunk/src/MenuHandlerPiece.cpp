#include "MenuHandlerPiece.h"
#include "MenuHandlerGeneric.h"
#include "GamePiece.h"
#include "QGIGamePiece.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QIcon>
#include "utility.h"
#include "S11nQt.h"
#include "S11nQtList.h"
#include "S11nClipboard.h"


struct MenuHandlerPiece::Impl
{
	QGraphicsScene * scene; 
	Impl()
	{
		scene = 0;
	}
	~Impl()
	{
		
	}
};

MenuHandlerPiece::MenuHandlerPiece() 
	: impl(new MenuHandlerPiece::Impl)
{
	
}

MenuHandlerPiece::~MenuHandlerPiece()
{
	delete this->impl;
}


void MenuHandlerPiece::doMenu( QGIGamePiece * pv, QGraphicsSceneContextMenuEvent * ev )
{
	MenuHandlerCommon proxy;
	ev->accept();
	QMenu * m = proxy.createMenu( pv );
	PiecePropertyMenu * mColor = PiecePropertyMenu::makeColorMenu(pv, "color", "colorAlpha" );
	mColor->setIcon(QIcon(":/QBoard/icon/color_fill.png"));
	m->addMenu(mColor);
	if(1)
	{
		
		PiecePropertyMenu * pm = PiecePropertyMenu::makeIntListMenu("Rotate",pv,"angle",0,360,45);
		pm->setIcon(QIcon(":/QBoard/icon/rotate_cw.png"));
		m->addMenu(pm);
	}

	QMenu * mBrd = m->addMenu("Border");
	mBrd->addMenu( PiecePropertyMenu::makeColorMenu(pv,"borderColor","borderAlpha") );
	mBrd->addMenu( PiecePropertyMenu::makePenStyleMenu(pv,"borderStyle") );
	if(1)
	{
		mBrd->addMenu( PiecePropertyMenu::makeIntListMenu("Size",pv,"borderSize",0,8) );
	}
	m->exec( ev->screenPos() );
	delete m;
}



PiecePropertyAction::PiecePropertyAction( QGIGamePiece * view,
		QString const & propName,
		QVariant const & val,
		QObject * parent )
	: QAction(val.toString(),parent),
	m_gi(view),
	m_key(propName),
	m_val(val)
{
	if( val.type() == QVariant::Color )
	{
		QPixmap px(16,16);
		px.fill(val.value<QColor>());
		this->setIcon(QIcon(px));
	}
#if 1
	if( val == view->piece()->property(propName.toAscii()) )
	{
		this->setCheckable(true);
		this->setChecked(true);
	}
#endif
	connect( view->piece(), SIGNAL(destroyed()), this, SLOT(dtorDisconnect()) );
	connect( this, SIGNAL(triggered()), this, SLOT(setProperty()) );
}

void PiecePropertyAction::dtorDisconnect()
{
	this->m_gi = 0;
}

PiecePropertyAction::~PiecePropertyAction()
{
	
}

void PiecePropertyAction::setProperty()
{
	if( ! m_gi ) return;
	if( ! m_gi->isSelected() )
	{
		GamePiece * pc = m_gi->piece();
		if( ! pc ) return;
		pc->setPieceProperty(m_key.toAscii(), m_val);
		return;
	}
	typedef QList<QGraphicsItem *> QL;
	QL ql( this->m_gi->scene()->selectedItems() );
	QL::iterator it( ql.begin() );
	QL::iterator et( ql.end() );
	for( ; et != it; ++it )
	{
		if( (*it)->type() == QGITypes::GamePiece )
		{
			
			QGIGamePiece * pcv = dynamic_cast<QGIGamePiece*>(*it);
			if( pcv )
			{
				GamePiece * pc = pcv->piece();
				if( pc )
				{
					pc->setPieceProperty(this->m_key.toAscii(), this->m_val);
				}
			}
			continue;
		}
	}
}

PiecePropertyMenu::PiecePropertyMenu( const QString & title,
		QGIGamePiece * view,
		QString const & propName,
		QWidget * parent ) :
	QMenu(title,parent),
	m_view(view),
	m_key(propName)
{
}

QAction * PiecePropertyMenu::addItem( QVariant const & val, QString const & lbl)
{
	PiecePropertyAction * ac = new PiecePropertyAction( m_view, m_key, val, this );
	if( ! lbl.isEmpty() )
	{
		ac->setText(lbl);
	}
	this->addAction(ac);
	return ac;
}

PiecePropertyMenu::~PiecePropertyMenu()
{
}

PiecePropertyMenu * PiecePropertyMenu::makePenStyleMenu( QGIGamePiece * pv, char const * propertyName )
{
	PiecePropertyMenu * pm = new PiecePropertyMenu("Style", pv, propertyName, 0 );
	for( int i = Qt::NoPen; i < Qt::CustomDashLine; ++i )
	{
		pm->addItem( QVariant(i), qboard::penStyleToString(i) );
	}
	return pm;
}

PiecePropertyMenu * PiecePropertyMenu::makeIntListMenu(
	char const * lbl, 
	QGIGamePiece * pv,
	char const * propertyName,
	int from,
	int to,
	int step )
{
	PiecePropertyMenu * pm = new PiecePropertyMenu(lbl, pv, propertyName, 0 );
	for( int i = from; i < to; i += step )
	{
		pm->addItem( QVariant(i) );
	}
	return pm;
}

PiecePropertyMenu * PiecePropertyMenu::makeAlphaMenu(
	QGIGamePiece * pv,
	char const * propertyName,
	QColor const & hint )
{
	PiecePropertyMenu * pm = new PiecePropertyMenu("Transparency", pv, propertyName, 0 );
	const short opaque = 255;
	const qreal step = opaque/10.0;
	int pct = 0;
	for( qreal i = 0.0; i <= opaque; i += step, pct += 10)
	{
		QString lbl;
		if( i == 0 )
		{
			lbl = QString("Transparent");
		}
		else if( i==opaque )
		{
			i = opaque;
			lbl = QString("Opaque");
		}
		else
		{
			lbl = QString("%1%").arg(pct);
		}
		QAction * ac = pm->addItem( QVariant(i), lbl );
		if( hint.isValid() )
		{
			QColor c( hint );
			c.setAlpha(int(i));
			QPixmap px(16,16);
			px.fill(c);
			ac->setIcon(px);
		}
	}
	return pm;
}

PiecePropertyMenu * PiecePropertyMenu::makeColorMenu(
	QGIGamePiece * pv,
	char const * propertyName,
	char const * alphaName )
{
	PiecePropertyMenu * mc = new PiecePropertyMenu("Color",pv,propertyName,0);
	mc->setIcon(QIcon(":/QBoard/icon/colorize.png"));
	if( alphaName )
	{
		QColor hint( pv->piece()->property(propertyName).value<QColor>() );
		mc->addMenu( PiecePropertyMenu::makeAlphaMenu(pv, alphaName, hint) );
	}
	typedef QList<QColor> QL; 
	QL colors( qboard::colorList() ); 
	QL::iterator it = colors.begin();
	for( ; colors.end() != it; ++it )
	{
		QVariant var( *it );
		QString name( (*it).name() );
		mc->addItem( var, name  );
	}
	return mc;
}