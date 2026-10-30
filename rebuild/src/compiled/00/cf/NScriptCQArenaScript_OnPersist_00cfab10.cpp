// NScript::CQ_ArenaScript::OnPersist(CPersistContext& context) -- persist the arena state, round, gold multiplier and total, new crowd points (default 0) and the ChamLeaving / PlayerLeaving flags (default false)
// Leaf: ?OnPersist@CQ_ArenaScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFAB10, 215 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the level-script vtable 0x012C7798 (0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist), stored by the quest ctor 0x00CFA660; ?OnPersist@CQ_ArenaScript@NScript@@ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00cfab10" is a gap-scan placeholder.
// Callees: CPersistContext::Transfer<long> 0x00410BE0, Transfer<bool> 0x004045C0 (Ego_r.pdb ??$Transfer@J@ / ??$Transfer@_N@CPersistContext@@); keys 0x012C78A4 .. 0x012C7850.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. CrowdLoopTags (CCharString[4][5]) +0x48, Rounds (vector) +0x98, ArenaState +0xa4, ArenaRound +0xa8, GoldMultiplier +0xb0, ChamLeaving +0xb4, GoldTotal +0xb8, PlayerLeaving +0xcc, NewCrowdPoints +0xf4.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <long> 0x00410BE0, <bool> 0x004045C0
};
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad048[0x5c];                        // +0x48 CrowdLoopTags, +0x98 Rounds (STLport vector)
    long ArenaState;                                    // +0xa4 (Ego_r.pdb +0xbc)
    long ArenaRound;                                    // +0xa8
    long ArenaRoundWave;                                // +0xac
    long GoldMultiplier;                                // +0xb0
    bool ChamLeaving;                                   // +0xb4
    long GoldTotal;                                     // +0xb8
    bool InHitCutsceneAlready;                          // +0xbc
    bool NeedBertForSpeech;                             // +0xbd
    unsigned char _pad0be[0xe];                         // +0xc0 CellsVillage (CScriptThing)
    bool PlayerLeaving;                                 // +0xcc (Ego_r.pdb +0xe4)
    bool PlayerWon;                                     // +0xcd
    unsigned char _pad0ce[0x26];
    long NewCrowdPoints;                                // +0xf4 (Ego_r.pdb +0x10c)
    virtual void OnPersist(CPersistContext& context);
};
}

void NScript::CQ_ArenaScript::OnPersist(CPersistContext& context)
{
    context.Transfer("ArenaState", ArenaState, 0L);
    context.Transfer("ArenaRound", ArenaRound, 0L);
    context.Transfer("GoldMultiplier", GoldMultiplier, 0L);
    context.Transfer("GoldTotal", GoldTotal, 0L);
    context.Transfer("NewCrowdPoints", NewCrowdPoints, 0L);
    context.Transfer("ChamLeaving", ChamLeaving, false);
    context.Transfer("PlayerLeaving", PlayerLeaving, false);
}