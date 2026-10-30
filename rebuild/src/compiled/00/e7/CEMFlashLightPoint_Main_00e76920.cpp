// CEM_FlashLightPoint::Main() -- Expose-the-Mayor lamp point entity: waits for stage 2, spawns the parent's CheckLampSlot thread, then until the hero has flashed the lights correctly waits for the hero to come within MaxFlashDistFromStables, waits for the hand lamp to go off and on again, spawns HeroFlashesLamp(5) and waits for a result or a lamp reset
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) NScript::CV_ExposeMayorScript::CEM_FlashLightPoint::Main @ 0x00E76920, 851 bytes (+ int3 padding).
// Callees: IsActiveThreadTerminating 0x00F35B30 (on this), operator new 0x00BFEA1A (0x3c / 0x40 bytes), CCharString(const
// char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator+ 0x0099F570 (__fastcall), CSpawnedFuncBase ctor 0x00CDD450,
// CScriptBase::AddSpawnedFunction 0x00CB7E50 (catalog "CGuiVarTransferStruct::Add" is propagated), IsDistanceBetweenThingsUnder
// 0x00CBE2FF (__fastcall); GameCalls slots 7 NewScriptFrame (+0x1c), 70 GetHero (+0x118), 185 IsHeroHandLampLit (+0x2e4),
// 376 Pause (+0x5e0). Spawned threads: CSpawnedFunc0<CV_ExposeMayorScript> (vtable 0x012E96E0, CheckLampSlot 0x00E76CA0)
// and CSpawnedFunc1<CV_ExposeMayorScript, long> (vtable 0x012E9924, HeroFlashesLamp 0x00E76E90, Par1 = 5).
// Strings: "CheckLampSlot" 0x012E9950, "HeroFlashesLamp" 0x012E9940, "ParentClass." 0x012C9934, "" 0x0122D70E.
// Layout: CEM_FlashLightPoint (Ego_r.pdb, 28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18. CV_ExposeMayorScript members sit 0x14 below their PDB offsets (quest-script base 0x40 in retail, as
// for the other quest scripts): StageNumber +0x60 -> +0x4c, HeroFlashedLightsCorrectly +0x64 -> +0x50, ResetLamp +0x66 ->
// +0x52, MaxFlashDistFromStables +0xb0 -> +0x9c.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement, WAIT_UNTIL loops of
// NewScriptFrame + terminate test); the spawned threads are the landed CFishCompOrganiser::OnPredicateFail
// (0x00E7F030) model: the thread name temporaries exist only when the new succeeded, so VC guards them with flag
// masks (7, then 0x38) in ebx.
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

class CScriptThing { public:                                   // 12 bytes {vptr, PImp_Data, PImp_Info}
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
template <class T, class P1> struct CSpawnedFunc1 : CSpawnedFuncBase {
    void (T::*Func)(P1);                                // +0x34
    T* PClass;                                          // +0x38
    P1 Par1;                                            // +0x3c
    CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:
    unsigned char _pad40[0x48 - 0x40];
    long  Path;                                         // +0x48 PATH_ID
    long  StageNumber;                                  // +0x4c STAGE_IN_PATH
    bool  HeroFlashedLightsCorrectly;                   // +0x50
    bool  PrisonerTalkedAboutLamp;                      // +0x51
    bool  ResetLamp;                                    // +0x52
    unsigned char _pad53[0x9c - 0x53];
    float MaxFlashDistFromStables;                      // +0x9c
    void CheckLampSlot();                               // 0x00E76CA0
    void HeroFlashesLamp(long slot);                    // 0x00E76E90
    class CEM_FlashLightPoint;
};
class CV_ExposeMayorScript::CEM_FlashLightPoint {
public:
    void*                  __vftable;                   // +0x00
    CGameScriptInterface*  Game;                        // +0x04
    CScriptThing           Me;                          // +0x08
    CV_ExposeMayorScript*  ParentClass;                 // +0x14
    void*                  PMasterData;                 // +0x18
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CV_ExposeMayorScript::CEM_FlashLightPoint::Main()
{
    while (ParentClass->StageNumber != 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc0<CV_ExposeMayorScript>(CCharString("ParentClass.") + CCharString("CheckLampSlot"), 0,
                                                ParentClass, &CV_ExposeMayorScript::CheckLampSlot),
        CCharString("", -1));
    while (!ParentClass->HeroFlashedLightsCorrectly) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, ParentClass->MaxFlashDistFromStables)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsHeroHandLampLit()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsHeroHandLampLit()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->AddSpawnedFunction(
            new CSpawnedFunc1<CV_ExposeMayorScript, long>(CCharString("ParentClass.") + CCharString("HeroFlashesLamp"), 0,
                                                          ParentClass, &CV_ExposeMayorScript::HeroFlashesLamp, 5),
            CCharString("", -1));
        while (!ParentClass->HeroFlashedLightsCorrectly && !ParentClass->ResetLamp) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->ResetLamp = false;
        Game->Pause(0.5f);
    }
    if (IsActiveThreadTerminating()) return;
}
}