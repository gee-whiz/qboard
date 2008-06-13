#ifndef __QGIGAMEPIECE_H__
#define __QGIGAMEPIECE_H__

class GamePiece;
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QVariant>
#include "QGI.h"
/**
	A QGIGamePiece represents the main GUI component of a GamePiece.
	
	Pieces and views They communicate with each other view
	GamePiece::piecePropertySet() and GamePiece::setPieceProperty().

	The idea is that pieces and views have a 1:N relationship, but
	for consistency with how other QGraphicsItems are destroyed,
	this type will destroy its attached game piece when it 
	destructs. If a viewed piece is destroyed, the watching QGIGamePiece will
	schedule its own destruction using QObject::deleteLater(). 
*/
class QGIGamePiece : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
private:
	QGIGamePiece( QGIGamePiece const & ); // not implemented
	QGIGamePiece & operator=(QGIGamePiece const &); // not implemented
public:
	/**
		Points this view at pc. If sc is not null then this
		object is added to sc, transfering ownership of this
		object to sc.
	*/
	QGIGamePiece( GamePiece * pc, QGraphicsScene * sc );
	virtual ~QGIGamePiece();
	/**
		Points this object at pc.
		
		BROKEN/FIXME: this routine does not properly handle the case
		that it is passed multiple pieces over time. The ownership
		of pieces and views is annoyingly tricky, and this function
		does not take that into account at all. 
	*/
	void setPiece( GamePiece * pc );
	/**
		Returns this object's piece (may be 0). Does not change
		ownership of the piece.
	*/
	GamePiece * piece();
	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	virtual QRectF boundingRect () const;
	/** Updates the piece's position to this object's position. */
	void updatePiecePos( bool onlyIfNotSet );
	virtual int type() const { return QGITypes::GamePiece; }
	virtual bool event( QEvent * e );
public Q_SLOTS:
	/** Uses QObject::deleteLater() to delete this object and the underlying GamePiece. */
	void destroyWithPiece();
protected:
	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent * event );
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

private Q_SLOTS:
	/** Catches GamePiece::::piecePropertySet(). */
	void piecePropertySet( char const * );
	/** Disconnects pc from internal handlers. */	
	void disconnect( GamePiece * pc );
	/** Connects pc to internal handlers. */
	void connect( GamePiece * pc );
	/** Pulls all recognized properties from the current piece. */
	void syncProperties();
	/** Catches GamePiece::destructing() and schedules THIS object for deletion
	via deleteLater(); */
	void pieceDestroyed( GamePiece * pc );
private:	
	GamePiece * m_pc;
	struct Impl;
	Impl * impl;
};

#endif // __QGIGAMEPIECE_H__