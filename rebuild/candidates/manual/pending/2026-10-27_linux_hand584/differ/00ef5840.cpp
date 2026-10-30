// NScript::CPersonalScript_EmptyScript::CBanditWalker::Main() -- the bandit walker: guard the thread with "me and both route markers are alive", take scripting control of me, walk to BanditRoute1, then patrol BanditRoute1 / BanditRoute2 until the thread terminates
// Grade: DIFFER(768v727) -- the only residue: inside the inlined dtor of the CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive> temporary, retail CALLS the member's ~CPredicate_And<CIsThingAlive,CIsThingAlive> out of line (0x00EF5B50) while still expanding the inner temporary's identical dtor and both CPredicateBase vtable stores; VC7.1 expands both here (frame layout and every other byte identical)
// Leaf: ?Main@CBanditWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF5840, 727 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the CBanditWalker entity vtable 0x012F60F0, stored by the landed CBanditWalker::Alloc
//   0x00EF1B70; Ego_r.pdb / FableWin.pdb name ?Main@CBanditWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ. Twin of the
//   landed CCrate1::Main 0x00EF54A0 (same TU).
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0, CScriptThing copy 0x004ABE90 / ~ 0x004AA840 (out of line at the late exits),
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, CPredicate_And<A,A>(const A&, const A&) 0x00E24760,
//   CPredicate_And<CPredicate_And<A,A>,A>(...) 0x00EF5BC0, ~CPredicate_And<A,A> 0x00EF5B50 (retail only), ~CIsThingAlive
//   0x00E241B0, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380,
//   ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToThing (slot 5) 0x007E7300 / FollowPreCalculatedRoute
//   (slot 6) 0x007E7310 (direct calls on the local) / ~ 0x007E74D0; CGameScriptInterface slots 72 GetThingWithScriptName,
//   7 NewScriptFrame, 8 StartScriptingEntity.
// Data: "BanditRoute1" 0x012F6644, "BanditRoute2" 0x012F6634; vtables CIsThingAlive 0x012C32F0, CPredicate_And<A,A> 0x012E1B70,
//   CPredicate_And<CPredicate_And<A,A>,A> 0x012F6628, CPredicateBase 0x012C2FF4, CScriptThing 0x01238C8C, ScriptedThingBase 0x0127094C.
// Tried (3 grades): (1) explicit specialisation declaration of ~CPredicate_And<A,A> (both sites call it; frame slots of the two
//   predicate temporaries swap: 704v727); (2) `#pragma inline_depth(1)` (member call right, but ~CPredicateBase also goes out
//   of line at depth 2 and ebx is no longer saved: 735v727); (3) this source, default depth (768v727). Per-site inlining of the
//   same inline dtor (temporary vs member subobject) looks like the chunk-583 per-exit-site gap; parked per the brief.
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer() : Data(0), Info(0) { }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // vtable 0x01238C8C, copy 0x004ABE90, ~CScriptThing 0x004AA840
inline CScriptThing::~CScriptThing() { }
#include "engine/CIsThingAlive.h"                      // vtable 0x012C32F0, ~CIsThingAlive 0x00E241B0
#include "engine/CPredicate_And.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
template <class P1, class P2> inline NScript::CPredicate_And<P1, P2>::~CPredicate_And() { }
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"                  // SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30

#ifdef _MSC_VER
enum EScriptEntityMoveType;
#else
enum EScriptEntityMoveType { };
#endif
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                 // vtable 0x0129A7D8
protected:
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    __forceinline CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual ~CScriptGameResourceObjectScriptedThingBase();                // 0 0x007E7520 (scalar deleting); 0x007E74D0
    virtual void Validate() const;                                       // 1 0x007E72C0
    virtual void SummonerLightningOrbAttackTarget(const CScriptThing&);  // 2 0x007E72D0
    virtual void FireProjectileWeaponAtTarget(const CScriptThing&);      // 3 0x007E72E0
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);                  // 4 0x007E72F0
    virtual void MoveToThing(const CScriptThing&, float, EScriptEntityMoveType, bool, bool, bool, bool);      // 5 0x007E7300
    virtual void FollowPreCalculatedRoute(const CScriptThing&, EScriptEntityMoveType, bool, bool);            // 6 0x007E7310
    bool IsValid() const;                               // 0x00CD23B9 (Ego_r.pdb: the only bool non-virtual; tests PImp.Data)
    void Release();                                     // 0x00CD2770
};

namespace NScript {
class CEmptyData;
class CPersonalScript_EmptyScript {
public:
    class CBanditWalker;
};
class CPersonalScript_EmptyScript::CBanditWalker : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
private:
    virtual void Main();
};


void CPersonalScript_EmptyScript::CBanditWalker::Main()
{
    CScriptThing route1 = Game->GetThingWithScriptName("BanditRoute1");
    CScriptThing route2 = Game->GetThingWithScriptName("BanditRoute2");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(route1) && CIsThingAlive(route2));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase me;
    if (me.IsValid())
        me.Release();
    while (!Game->StartScriptingEntity(Me, me, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    me.MoveToThing(route1, 0.0f, (EScriptEntityMoveType)1, true, true, false, true);
    while (!IsActiveThreadTerminating()) {
        me.FollowPreCalculatedRoute(route1, (EScriptEntityMoveType)1, true, true);
        me.FollowPreCalculatedRoute(route2, (EScriptEntityMoveType)1, true, true);
        Game->NewScriptFrame();
    }
}
}   // namespace NScript
