// NScript::CV_FishermanScript::Main() -- the Fisherman quest main
// Leaf: ?Main@CV_FishermanScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B4E0, 698 bytes, thiscall, plain `ret` (virtual, script vtable slot 2).
// Identity (proven): slot 2 (Main) of the CV_FishermanScript vtable 0x012EA024, whose slots 1 RegisterMain 0x00E7B440 and
//   3 Init 0x00E7B420 are landed; Ego_r.pdb / FableWin.pdb name ?Main@CV_FishermanScript@NScript@@UAEXXZ.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, PostAddScriptedEntities 0x00CB8930, IsActiveThreadTerminating 0x00CB7940;
//   entity Allocs CFisherman 0x00E7B7B0, CM_FishermanWaspSpawn 0x00E7B890, CFishermanBulletinBoard 0x00E7BBE0, CFishermanWasp
//   0x00E7BA40; CGameScriptInterface slots 64 PostAddScriptedEntities, 7 NewScriptFrame, 12 IsRegionLoaded, 276 ActivateQuest,
//   655 GetActiveQuestName, 281 DeactivateQuestLater.
// Data: CEntityScriptBinding<CV_FishermanScript> vtable 0x012EA0AC; "Fisherman" 0x012C9ED0, "M_FishermanWaspSpawn" 0x012EA094,
//   "FishermanBulletinBoard" 0x012EA07C, "FishermanWasp" 0x012EA06C, "V_FishingCompetition" 0x012EA054.
// Layout (retail = Ego_r.pdb - 0x14): HeroHasSavedFishermanFromWasps +0x49.
// Shape: the standard level-script Main: four bindings, the two PostAddScriptedEntities, two wait loops (each followed by a
//   terminating test), then the quest hand-over.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CFisherman {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00E7B7B0
    };
    class CM_FishermanWaspSpawn {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00E7B890
    };
    class CFishermanWasp {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00E7BA40
    };
    class CFishermanBulletinBoard {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00E7BBE0
    };
    bool IntroPlayed;                                   // +0x48
    bool HeroHasSavedFishermanFromWasps;                // +0x49
    virtual void Main();
};

void CV_FishermanScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishermanScript>("Fisherman", this, &CFisherman::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishermanScript>("M_FishermanWaspSpawn", this, &CM_FishermanWaspSpawn::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishermanScript>("FishermanBulletinBoard", this, &CFishermanBulletinBoard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishermanScript>("FishermanWasp", this, &CFishermanWasp::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!HeroHasSavedFishermanFromWasps) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!(!Game->IsRegionLoaded("Fisherman"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ActivateQuest("V_FishingCompetition");
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript