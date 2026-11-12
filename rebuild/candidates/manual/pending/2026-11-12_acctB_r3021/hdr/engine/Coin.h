#ifndef FABLE_ENGINE_COIN_H
#define FABLE_ENGINE_COIN_H
// New shared header (w22 agent2007, row 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput): the tavern coin-game coin record.
// Ego_r.pdb class Coin (key V: ??0Coin@@QAE@MMPAVCThing@@_N@Z, ?Update@Coin@@QAEXM@Z, ?UpdateSound@Coin@@QAEX_N@Z, ??0Coin@@QAE@ABV0@@Z),
// 60 bytes, derived from Mass (engine/Mass.h: Pos +0x04, Vel +0x0C ...). Members (struct_layouts_egor.tsv): Owned +0x24, Thing +0x25
// (CIntelligentPointer<CThing>, byte-packed, so it sits at an odd offset), Radius +0x30, Owner +0x34, Sound +0x38. Retail agrees: the
// landed copy constructor 0x008EFF40 copies exactly these offsets, CalcScores 0x007CF860 reads Pos +0x04 / Radius +0x30, ProcessInput
// 0x007CF9A0 writes Pos.X +0x04 and Vel +0x0C / +0x10 through CTCTavernGameCoinBase::Held. Methods: declarations only (the
// landed rows define them locally).
// Needs FABLE_RETAIL_CINTELLIGENTPOINTER (packed pointer) and the retail C2DVector (FABLE_RETAIL_C2DVECTOR) for the 0x24-byte Mass.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#include "engine/Mass.h"
#include "engine/CIntelligentPointer.h"
class CThing;

class Coin : public Mass {
public:
    Coin(float x, float radius, CThing* thing, bool owned);             // ??0Coin@@QAE@MMPAVCThing@@_N@Z (argument names descriptive)
    Coin(const Coin&);                                                   // ??0Coin@@QAE@ABV0@@Z 0x008EFF40
    void Update(float);                                                  // ?Update@Coin@@QAEXM@Z
    void UpdateSound(bool);                                              // ?UpdateSound@Coin@@QAEX_N@Z 0x008EF400
    bool                        Owned;                                   // +0x24
    CIntelligentPointer<CThing> Thing;                                   // +0x25
    float                       Radius;                                  // +0x30
    long                        Owner;                                   // +0x34 (PDB int)
    unsigned long               Sound;                                   // +0x38
};
FABLE_STATIC_ASSERT(offsetof(Coin, Owned) == 0x24);
FABLE_STATIC_ASSERT(offsetof(Coin, Radius) == 0x30);
FABLE_STATIC_ASSERT(sizeof(Coin) == 0x3c);
#endif
