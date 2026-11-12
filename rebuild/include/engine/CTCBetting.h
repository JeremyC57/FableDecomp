#ifndef FABLE_ENGINE_CTCBETTING_H
#define FABLE_ENGINE_CTCBETTING_H
#ifdef FABLE_RETAIL_CTCBETTING
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CTCHeroStats;class CEnginePrimitiveHandle;
// Original Ego_r339f4 public CTCBase base; native007e57c0 with 12-byte vectors.
class CTCBetting:public CTCBase {
public:
 CWideString GetPlayersMoney();
 CWideString GetText(unsigned long);
 void GetHeroStats(CTCHeroStats**);
private:
 bool Active;
 std::vector<CEnginePrimitiveHandle> BetHandles,MoneyHandles;
 long FrameBetWasLastChanged,FrameBetChangeStarted,Mult,FrameToStart,PreviousDonation;
#ifdef FABLE_RETAIL_CTCBETTING_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007E5910
public:
#endif
#ifdef FABLE_RETAIL_CTCBETTING_FRAMEUPDATE
    // Opt-in (wf16 row 0x007E4E30): Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCBetting@@UAEXXZ, vtable 0x01270624 slot 9.
    // Declaration only (no layout change; the vtable already exists).
public:
    virtual void FrameUpdate();           // slot 9, 0x007E4E30
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCBetting)==0x3c);
#else
class CTCBetting;
#endif
#endif
