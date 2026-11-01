// NScript::CQ_GuildTrainingDepartureScript::Main() -- the guild departure quest's main: bind the TheRealGuildmaster entity script, finish adding scripted entities, wait for Q_GuildTrainingWoodsDeparture to become active and then to finish, and deactivate this quest afterwards
// Leaf: ?Main@CQ_GuildTrainingDepartureScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D50800, 448 bytes, thiscall, plain `ret`. Slot 2 (Main) of the RTTI-less script vtable
//   0x012CF670 (word 0x012CF678), whose slot 1 is the landed RegisterMain 0x00D50760.
// Identity (proven, two sources): (1) the vtable slot (CScriptBase slots 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist;
//   the landed RegisterMain / Init 0x00D506B0 / ctor 0x00D50680 rows prove the vtable is CQ_GuildTrainingDepartureScript's);
//   (2) the binding's allocator is the landed CQ_GuildTrainingDepartureScript::CTheRealGuildmaster::Alloc 0x00D509D0 and Ego_r.pdb
//   holds ?Main@CQ_GuildTrainingDepartureScript@NScript@@UAEXXZ between RegisterMain and CTheRealGuildmaster::Main (retail
//   0x00D50C60). The quest it waits for is "Q_GuildTrainingWoodsDeparture" (0x012CDFD8); the catalog row "_gapscan::sub_00d50800"
//   carried no name (the landed CQ_GuildTrainingWoodsDepartureScript::Main is 0x00D61D20, a different script).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 /
//   IsActiveThreadTerminating 0x00CB7940; the game script interface (+0x40) slots 7 NewScriptFrame, 64 PostAddScriptedEntities,
//   281 DeactivateQuestLater, 284 IsQuestActive, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Data: binding vtable 0x012CF6BC (CEntityScriptBinding<CQ_GuildTrainingDepartureScript>), "TheRealGuildmaster" 0x012C5A38.
// Shape: the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20 model (one flag word guarding the name temporary;
//   `while (!IsQuestActive(..))` gives the `neg/sbb/inc` bool, `while (IsQuestActive(..) != false)` the `setne` (a plain `while (IsQuestActive(..))` gives `mov bl,al`, 443 bytes)); every wait loop is
//   NewScriptFrame then the termination test; the last statement deactivates the active quest through a GetActiveQuestName
//   temporary (`push 0` for the delay before the hidden-pointer call).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBinding.h"                // CEntityScriptBinding<T>: 0x1c bytes, inline ctor, slot 0 GetEntityScriptName
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; every Fable script derives from
//   CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingDepartureScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTheRealGuildmaster { public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D509D0
    };
    bool Finished;                                      // +0x48
    long MeleeGrade;                                    // +0x4c
    long SkillGrade;                                    // +0x50
    long WillGrade;                                     // +0x54
    virtual void Main();                                // slot 2
};
void CQ_GuildTrainingDepartureScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingDepartureScript>("TheRealGuildmaster", this, &CTheRealGuildmaster::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsQuestActive("Q_GuildTrainingWoodsDeparture")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (Game->IsQuestActive("Q_GuildTrainingWoodsDeparture") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}