include(../../config.qmake)
TEMPLATE = app
QT += script svg
QMAKE_CXXFLAGS = $$QBOARD_CXXFLAGS

HEADERS = \
 $$QBOARD_HEADERS_LIB

SOURCES = \
 $$QBOARD_SOURCES_LIB \
 S11nQtTests.cpp

OBJECTS += $$S11N_OBJECTS
