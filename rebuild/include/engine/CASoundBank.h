#ifndef FABLE_ENGINE_CASOUNDBANK_H
#define FABLE_ENGINE_CASOUNDBANK_H
#include "engine/CBaseClassNonCopyable.h"
class CWideString;
// Audio module has no PDB method names. Native bank vtable0129c9ac has destructor and abstract slots1..4;
// InitSound calls slot3 with the source .lug path. Method names below are explicitly descriptive.
class CASoundBank : public CBaseClassNonCopyable {
public:
    virtual ~CASoundBank();
    // CSoundBankLH slot1 00a0b630 opens the compiled file through its backend (ret4).
    virtual void LoadCompiledBank(const CWideString&)=0;
    // Slot2 00a0b6c0 closes that backend handle and clears it.
    virtual void UnloadCompiledBank()=0;
    virtual void SetSourceLugPath(const CWideString&)=0;
private:
    // Native constructor00a0bd00 zeros both owned-handle words; native dtor00a0bd40 releases them.
    unsigned char ResourceHandle[8];
};
FABLE_STATIC_ASSERT(sizeof(CASoundBank)==12);
#endif
