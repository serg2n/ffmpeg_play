#ifndef FFREADER_GLOBAL_H
#define FFREADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FFREADER_LIBRARY)
#  define FFREADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FFREADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FFREADER_GLOBAL_H
