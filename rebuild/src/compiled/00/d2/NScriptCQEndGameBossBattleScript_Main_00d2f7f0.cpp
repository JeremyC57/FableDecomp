// CQ_EndGameBossBattleScript::Main() -- the end-game Jack of Blades boss battle main: bind the SwordVoid / EndGameSister / EndGameJackOfBladesBoss / TheRealGuildmaster entity scripts, give the Jack fight quest card, activate Q_EndGameBossBattleScenery and run the battle
// Leaf: ?Main@CQ_EndGameBossBattleScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2F7F0, 0x23c bytes, plain `ret`.
// Label: manifest "CV_GhostGrannyNecklaceScript::Main" is propagated; the class is CQ_EndGameBossBattleScript (vtable
//   0x012CC60C, object size 0x6c = Ego_r 128 - 0x14, allocated at 0x00D34410; the binding names are its
//   CSwordVoid / CEndGameSister / CEndGameJackOfBladesBoss / CTheRealGuildmaster nested classes in Ego_r.pdb).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, operator new 0x00BFEA1A, AddEntityScriptBinding 0x00CB8230
//   (binding vtable 0x012CC6A0), CScriptBase::PostAddScriptedEntities 0x00CB8930; allocators 0x00D2FCE0 / 0x00D2FC00 /
//   0x00D2FA40 / 0x00D2FDC0 (manifest "CTriangleList" labels are propagated); 0x00D30030 the battle body (Ego_r analogue
//   JackOfBladesBossBattle, the call Main ends on; unverified); CGameScriptInterface slots 64, 276 ActivateQuest,
//   295 GiveHeroQuestCardDirectly, 655 GetActiveQuestName.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

class CGameScriptInterface;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_EndGameBossBattleScript : public CScriptBase {
public:
    struct CSwordVoid {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_EndGameBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D2FCE0
    };
    struct CEndGameSister {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_EndGameBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D2FC00
    };
    struct CEndGameJackOfBladesBoss {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_EndGameBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D2FA40
    };
    struct CTheRealGuildmaster {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_EndGameBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D2FDC0
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void JackOfBladesBossBattle();                       // 0x00D30030
};
} // namespace NScript
using NScript::CQ_EndGameBossBattleScript;

void NScript::CQ_EndGameBossBattleScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameBossBattleScript>("SwordVoid", this, &CSwordVoid::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameBossBattleScript>("EndGameSister", this, &CEndGameSister::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameBossBattleScript>("EndGameJackOfBladesBoss", this, &CEndGameJackOfBladesBoss::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameBossBattleScript>("TheRealGuildmaster", this, &CTheRealGuildmaster::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_JACK_FIGHT", Game->GetActiveQuestName(), false);
    Game->ActivateQuest("Q_EndGameBossBattleScenery");
    JackOfBladesBossBattle();
}