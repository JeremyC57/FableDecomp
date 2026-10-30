// CShowTargetedThingHealthScript::ShowTargetedThingsHealth() -- script thread: whenever the hero targets a different
//   living thing, replace the quest-info health bar with one for the new target; when the old target dies and nothing
//   living is targeted, remove the bar. Runs until the thread terminates.
// Leaf: ShowTargetedThingsHealth
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFF930, 893 bytes (+ int3 padding), plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0; ~CScriptThing
//   0x004AA840 (out-of-line copy); operator delete 0x00BFE9BC; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0. CGameScriptInterface (quest +0x40) slots 7 NewScriptFrame, 71 GetHeroTargetedThing, 321 DisplayQuestInfo,
//   325 AddQuestInfoBarHealth, 338 RemoveQuestInfoElement (ghidra_out/cgsi_slots_aligned.tsv); CScriptThing slot 75
//   IsAlive (0x004AB130: `PImp && PImp->IsAlive()`) and 76 IsDead (0x004AA890: `!IsAlive()` through the vtable);
//   the thing impl's slots 75 IsAlive and 78 IsEqualTo. Literal ""
//   0x0122D70E; 1.0f pushed; the bar colour is opaque white.
// Layout: CScriptBase is 0x40 bytes in retail (as the landed CQ_HangingTreeEvilScript::MonitorEnemies 0x00D6BE20), the
//   script interface at +0x40. CScriptThing = CBase (vtable) + CCountedPointer {Data, Info} (vtable 0x01238C8C), as the
//   landed CHTG_FreedomFighter::Main 0x00D701C0.
// Shape: the GOTCHAS b18 mixed-dtor recipe: `__forceinline` ~CScriptThing, `#pragma inline_depth(0)` around the two
//   late returns retail sends to the out-of-line copy, and `for (;;) { if (IsActiveThreadTerminating()) return; ... }`.
//   The counted-pointer assignment reads the source's Info then Data first (the landed RemoveParty 0x00D732F0 model).
//   IsAlive and IsDead are inline virtuals expanded on the locals, so IsDead leaves its own virtual IsAlive call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;              // the source's fields are read first
        void* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImp {
    virtual void i000() const; virtual void i001() const; virtual void i002() const; virtual void i003() const; virtual void i004() const; virtual void i005() const;
    virtual void i006() const; virtual void i007() const; virtual void i008() const; virtual void i009() const; virtual void i010() const; virtual void i011() const;
    virtual void i012() const; virtual void i013() const; virtual void i014() const; virtual void i015() const; virtual void i016() const; virtual void i017() const;
    virtual void i018() const; virtual void i019() const; virtual void i020() const; virtual void i021() const; virtual void i022() const; virtual void i023() const;
    virtual void i024() const; virtual void i025() const; virtual void i026() const; virtual void i027() const; virtual void i028() const; virtual void i029() const;
    virtual void i030() const; virtual void i031() const; virtual void i032() const; virtual void i033() const; virtual void i034() const; virtual void i035() const;
    virtual void i036() const; virtual void i037() const; virtual void i038() const; virtual void i039() const; virtual void i040() const; virtual void i041() const;
    virtual void i042() const; virtual void i043() const; virtual void i044() const; virtual void i045() const; virtual void i046() const; virtual void i047() const;
    virtual void i048() const; virtual void i049() const; virtual void i050() const; virtual void i051() const; virtual void i052() const; virtual void i053() const;
    virtual void i054() const; virtual void i055() const; virtual void i056() const; virtual void i057() const; virtual void i058() const; virtual void i059() const;
    virtual void i060() const; virtual void i061() const; virtual void i062() const; virtual void i063() const; virtual void i064() const; virtual void i065() const;
    virtual void i066() const; virtual void i067() const; virtual void i068() const; virtual void i069() const; virtual void i070() const; virtual void i071() const;
    virtual void i072() const; virtual void i073() const; virtual void i074() const;
    virtual bool IsAlive() const;                                       // slot 75 (+0x12c)
    virtual void i076() const; virtual void i077() const;
    virtual bool IsEqualTo(const CScriptThingImp* other) const;         // slot 78 (+0x138)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const;
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const;
    virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const;
    virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const; virtual void s024() const;
    virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const;
    virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const;
    virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const;
    virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const;
    virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const;
    virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const;
    virtual void s061() const; virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const;
    virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const;
    virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && Imp()->IsAlive(); }   // slot 75 (+0x12c) 0x004AB130
    virtual bool IsDead() const { return !IsAlive(); }                       // slot 76 (+0x130) 0x004AA890
    CCountedPointer<CScriptThing> PImp;                               // +0x04 Data, +0x08 Info
    CScriptThing() {}
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
    const CScriptThingImp* Imp() const { return (const CScriptThingImp*)PImp.Data; }
};
inline bool operator==(const CScriptThing& a, const CScriptThing& b)
{
    return a.Imp()->IsEqualTo(b.Imp());
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CShowTargetedThingHealthScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void ShowTargetedThingsHealth();
};

void CShowTargetedThingHealthScript::ShowTargetedThingsHealth()
{
    CScriptThing target;
    CScriptThing previous;
    long healthBar = -1;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        target = GSI->GetHeroTargetedThing();
        if ((target.IsAlive() && previous.IsAlive() && !(target == previous)) || (target.IsAlive() && previous.IsDead())) {
            if (IsActiveThreadTerminating())
                return;
            if (healthBar >= 0) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                GSI->RemoveQuestInfoElement(healthBar);
            }
            GSI->DisplayQuestInfo(true);
            healthBar = GSI->AddQuestInfoBarHealth(target, CRGBColour(255, 255, 255, 255), "", 1.0f);
        } else if (previous.IsAlive() && target.IsDead()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            GSI->RemoveQuestInfoElement(healthBar);
        }
        previous = target;
        GSI->NewScriptFrame();
    }
}