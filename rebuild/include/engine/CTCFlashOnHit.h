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
#ifdef FABLE_RETAIL_CTCFLASHONHIT_DTOR
    // agentNF182 (2026-11-20): the special members and the remaining overrides a TU that emits the vtable needs (the ??_G
    // row 0x004D62D1); declarations, plus the constructor in the form of its landed row 0x004D6292 (a user's pick-any COMDAT
    // copy grades RELOCATION_MATCH against 0x004D6292). Names: Ego_r.pdb / FableWin.pdb name sets; slots: vtable 0x0123E034.
public:
    CTCFlashOnHit(CThing& thing) : CTCBase(thing), FadeInTime(0), FadeOutTime(0), LifeTime(0), Timer(0) { } // 0x004D6292
    virtual ~CTCFlashOnHit();                                            // slot 0  0x007E3420 (??_G 0x004D62D1)
    virtual void FrameUpdate();                                          // slot 9  0x007E33B0
    virtual ETCInterfaceType GetInterfaceType() const;                   // slot 21 0x004D62B8
    virtual CCharString GetClassTypeName() const;                        // slot 22 0x004D62BE
#endif
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CTCFlashOnHit)==0x1c);

#endif
