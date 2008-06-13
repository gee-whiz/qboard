#include "FileBrowser.h"
#include <QDir>
#include <QFile>
#include <QSizePolicy>
#include <QDebug>
#include <QFileIconProvider>
#include <QListWidgetItem>
#include <QWidget>

// If FileBrowser_USE_DIRICON is a string then: if a dir entry has a file named that
// string (e.g. ".diricon.png") then that icon is used in place of the default one.
// To turn this of, undefine FileBrowser_USE_DIRICON 
#define FileBrowser_USE_DIRICON ".diricon.png"
#ifdef FileBrowser_USE_DIRICON
#include <QRegExp>
#endif

// If FileBrowser_USE_QBOARD is true then this code uses qboard-specific
// functions to try to use qboard-home-relative paths.
#define FileBrowser_USE_QBOARD 1
#if FileBrowser_USE_QBOARD
#include "utility.h"
#endif

// If FileBrowser_GENERATE_MINIICONS is true then code is enabled which tries to
// generate preview icons for "small" image files. Whe a list entry has the
// suffix 'png' or 'xpm' and that file is under a certain size (e.g. 8k) then
// it is loaded as a pixmap, scaled to 16x16, and used as its own icon.
#define FileBrowser_GENERATE_MINIICONS 1
#if FileBrowser_GENERATE_MINIICONS
#include <QPixmapCache>
#include <QRegExp>
#endif

class FBFileIconProvider : public QFileIconProvider
{
public:
    FBFileIconProvider() : QFileIconProvider()
    {
    }
    virtual ~FBFileIconProvider(){}
    virtual QIcon icon( const QFileInfo & info ) const
    {
#ifdef FileBrowser_USE_DIRICON
	if( info.isDir() )
	{
	    //QString check( QDir::toNativeSeparators(info.canonicalFilePath()+"/dir
	    QDir dir(info.canonicalFilePath());
	    if( dir.exists(FileBrowser_USE_DIRICON) )
	    {
		return QIcon(QDir::toNativeSeparators(dir.filePath(FileBrowser_USE_DIRICON)));
	    }
	}
#endif // FileBrowser_USE_DIRICON
#if FileBrowser_GENERATE_MINIICONS
	static const int thresh = 8 * 1024;
	if( (info.size()<thresh)
	    && (0 == QRegExp("(png|xpm)",Qt::CaseInsensitive).indexIn(info.suffix()) )
	    )
	{
	    QString key( info.canonicalFilePath() );
	    QPixmap pix;
	    if( ! QPixmapCache::find(key,pix) )
	    {
		if( pix.load( key ) )
		{
		    pix = pix.scaled(16,16);
		    QPixmapCache::insert(key,pix);
		}
	    }
	    if( ! pix.isNull() )
	    {
		return QIcon(pix);
	    }
	}
#endif // FileBrowser_GENERATE_MINIICONS
	return this->QFileIconProvider::icon(info);
    }
};

FileBrowser::FileBrowser (const QString & filter, QWidget * parent):
    QListWidget (parent),
    basePath(),
    iconer( new FBFileIconProvider ),
    showHidden(false)
{

    nameFilter = filter;
    this->setDir(QDir::currentPath());
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(somethingSelected(QListWidgetItem*)));
}

FileBrowser::~FileBrowser()
{
    delete this->iconer;
}

void
FileBrowser::setDir (const QString & path)
{
    //qDebug() << "setDir("<<path<<")\n";
#if FileBrowser_USE_QBOARD
    QString qbhome( qboard::home().canonicalPath() );
#endif
    QDir dir(path, nameFilter, QDir::DirsFirst);
    if (!dir.isReadable ())	return;
    dir.setFilter( QDir::AllDirs | QDir::Files );
    clear();
    QStringList entries = dir.entryList();
    QStringList::ConstIterator it = entries.constBegin();
    QString fn;
    for( ; it != entries.constEnd(); ++it)
    {
	fn = *it;
	if (fn == ".") continue;
	if( (fn == "..") )
	{
#if FileBrowser_USE_QBOARD
	    if( QDir(path).canonicalPath() == qbhome ) continue;
#endif 
	    if( path == "/" ) continue;
	}
	QFileInfo fi(dir.absoluteFilePath(fn));
	if( showHidden && fi.isHidden() ) continue;
	this->addItem(new QListWidgetItem( iconer->icon(fi), fn ) );
    }
    basePath = dir.canonicalPath();
}

void FileBrowser::reloadDir()
{
    this->blockSignals(true);
    this->setDir( this->basePath );
    this->blockSignals(false);
}

void FileBrowser::somethingSelected( QListWidgetItem * item)
{
    QString txt( item->text() );
    if( txt.isEmpty() ) return;
    QString path = QDir::toNativeSeparators(basePath + "/" +txt);
#if FileBrowser_USE_QBOARD
    path = qboard::homeRelative(path);
#endif
    QFileInfo fi(path);
    if(fi.isDir())
    {
    	path = fi.canonicalFilePath();
	this->setDir(path);
	emit pickedDir(QDir(path));
    }
    else if (fi.isFile())
    {
    	emit pickedFile(fi);	
    }

}
    
#undef FileBrowser_USE_QBOARD
#undef FileBrowser_USE_DIRICON