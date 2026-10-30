// CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit) -- watch thread: frame by frame, whenever the hero hits the hired bandit (by weapon or an aggressive special ability) and the bandit still has health, flag the attack and count it
// Levers: ~CScriptThing __forceinline; CCountedPointer dtor zero order `Data = 0; Info = 0;`; for(;;) with in-body `#pragma inline_depth(0)` around the third terminating return (after the health test), the one exit retail calls 0x004AA840 at
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F061E0, length 395 bytes (+ int3 padding), `ret 0Ch` (the 12-byte
// CScriptThing is passed by value and destroyed here).
// Identity: Ego_r.pdb ?WatchForHeroAttackOnBandit@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z (spawned as
// "WatchForHeroAttackOnBandit", string 0x012F7ED8, at 0x00F0604E).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0; the thing's implementation object slots 21 MsgIsHitBy (+0x54) and 43
// MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xAC) through inline CScriptThing virtuals (vtable 0x01238C8C); the
// by-value destructor: inline (operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0) at two exits and the
// out-of-line ~CScriptThing 0x004AA840 at the third; through the quest's interface (+0x40): NewScriptFrame (slot 7),
// GetHealth (264).
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, 0.0001f 0x0129BA3C.
// Layout: retail = Ego_r.pdb - 0x14: BanditAttackedByHero +0x90, BanditAttackByHeroCount +0x94.
// Shape: the landed CQ_BountyHuntScript::WatchForLeaderKilled (0x00D17090) by-value CScriptThing model; the hit test
// is a named bool over one `||` chain whose operands each hold a "SCRIPT_NAME_HERO" conversion temporary (flags 1/2
// in ebx).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
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

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual __forceinline ~CScriptThing() {}                          // out of line: 0x004AA840
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& attacker) const     // slot 21 (+0x54)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsHitBy(attacker);
        return false;
    }
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const     // slot 43 (+0xac)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
        return false;
    }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x90 - 0x48];
    bool                  BanditAttackedByHero;         // +0x90 (PDB +0xa4)
    long                  BanditAttackByHeroCount;      // +0x94 (PDB +0xa8)
    void WatchForHeroAttackOnBandit(CScriptThing bandit);
};

void CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool attacked = bandit.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                        bandit.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (attacked) {
            if (IsActiveThreadTerminating())
                return;
            if (GSI->GetHealth(bandit) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                BanditAttackedByHero = true;
                ++BanditAttackByHeroCount;
            }
        }
        GSI->NewScriptFrame();
    }
}
}