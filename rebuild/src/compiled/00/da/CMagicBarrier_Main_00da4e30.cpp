// CMagicBarrier::Main() -- a guild magic barrier: alive filter on Me, spawn the two red forcefield halves at Me turned 90 degrees; until the parent quest opens the barrier, respawn both idle halves whenever both are gone; then remove them, play the two "_OFF" effects, tell the hero the guildmaster unlocked the barrier and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA4E30, 1389 bytes; entity-script Main.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CBaseClass dtor 0x0099A2E0, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, operator delete 0x00BFE9BC; CScriptThing slots
//   6 GetPos, 75 IsAlive; the thing impl's slots 10 GetHeading, 75 IsAlive; GameCalls slots 7 NewScriptFrame, 100 CreateEffect,
//   108 RemoveThing, 274 HeroReceiveMessageFromGuildMaster, 474 EntitySetFacingAngle.
// Data: "NEW_RED_FORCEFIELD_01/02" 0x012C7D6C / 0x012C7D54, "..._IDLE_01/02" 0x012C354C / 0x012C3530, "..._IDLE_01/02_OFF"
//   0x012C3470 / 0x012C3450, "TEXT_QST_045_GUILDMASTER_UNLOCKED_BARRIER" 0x012D74E4, "" 0x0122D70E, -1.0f 0x0122DEDC,
//   90.0f 0x012B7B74.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (barrier-unlocked flag +0x4c, descriptive).
// Shape: the same script as the landed CATO_MagicBarrier::Main 0x00CFD6C0 with one wait loop; the respawn assignments use the
//   landed CTollBanditScript::HandleBanditToll 0x00E4E0E0 forceinline CCountedPointer/CScriptThing operator= (inline
//   release + inline ~CScriptThing of the temporary); `inline_depth(0)` keeps the other dtors out of line.
// One symbol per callee: CIsThingAlive holds a plain CScriptThing; its dtor is defined out of class under
//   `inline_depth(0)`, so the dtor is inlined (vtable store) and the member's ~CScriptThing stays the 0x004AA840 call.
//   Retail length 1389 bytes (the row's 1392 includes the 3-byte pad).
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC
#include "engine/CCountedPointer.h"
struct CScriptThingImp {
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetHeading() const;                    // slot 10 (+0x28)
    VPAD64(i11_0)                                        // slots 11..74
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
};
extern const C3DVector g_ZeroPos;                        // 0x0143E8E0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    __forceinline ~CScriptThing() { }                    // out of line 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }   // slot 6
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetHeading() const { return PImp.Data == 0 ? -1.0f : PImp.Data->GetHeading(); }         // slot 10
    VPAD64(t11_0)                                                                                        // slots 11..74
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase { public: virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; };
class CIsThingAlive : public CPredicateBase { public:                // vtable 0x012C32F0
    CScriptThing Thing;                                // +0x04 (copied inline, destroyed out of line)
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    virtual bool operator()() const; virtual CPredicateBase* Clone() const;
    virtual ~CIsThingAlive();
};
// The filter's dtor is inlined into Main, but its CScriptThing member's dtor stays a call (0x004AA840).
#pragma inline_depth(0)
inline CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
}
inline bool BothGone(const CScriptThing& a, const CScriptThing& b) { return !a.IsAlive() && !b.IsAlive(); }
struct CGuildBarrierScript {
    unsigned char _pad[0x4c];
    bool          BarrierUnlocked;                     // +0x4c (descriptive)
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CMagicBarrier : public CEntityScriptBase {
    void*                 __vftable;                   // +0x00
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    CGuildBarrierScript*  ParentClass;                 // +0x14
    void*                 PMasterData;                 // +0x18
    void Main();
};

void CMagicBarrier::Main()
{
    SetPredicate(NScript::CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptThing barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_01", Me.GetPos(), "", 0.0f, false, false);
    CScriptThing barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_02", Me.GetPos(), "", 0.0f, false, false);
    if (barrier1.IsAlive() && barrier2.IsAlive()) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->EntitySetFacingAngle(barrier1, barrier1.GetHeading() + 90.0f, true);
        Game->EntitySetFacingAngle(barrier2, barrier2.GetHeading() + 90.0f, true);
    }
    while (!ParentClass->BarrierUnlocked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        if (BothGone(barrier1, barrier2)) {
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", 0.0f, false, false);
            barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", 0.0f, false, false);
        }
    }
    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
        return;
    Game->RemoveThing(barrier1, false, true);
    Game->RemoveThing(barrier2, false, true);
#pragma inline_depth(0)
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01_OFF", Me.GetPos(), "", 0.0f, false, false);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02_OFF", Me.GetPos(), "", 0.0f, false, false);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_045_GUILDMASTER_UNLOCKED_BARRIER", "", true, true);
    Game->RemoveThing(Me, false, true);
}
#pragma inline_depth()