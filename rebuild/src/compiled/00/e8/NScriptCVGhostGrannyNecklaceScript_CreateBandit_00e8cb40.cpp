// CV_GhostGrannyNecklaceScript::CreateBandit() -- wait until the hero knows about the necklace, switch off the GreatwoodLake creature generators, then each frame until the hero holds the necklace: when Greatwood_3 is loaded, the bandit is not killed and no live GhostGrannyBandit exists, create a bandit lieutenant at GhostGrannyBanditSpawn, attach it to the quest, spawn CheckBanditDead on it and wait for Greatwood_3 to unload; finally switch the generators back on
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8CB40, 0x3fc bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc1<CV_GhostGrannyNecklaceScript, CScriptThing> vtable 0x012EBC50, 0x48 bytes,
//   CheckBanditDead 0x00E8CF40), AddSpawnedFunction 0x00CB7E50, CScriptThing copy 0x004ABE90 / ~ 0x004AA840 (out of line),
//   CScriptThing virtual slots 6 GetPosition, 75 IsAlive; Game slots 7, 11 IsLevelLoaded, 70 GetHero,
//   72 GetThingWithScriptName, 91 CreateCreature, 184 IsObjectInThingsPossession, 465 EntityAttachToScript,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName.
// Shape: the outer wait is `while (!IsActiveThreadTerminating())` (entry check + bottom check, aligned loop head) with
//   the necklace test first and the post-loop statements inside its branch.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, HeroKnowsAboutNecklace +0x49, BanditKilled +0x4b.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T, class A> struct CSpawnedFunc1 : CSpawnedFuncBase {   // retail 0x48 bytes
    void (T::*Func)(A);                                 // +0x34
    T* PClass;                                          // +0x38
    A Arg;                                              // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(A), A arg)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Arg(arg) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Arg); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_GhostGrannyNecklaceScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool WifeNeededForCutscene;                          // +0x48
    bool HeroKnowsAboutNecklace;                         // +0x49
    bool NecklaceReturned;                               // +0x4a
    bool BanditKilled;                                   // +0x4b
    void CreateBandit();
    void CheckBanditDead(CScriptThing bandit);           // 0x00E8CF40
};
} // namespace NScript
using NScript::CV_GhostGrannyNecklaceScript;

void NScript::CV_GhostGrannyNecklaceScript::CreateBandit()
{
    while (!HeroKnowsAboutNecklace) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("GreatwoodLake", false);
    while (!IsActiveThreadTerminating()) {
        if (Game->IsObjectInThingsPossession("OBJECT_GHOST_GRANNY_NECKLACE", Game->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            Game->SetCreatureGeneratorsEnabled("GreatwoodLake", true);
            return;
        }
        if (Game->IsLevelLoaded("Greatwood_3") && BanditKilled != true && !Game->GetThingWithScriptName("GhostGrannyBandit").IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_LIEUTENANT", Game->GetThingWithScriptName("GhostGrannyBanditSpawn").GetPos(), "GhostGrannyBandit", false);
            Game->EntityAttachToScript(bandit, Game->GetActiveQuestName());
            SpawnFunction(new CSpawnedFunc1<CV_GhostGrannyNecklaceScript, CScriptThing>("CheckBanditDead", 0, this, &NScript::CV_GhostGrannyNecklaceScript::CheckBanditDead, bandit));
            while (!!Game->IsLevelLoaded("Greatwood_3")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}