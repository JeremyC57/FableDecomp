#ifndef FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#define FABLE_LIBPK_CSOUNDEVENTMANAGER_H
#include "libpk/CHandleBasedMemoryPool_012b4098.h"
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
#include "libpk/CFader.h"
#endif
#include "rebuild_abi.h"
#include <stddef.h>
// Descriptive shared libpk type; existing00C26090 model and native00C262E0.
// Layout: native00C26260/00C262E0 address the embedded pool at+360.
struct CSoundEventManager {
    unsigned char OpaqueManagerPrefix[0x360];
    CHandleBasedMemoryPool_012b4098 m_pool;
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
    // Opt-in (landing gm, row 0x00C11700): the fader at +0x7D8 (constructed by 0x00C11D00 with CFader::CFader 0x00C26010,
    //   destroyed by 0x00C13470) and its one forwarder. Descriptive names (libpk has no PDB or RTTI).
    unsigned char m_7b0[0x7d8 - 0x7b0];         // +0x7B0 (the two worker parts +0x7B4 / +0x7B8, resources +0x7BC, ...)
    CFader m_fader;                             // +0x7D8
    float GetFaderOutput() const;               // 0x00C11700
#endif
};
FABLE_STATIC_ASSERT_AT(12, offsetof(CSoundEventManager,m_pool)==0x360);
#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
FABLE_STATIC_ASSERT(offsetof(CSoundEventManager,m_7b0)==0x7b0);
FABLE_STATIC_ASSERT(offsetof(CSoundEventManager,m_fader)==0x7d8);
#endif
#endif
