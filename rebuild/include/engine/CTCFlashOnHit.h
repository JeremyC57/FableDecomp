#ifndef FABLE_ENGINE_CTCFLASHONHIT_H
#define FABLE_ENGINE_CTCFLASHONHIT_H
#include "engine/CTCBase.h"
// TPI0x323a5 original fields. Native00667e45/48 and007e33a0 prove offsets.
class CTCFlashOnHit : public CTCBase {
private: long FadeInTime,FadeOutTime,LifeTime,Timer;
public:
    void SetFadeInTime(long n) { FadeInTime=n; }
    void SetFadeOutTime(long n) { FadeOutTime=n; }
    void SetLifeTime(long);
#ifdef FABLE_RETAIL_CTCFLASHONHIT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004D62CE
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCFlashOnHit)==0x1c);

#endif
