include(../../config.qmake)
TEMPLATE = lib
RESOURCES = counters.qrc
CONFIG += plugin
QT += script
# TARGET = CGMEJoe
# DLLDESTDIR = $$DESTDIR
DESTDIR = $$QBOARD_PLUGINS_DESTDIR
QMAKE_CXXFLAGS = $$QBOARD_CXXFLAGS
TARGET = $$qtLibraryTarget(CGMEJoe)
HEADERS = \
  CGMEJoe.h

SOURCES = \
  CGMEJoe.cpp

# OBJECTS += $$S11N_OBJECTS
