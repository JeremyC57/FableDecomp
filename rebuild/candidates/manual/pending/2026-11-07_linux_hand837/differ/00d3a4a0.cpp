// NScript::CQ_FireHeartScript::CFH_Prisoner::Main() -- a Fireheart prisoner (prophet): under an alive filter on Me take scripted control into the quest's seh_Prophet[MyCageNumber], count itself scripted, wait (commenting when asked) until the puzzle round reaches its cage, become the guest and make the round-start comment; then until chosen: when released say the end speech into csargs and play CS_FIREHEART_RELEASE (freed count, suns, good history, morality), when killed play its end cut scene (killed count, suns, bad history, morality), else comment when asked; finally advance the round, spawn the parent's CheckCompletion thread, remove itself and quit the Fireheart minigame
// Grade: DIFFER(1140v1140) -- register colouring only (13 lines): retail forms the seh_Prophet[MyCageNumber] element address as `mov ecx,[esi+0x20]; shl ecx,4; lea edi,[edx+ecx]` where ours does `mov edi,[esi+0x20]; shl edi,4; add edi,edx`, and retail loads ParentClass into eax (ours ecx) at the StartScriptingEntity arguments; size, calls and control flow exact
// Next idea: the element address is identical under reference / pointer / begin()+n / &[0]+n / cage and parent locals (11 grades); try spelling the handle access through a CFH_Prisoner-private inline accessor (as the PDB may have one, e.g. GetMyHandle()) or reading ParentClass->seh_Prophet into a reference to the vector first
// Leaf: ?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3A4A0, 1140 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CFH_Prisoner entity vtable stored by the landed Alloc 0x00D36540 (Init 0x00D36620 landed);
//   Ego_r.pdb ?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840 (the CIsThingAlive temporary, vtable 0x012C32F0, ctor and dtor expanded),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase IsValid 0x00CD23B9 /
//   Release 0x00CD2770, CFH_Prisoner MakeComment 0x00D3ACE0 / MakeRoundStartComment 0x00D3A9B0 / GetEndSpeech 0x00D3ABE0 /
//   GetEndCutscene 0x00D3AB00 (Ego_r.pdb private members; the landed rows carry under-qualified spellings, NAME_FIXES.tsv),
//   CQ_FireHeartScript PlayCutscene 0x00D39330 / UpdateHistory 0x00D3A920 / CheckCompletion 0x00D3AD60 (member pointer),
//   map<CCharString,CCharString>::operator[] 0x009AC700, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator=
//   0x0099EFB0, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&,
//   const CCharString&) 0x0099F570, rand 0x00BFEB16 (result unused), operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc<CQ_FireHeartScript> vtable 0x012CCBA4), CScriptBase::AddSpawnedFunction 0x00CB7E50; CGameScriptInterfaceBase
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 108 RemoveThing, 156 GiveHeroMorality, 255 HeroQuitFireheartMinigame.
// Layout: engine/CQ_FireHeartScript.h (csargs +0x58, FreedProphets +0x64, KilledProphets +0x68, StartNumberOfSuns +0x6c,
//   PuzzleRound +0x88, ProphetsKilled +0xc8, ProphetsReleased +0xd4, ShouldProphetSpeak +0xe0, Guest +0x140, ProphetsScripted
//   +0x144, seh_Prophet +0x148, MoralityChangeOnKilled +0x198, MoralityChangeOnFreed +0x19c, KilledAllPrisoners +0x1a2,
//   FreedAllPrisoners +0x1a3); Ego_r.pdb CFH_Prisoner (36 bytes = retail new size): ParentClass +0x14, PMasterData +0x18,
//   Chosen +0x1c, MyCageNumber +0x20.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_text.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

namespace NScript {
class CQ_FireHeartScript::CFH_Prisoner : public CEntityScriptBase {
public:
    CQ_FireHeartScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    bool Chosen;                                    // +0x1c
    long MyCageNumber;                              // +0x20
private:
    virtual void Main();
    void MakeRoundStartComment();                   // 0x00D3A9B0
    void MakeComment();                             // 0x00D3ACE0
    void GetEndCutscene(CCharString&) const;        // 0x00D3AB00
    void GetEndSpeech(CCharString&) const;          // 0x00D3ABE0
};
}

void NScript::CQ_FireHeartScript::CFH_Prisoner::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    {
        CScriptGameResourceObjectScriptedThingBase& seh_me = ParentClass->seh_Prophet[MyCageNumber];
        if (seh_me.IsValid())
            seh_me.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass->seh_Prophet[MyCageNumber], SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->ProphetsScripted++;
    while (ParentClass->PuzzleRound < MyCageNumber) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->ShouldProphetSpeak[MyCageNumber]) {
            if (IsActiveThreadTerminating())
                return;
            MakeComment();
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->Guest = MyCageNumber;
    MakeRoundStartComment();
    while (!Chosen) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->ProphetsReleased[MyCageNumber]) {
            if (IsActiveThreadTerminating())
                return;
            GetEndSpeech(ParentClass->csargs["$LINE"]);
            ParentClass->csargs["$CAMERA"] = "CAM_FH_PRIS" + GFIntToCharString(MyCageNumber);
            ParentClass->PlayCutscene("CS_FIREHEART_RELEASE");
            ParentClass->FreedProphets++;
            ParentClass->StartNumberOfSuns--;
            ParentClass->UpdateHistory(false);
            Chosen = true;
            ParentClass->KilledAllPrisoners = false;
            Game->GiveHeroMorality(ParentClass->MoralityChangeOnFreed);
        } else if (ParentClass->ProphetsKilled[MyCageNumber]) {
            if (IsActiveThreadTerminating())
                return;
            rand();
            CCharString cutscene("");
            GetEndCutscene(cutscene);
            ParentClass->PlayCutscene(cutscene);
            ParentClass->KilledProphets++;
            ParentClass->StartNumberOfSuns++;
            ParentClass->UpdateHistory(true);
            Chosen = true;
            ParentClass->FreedAllPrisoners = false;
            Game->GiveHeroMorality(ParentClass->MoralityChangeOnKilled);
        } else if (ParentClass->ShouldProphetSpeak[MyCageNumber]) {
            if (IsActiveThreadTerminating())
                return;
            MakeComment();
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->PuzzleRound++;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>(CCharString("ParentClass.") + CCharString("CheckCompletion"), 0, ParentClass, &CQ_FireHeartScript::CheckCompletion), CCharString(""));
    Game->RemoveThing(Me, false, true);
    Game->HeroQuitFireheartMinigame();
}
