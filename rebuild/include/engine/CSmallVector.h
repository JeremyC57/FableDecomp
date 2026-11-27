#ifndef FABLE_ENGINE_CSMALLVECTOR_H
#define FABLE_ENGINE_CSMALLVECTOR_H
// NEW shared header (AUTH22, 2026-11-22; first user row 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents).
// CSmallVector<T,N>: a GLOBAL template in both debug PDBs (FableWin.pdb ?begin@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@QAEPAVCEvent@NAnimationEvents@@XZ,
// ?end@.., ?size@..QBEJXZ, ?empty@.., ?erase@.., ?resize@..QAEXJABVCEvent@NAnimationEvents@@@Z; Ego_d.pdb TPI
// `CSmallVector<NAnimationEvents::CEvent,32>`, sizeof 8: protected Start +0x00 (T*), Size +0x04 (int)). The landed rows of the
// animation-events TU model it locally with the same two members (0x006F9820, 0x006FADE0 resize).
// Only the members the first user needs. Bodies: inline in retail. The destructor's body is INFERRED from its expansion
// (0x0066BA0B..0x0066BA24: `test eax,eax ; je ; push eax ; call operator delete[] 0x00BFEB1C ; add esp,4`, then Start = 0, Size = 0);
// begin / end are the pointer pair (0x0066B70B `mov eax,[Start]`, 0x0066B715 `lea ecx,[eax+Size*8]`).
#include "rebuild_abi.h"
void __cdecl operator delete[](void*);                                  // 0x00BFEB1C (MSVCR71 ??_V@YAXPAX@Z)
template <class T, long N> class CSmallVector {
public:
    CSmallVector() : Start(0), Size(0) { }
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
    CSmallVector(const CSmallVector&);
    CSmallVector& operator=(const CSmallVector& other) { if (&other != this) Copy(other); return *this; }
protected:
    void Copy(const CSmallVector& other);
public:
#endif
    ~CSmallVector() { delete [] Start; Start = 0; Size = 0; }
    T* begin() { return Start; }
    T* end() { return Start + Size; }
protected:
    T*  Start;                                                           // +0x00
    int Size;                                                            // +0x04
};
#endif // FABLE_ENGINE_CSMALLVECTOR_H
