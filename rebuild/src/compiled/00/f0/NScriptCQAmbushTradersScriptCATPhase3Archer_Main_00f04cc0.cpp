// NScript::CQ_AmbushTradersScript::CAT_Phase3Archer::Main() -- Arena phase-3 archer: alive filter on Me, find "AT_ConvoySpy" and ally it with every live guard of the parent; make it (or the hero) the archer's best enemy target, mark the archer on the minimap, wait for the phase to end, then take scripted control, walk to the nearest REGION_EXIT_POINT and fade out
// Leaf: ?Main@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F04CC0, 1056 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CIsThingAlive dtor 0x00E241B0, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, scripted-thing handle IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716; GameCalls slots 7, 8, 70 GetHero,
//   72 GetThingWithScriptName, 81 GetNearestWithDefName, 350 MiniMapAddMarker, 451 GiveThingBestEnemyTarget,
//   597 EntitySetThingAsEnemyOfThing, 599 EntitySetThingAsAllyOfThing.
// Data: "AT_ConvoySpy" 0x012F7CE8, "HUD_ORB_RED_SMALL" 0x012D4BF0, "REGION_EXIT_POINT" 0x012CA510, zero vector 0x0143E8E0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (phase flags +0x48 / +0x49, guard vector +0xa8).
// Naming wave 35: identity reg 00f0276d "AT_Phase3Archer" in 00f024c0; Alloc 00f04bd0; vtable 012f7e4c slot 1; PDB ?Main@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@; refs=1; was under-qualified CAT_Phase3Archer::Main (wave3/labels_alloc.tsv); PDB ?Main@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CAT_Phase3Archer@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
#include "engine/C3DVector.h"
extern C3DVector g_ZeroVector;                      // 0x0143E8E0
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPos(); }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t) : Thing(t) { }
namespace NScript {
}
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
enum EScriptEntityMoveType;
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);   // 0x007E72F0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
struct CScriptThingVector {                            // STLport vector<CScriptThing> (12 bytes)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CScriptThing& operator[](unsigned long i) { return *(_M_start + i); }
};
struct CArenaPhase3Script {
    unsigned char _pad[0x48];
    bool PhaseOver;                                    // +0x48
    bool PhaseFailed;                                  // +0x49
    unsigned char _pad4a[0xa8 - 0x4a];
    CScriptThingVector Guards;                         // +0xa8
};
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_AmbushTradersScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CAT_Phase3Archer;
};
}
using NScript::CQ_AmbushTradersScript;
namespace NScript {
class CQ_AmbushTradersScript::CAT_Phase3Archer : public CEntityScriptBase {
public:
        CGameScriptInterface* Game;                        // +0x04
    CScriptThing Me;                                   // +0x08
    CArenaPhase3Script* ParentClass;                   // +0x14
    void* PMasterData;                                 // +0x18
private:
    virtual void Main();                          // PDB ?Main@CAT_Phase3Archer@CQ_AmbushTradersScript@NScript@@EAEXXZ
};
}
void NScript::CQ_AmbushTradersScript::CAT_Phase3Archer::Main()
{
    SetPredicate(NScript::CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase handle;
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    for (unsigned long i = 0; i < ParentClass->Guards.size(); ++i) {
        if (ParentClass->Guards[i].IsAlive() && spy.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetThingAsAllyOfThing(spy, ParentClass->Guards[i]);
            Game->EntitySetThingAsAllyOfThing(ParentClass->Guards[i], spy);
        }
    }
    if (spy.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetThingAsEnemyOfThing(Me, spy);
        Game->EntitySetThingAsEnemyOfThing(spy, Me);
        Game->GiveThingBestEnemyTarget(Me, spy);
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    }
    Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), Me);
    Game->MiniMapAddMarker(Me, "HUD_ORB_RED_SMALL");
    while (!ParentClass->PhaseOver && !ParentClass->PhaseFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    if (exitPoint.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        if (handle.IsValid()) {
            if (IsActiveThreadTerminating()) return;
            handle.MoveToPosition(exitPoint.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
        }
        while (!IsDistanceBetweenThingsUnder(Me, exitPoint, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        FadeOutAndKillEntity(Me, true, 1.0f, true);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}