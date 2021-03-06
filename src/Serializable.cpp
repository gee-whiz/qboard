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

#include <qboard/Serializable.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <s11n.net/s11n/s11nlite.hpp>
#include <s11n.net/s11n/functional.hpp>
#include <QRegExp>

struct Serializable::Impl
{
    char const * cname;
    // File extension. i'd rather have (char const *),
    // but we need to be able to prepend a '.' to it.
    std::string ext;
    Impl() :
	cname(0),
	ext()
    {
    }
    ~Impl()
    {
    }
};

Serializable::Serializable( char const * cn ) :
    impl(new Impl)
{
    impl->cname = cn ? cn : "Serializable";
    impl->ext = cn ? (std::string(".")+cn) : std::string();

}

Serializable::~Serializable()
{
    delete this->impl;
    // ^^^ this means s11nClassName() and co. will crash if
    // this object is used as part of a QObject::destroyed()
    // handler.
}

Serializable & Serializable::operator=( Serializable const & )
{
    //return this->copy( rhs );
    return *this;
}
Serializable::Serializable( Serializable const & )
{
    //this->copy(rhs);
}


Serializable * Serializable::clone() const
{
    S11nNode node;
    return this->serialize( node )
	? s11nlite::deserialize<Serializable>( node )
	: 0;
}

Serializable & Serializable::copy( Serializable const & rhs )
{
    if( &rhs != this )
    {
	S11nNode node;
	if( ! rhs.serialize( node ) )
	{
	    throw s11n::s11n_exception("Serializable[%s]::copy(Serializable[%s]) serialize failed",
				       this->s11nClass(), rhs.s11nClass());
	}
	if( ! this->deserialize( node ) )
	{
	    throw s11n::s11n_exception("Serializable[%s]::copy(Serializable[%s]) deserialize failed",
				       this->s11nClass(), rhs.s11nClass());
	}
    }
    return *this;
}

bool Serializable::serialize( S11nNode & dest ) const
{
    typedef S11nNodeTraits NT;
    NT::class_name( dest, this->s11nClass() );
    return true;
}
bool Serializable::deserialize(  S11nNode const & src )
{
    typedef S11nNodeTraits NT;
    std::string cname( NT::class_name( src ) );
    if( cname != std::string(this->s11nClass()) )
    {
	throw s11n::s11n_exception("Serializable[%s] was asked to deserialize data for type [%s].",
				   this->s11nClass(), cname.c_str() );
    }
    return true;
}

char const * Serializable::s11nClass() const
{
    return impl->cname;
}
void Serializable::s11nClass( char const * cn )
{
    if( (! cn) || (!*cn) ) return; 
    impl->cname = cn;
}

char const * Serializable::s11nFileExtension()
{
    return impl->ext.c_str();
}
void Serializable::s11nFileExtension( char const * ext )
{
    impl->ext = ext ? ext : "";
}

bool Serializable::s11nSave( std::ostream & os) const
{
    return s11nlite::save<Serializable>( *this, os );
}
bool Serializable::s11nLoad( std::istream & is )
{
    typedef std::auto_ptr<S11nNode> NP;
    NP np( s11nlite::load_node( is ) );
    return np.get()
	? this->deserialize( *np )
	: false;
}

bool Serializable::s11nSave( QString const & src, bool autoAddFileExtension ) const
{
    QString rn( src );
    if( autoAddFileExtension && ! impl->ext.empty() )
    {
	QString extpat( impl->ext.c_str() );
	extpat.append( "$" );
	if( QRegExp(extpat).indexIn(src) < 0 )
	{
	    rn = src + impl->ext.c_str();
	}
    }
    return s11nlite::save<Serializable>( *this, rn.toAscii().constData() );
}

bool Serializable::s11nLoad( QString const & fn)
{
    if( fn.isEmpty() ) return false;
    typedef std::auto_ptr<S11nNode> NP;
    NP np( s11nlite::load_node( fn.toAscii().constData() ) );
    return np.get()
	? this->deserialize( *np )
	: false;
}

bool Serializable::fileNameMatches( QString const & fn ) const
{
    if( impl->ext.empty() ) return false;
    char const * ext = impl->ext.c_str();
    if( ! ext ) return false;
    QString pat(ext);
    pat.replace( ".", "\\." );
    pat.append("$");
    return (QRegExp(pat, Qt::CaseInsensitive).indexIn(fn) >= 0);
}

bool Serializable_s11n::operator()( S11nNode & dest, Serializable const & src ) const
{
    return src.serialize( dest );
}
bool Serializable_s11n::operator()( S11nNode const & src, Serializable & dest ) const
{
    return dest.deserialize( src );
}

