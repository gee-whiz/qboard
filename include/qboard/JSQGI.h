#ifndef QBOARD_JSQGI_H_INCLUDED
#define QBOARD_JSQGI_H_INCLUDED 1
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


#include <QObject>
#include <QPointF>
#include <QScriptable>
#include <QScriptValue>
class QGraphicsItem;

namespace qboard {
class JSQGI : public QObject,
	      public QScriptable
{
Q_OBJECT
public:
    JSQGI( QObject * parent = 0 );
    virtual ~JSQGI();

    /**
       Returns all children of this object which inherit both
       QGraphicsItem and QObject.
    */
    Q_INVOKABLE QList<QGraphicsItem*> childItems();
    /**
       Moves this object to the given coords.
    */
    Q_INVOKABLE void setPos( qreal x, qreal y );
    /**
       Moves this object to the given coords.
    */
    Q_INVOKABLE void setPos( QPointF const & p );
    /**
       Moves the item by the given relative coordinates.
    */
    Q_INVOKABLE void moveBy( qreal x, qreal y );
    /**
       Moves the item by the given relative coordinates.
    */
    Q_INVOKABLE void moveBy( QPointF const & p );

    /**
       Returns the object's current X position, relative to its parent
       (if it has one) or its scene.
    */
    Q_INVOKABLE qreal posX();
    /**
       Returns the object's current Y position, relative to its parent
       (if it has one) or its scene.
    */
    Q_INVOKABLE qreal posY();


    /**
       Damn! If this function is called pos() it gets hidden by the pos
       property of the various game items.

       So it was renamed.

       Returns the current position as a JS object {x:N,y:N}

       This value can, in turn, be passed to the QPoint or QPointF
       constructor in JS code, e.g.:

       \code
       var pos = someItem.posXY();
       pos.x = pos.x + 20;
       someItem.prop('pos',QPoint(pos));
       \endcode
    */
    Q_INVOKABLE QScriptValue posXY();

    /**
       Returns the object's current z-level value.
    */
    Q_INVOKABLE qreal zValue();

    /**
       Sets the object's current z-level value.
    */
    void setZValue( qreal );

    /**
       Returns the value of the given property.
    */
    Q_INVOKABLE QScriptValue prop(QString const & key);

    /**
       Returns the given property to the given value.
    */
    Q_INVOKABLE bool prop(QString const & key, QScriptValue const & val );

    /**
       Sets this object's parent item, transfering ownership of this
       object to that item.

       Returns false only if this object is not properly bound to
       an underlying QGraphicsItem.
    */
    //Q_INVOKABLE bool setParentItem( QGraphicsItem * p );
    Q_INVOKABLE bool setParentItem( QScriptValue const & p );

    /**
       Returns this object's parent item, or 0 if it is top-level or
       has not yet been added to a scene or parent.
    */
    Q_INVOKABLE QGraphicsItem * parentItem();

Q_SIGNALS:
    void doubleClicked(QGraphicsItem*);

private:
    QGraphicsItem * self();
    struct Impl;
    Impl * impl;
};

} // namespace
#endif // QBOARD_JSQGI_H_INCLUDED
