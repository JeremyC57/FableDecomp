#ifndef FABLE_ENGINE_CTCWALLMOUNT_H
#define FABLE_ENGINE_CTCWALLMOUNT_H
// CTCWallMount (new, chunk 765): RTTI .?AVCTCWallMount@@, single base CTCBase at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (struct_layouts_egor.tsv, 32 bytes, no STL container): BoughtForAmount +0x0C, Trophy (CIntelligentPointer<CThing>) +0x10, TrophyID
// (unsigned __int64) +0x18. Ego_r donor order GetObjectAttached 0x007DA9A0, PeekObjectAttached 0x007DA9B0, HasObjectAttached 0x007DA9C0;
// retail 0x007DE4E0 / 0x007DE4F0 both forward Trophy +0x10 to CBaseIntelligentPointer::GetPItem. Only the members and methods rows use
// are modelled (no virtual overrides).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CTCWallMount : public CTCBase {
public:
    CThing* GetObjectAttached();                                         // 0x007DE4E0
    const CThing* PeekObjectAttached() const;                            // 0x007DE4F0
protected:
    long BoughtForAmount;                                                // +0x0c
    CIntelligentPointer<CThing> Trophy;                                  // +0x10
    unsigned __int64 TrophyID;                                           // +0x18
#ifdef FABLE_RETAIL_CTCWALLMOUNT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x007DE490
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCWallMount) == 0x20);
#endif
