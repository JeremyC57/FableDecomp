#ifndef FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#define FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#include "libpk/CHandleBasedMemoryPool_012b4098.h"
#include "rebuild_abi.h"
#include <stddef.h>
// Descriptive shared libpk type; existing00C26090 model and native00C262E0.
// Layout: native00C26260/00C262E0 address the embedded pool at+360.
struct CSoundEventManager {
    unsigned char OpaqueManagerPrefix[0x360];
    CHandleBasedMemoryPool_012b4098 m_pool;
};
FABLE_STATIC_ASSERT(offsetof(CSoundEventManager,m_pool)==0x360);
#endif
