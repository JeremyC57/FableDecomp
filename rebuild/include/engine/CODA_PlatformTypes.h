#ifndef FABLE_CODA_PLATFORM_TYPES_H
#define FABLE_CODA_PLATFORM_TYPES_H
#include "rebuild_abi.h"
// Original CODA_PlatformTypes.h names. The non-MSVC path preserves the width.
#ifdef _MSC_VER
typedef __int64 CREFTIME;
#else
typedef long long CREFTIME;
#endif
#endif
