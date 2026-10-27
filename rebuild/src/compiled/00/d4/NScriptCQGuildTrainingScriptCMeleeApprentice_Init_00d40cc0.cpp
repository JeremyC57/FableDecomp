// NScript::CQ_GuildTrainingScript::CMeleeApprentice::Init() -- the melee apprentice starts out waiting for its fight, with Will's woods chat not yet done
// Leaf: ?Init@CMeleeApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D40CC0, 9 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CDF7C, which the landed
//   CQ_GuildTrainingScript::CMeleeApprentice::Alloc 0x00D40C10 stores (site 0x00D40C46); Ego_r.pdb has
//   ?Init@CMeleeApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ. Entity slot order (landed Init/OnPredicateFail rows):
//   0 dtor, 1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMeleeApprentice (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18,
//   WaitingForFight +0x1c, BadHit +0x20, WillWoodsChatDone +0x24.
class CGameScriptInterfaceBase;
#include "engine/CScriptThing.h"                      // Me: 12 bytes (vptr word, PImp Data, PImp Info)
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingScript {
public:
    class CMeleeApprentice;
};
class CQ_GuildTrainingScript::CMeleeApprentice : public CEntityScriptBase {
public:
    CQ_GuildTrainingScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    WaitingForFight;            // +0x1c
    long                    BadHit;                     // +0x20
    bool                    WillWoodsChatDone;          // +0x24
private:
    virtual void Init();
};

void CQ_GuildTrainingScript::CMeleeApprentice::Init()
{
    WaitingForFight = true;
    WillWoodsChatDone = false;
}
}