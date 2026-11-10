#ifndef FABLE_ENGINE_CASOUNDBANK_H
#define FABLE_ENGINE_CASOUNDBANK_H
#include "engine/CBaseClassNonCopyable.h"
class CWideString;
#ifdef FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
class CCharString;
#endif
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
#ifdef FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
    // Opt-in (agentSND, 2026-11-10, rows 0x005F64DD / 0x005F6793): FableWin.pdb ?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z
    // (and the Fable Anniversary 360 PDB public of the same name, lib_sound_bank_base.cpp), out of line in retail at 0x00A01920.
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;          // 0x00A01920
#endif
private:
    // Native constructor00a0bd00 zeros both owned-handle words; native dtor00a0bd40 releases them.
    unsigned char ResourceHandle[8];
};
FABLE_STATIC_ASSERT(sizeof(CASoundBank)==12);
#endif
