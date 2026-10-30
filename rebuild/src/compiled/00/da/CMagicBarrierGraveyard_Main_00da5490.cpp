// CMagicBarrierGraveyard::Main() (0x00DA5490) -- the second graveyard magic barrier: under an alive filter on Me, create the
//   two red force-field effects at Me (oriented by Me's XY angle); then every frame, once neither effect is alive, replace them
//   with the two force-field idle effects
// Leaf: ?Main@CMagicBarrierGraveyard@
// Retail: Fable.exe @ 0x00DA5490, 779 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
//   "NEW_RED_FORCEFIELD_01" 0x012C7D6C, "NEW_RED_FORCEFIELD_02" 0x012C7D54, "NEW_RED_FORCEFIELD_IDLE_01" 0x012C354C,
//   "NEW_RED_FORCEFIELD_IDLE_02" 0x012C3530), CScriptThing::operator= 0x008AB980, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC; CScriptThing slots 6 GetPos, 10 GetAngleXY, 75 IsAlive; GameCalls slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: entity-script base {vptr, Game +4, Me +8}; CScriptThing / CGameScriptInterface as the landed CMK_HS2_SOUL::Main
//   0x00CE0920.
// Shape: __forceinline ~CScriptThing; the effects are locals bound as `CScriptThing& e = Game->CreateEffect(...)` (IsAlive
//   stays virtual); the in-loop return and the two re-creation temporaries sit under inline_depth(0) (0x004AA840), the
//   function end expands both destructors inline; `for (;;) { if (Term()) return; ... }` (a `while` sends the first
//   early return to the wrong epilogue copy).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    __forceinline ~CScriptThing() {}                     // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28) 0x004AAA40
    virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const;
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
    virtual bool IsDead() const;                         // slot 76 (+0x130) 0x004AA890
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void f00();
};
}
#pragma inline_depth(0)
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                    // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#pragma inline_depth()
class CEntityScriptBase { public:                                // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                           // +0x04
    CScriptThing          Me;                             // +0x08
    bool IsActiveThreadTerminating() const;               // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CMagicBarrierGraveyard : CEntityScriptBase {
    void* ParentClass;                                    // +0x14
    void* PMasterData;                                    // +0x18
    void Main();
};

void CMagicBarrierGraveyard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing& effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing& effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!effect1.IsAlive() && !effect2.IsAlive()) {
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false)
#pragma inline_depth(0)
            ;
#pragma inline_depth()
            effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false)
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}