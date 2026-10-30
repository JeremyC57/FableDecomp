// CAT_Phase3Archer::Main() -- Arena phase-3 archer: alive filter on Me, find "AT_ConvoySpy" and ally it with every live guard of the parent; make it (or the hero) the archer's best enemy target, mark the archer on the minimap, wait for the phase to end, then take scripted control, walk to the nearest REGION_EXIT_POINT and fade out
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F04CC0, 1056 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CIsThingAlive dtor 0x00E241B0, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, scripted-thing handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716; GameCalls slots 7, 8, 70 GetHero,
//   72 GetThingWithScriptName, 81 GetNearestWithDefName, 350 MiniMapAddMarker, 451 GiveThingBestEnemyTarget,
//   597 EntitySetThingAsEnemyOfThing, 599 EntitySetThingAsAllyOfThing.
// Data: "AT_ConvoySpy" 0x012F7CE8, "HUD_ORB_RED_SMALL" 0x012D4BF0, "REGION_EXIT_POINT" 0x012CA510, zero vector 0x0143E8E0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (phase flags +0x48 / +0x49, guard vector +0xa8).
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                      // 0x0143E8E0
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CScriptThingImp {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014();
    virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022();
    virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030();
    virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038();
    virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046();
    virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054();
    virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062();
    virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070();
    virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
template <class T> class CCountedPointer { public:
    CScriptThingImp* Data;
    void*            Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThing> PImp;                              // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    virtual void t1(); virtual void t2(); virtual void t3(); virtual void t4(); virtual void t5();
    virtual __forceinline const C3DVector& GetPos() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPosition(); }   // slot 6
    virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14();
    virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22();
    virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30();
    virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38();
    virtual void t39(); virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46();
    virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54();
    virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62();
    virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66(); virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70();
    virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
};
namespace NScript {
class CPredicateBase { public: virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; virtual ~CPredicateBase() { } };
class CIsThingAlive : public CPredicateBase { public:                // vtable 0x012C32F0
    CScriptThing Thing;
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    virtual bool operator()() const; virtual CPredicateBase* Clone() const;
    ~CIsThingAlive();                                  // 0x00E241B0
};
}
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
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
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CAT_Phase3Archer : public CEntityScriptBase {
    void* VTable;
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing Me;                                   // +0x08
    CArenaPhase3Script* ParentClass;                   // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};
void CAT_Phase3Archer::Main()
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
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    if (exitPoint.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        if (handle.IsScripting()) {
            if (IsActiveThreadTerminating()) return;
            handle.MoveToPosition(exitPoint.GetPos(), 1.0f, 1, false, true);
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