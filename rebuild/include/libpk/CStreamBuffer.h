#ifndef FABLE_LIBPK_CSTREAMBUFFER_H
#define FABLE_LIBPK_CSTREAMBUFFER_H
#include "rebuild_abi.h"
struct CSoundEventManager;
struct CSoundSample;
// Descriptive shared libpk type; see NAMING_EVIDENCE.md for the exhausted name search.
// Layout: native00C262A0 constructs44bytes;00C262E0 attaches sample+00,handle+04.
struct CStreamBuffer {
    CSoundSample* m_pSample;
    long m_handle;
    unsigned char OpaqueBufferState[0x24];
    void* GetPoolData(CSoundEventManager* manager);
};
FABLE_STATIC_ASSERT(sizeof(CStreamBuffer)==0x2c);
#endif
