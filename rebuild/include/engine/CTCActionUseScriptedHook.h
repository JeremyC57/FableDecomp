#ifndef FABLE_ENGINE_CTCACTIONUSESCRIPTEDHOOK_H
#define FABLE_ENGINE_CTCACTIONUSESCRIPTEDHOOK_H
// CTCActionUseScriptedHook (chunk 849, row 0x00501450 CWorldMap::GenerateRegionConnectivityGraph): Ego_r.pdb members and order
// (struct_layouts_egor.tsv, sizeof 80; the retail size is the PDB's: CCharString is one word in both), the CTCBase base, interface type
// TCI_ACTION_USE_SCRIPTED_HOOK = 194 (the row's bit test `test byte ptr [eax+38h],4`, 0x0050163F, and the inline LowerBound compare
// with 0xC2). PEntranceConnectedTo +0x20 is the landed rows' offset (OnSerialise 0x00729CF0, GetRegionEntranceName 0x00729C50);
// GetPEntranceConnectedTo is the public const accessor the Codex pass 13 TPI read names, inline in retail (0x00501694: `add ecx,20h ;
// call 0x00A01B50`, CBaseIntelligentPointer::GetPItem). Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
class CThing;
class CTCActionUseScriptedHook : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ACTION_USE_SCRIPTED_HOOK; }
    CThing* GetPEntranceConnectedTo() const { return PEntranceConnectedTo.Get(); }
    const CCharString& GetRegionEntranceName() const;                   // 0x00729C50
    const CCharString& GetRegionEntranceDisplayName() const;            // 0x00729CA0
#ifdef FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
    // ?IsUsable@CTCActionUseScriptedHook@@QBE_NXZ, inline in retail (`cmp byte [eax+0Ch],0`: the scripted hook that is not usable
    // gets no "use" cursor). No layout change.
    bool IsUsable() const { return Usable; }
#endif
private:
    bool             Usable;                                             // +0x0c
    bool             ForceConfirmation;                                  // +0x0d
    bool             WaitingForConfirmation;                             // +0x0e
    bool             PerformAction;                                      // +0x0f
    bool             TeleportToRegionEntrance;                           // +0x10
    unsigned __int64 EntranceConnectedToUID;                             // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;                    // +0x20
    bool             EntranceLocked;                                     // +0x28
    unsigned __int64 CameraTrackUID;                                     // +0x30
    CIntelligentPointer<CThing> PCameraTrack;                            // +0x38
    CCharString      SoundName;                                          // +0x40
    CCharString      AnimationName;                                      // +0x44
    long             ReplacementObject;                                  // +0x48
    bool             ReversedOnMiniMap;                                  // +0x4c
    bool             HiddenOnMiniMap;                                    // +0x4d
#ifdef FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsSerialiseCreatable() const;  // slot 19, 0x0072A410
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x0072A420
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseScriptedHook) == 0x50);
#endif
