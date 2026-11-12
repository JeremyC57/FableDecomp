// CThing::GetScriptThing() const -- the thing's script handle, built by the quest manager's game-script-thing manager (CreateScriptThing 0x00704860, hidden result)
// Leaf: ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7E50.
//   mov eax,[0x013B89FC] ; push esi ; mov esi,[esp+8] ; push ecx ; mov ecx,[eax+4] ; push esi ; call 0x00704860 ; mov eax,esi ;
//   pop esi ; ret 4
// Naming (agentNF31, 2026-11-12): was the __fastcall stand-in Ctor_CPredicate_And_NScript(CPredicate_And*, int, CIsThingAlive*)
//   over local Callee / GlobHolder models. Ego_r.pdb 0x004C9A10 (order GetScriptThing / HasScriptThing / IsUnderScriptSupervision
//   = retail 0x004C7E50 / .. / 0x004C7E90) + FableWin.pdb 0x01CF8BE0 ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ; engine/CThing.h
//   declares it at 0x004C7E50 (FABLE_RETAIL_CTHING_STATEACCESS); the landed 0x004C7E90 row's same-TU copy of this body; 0x013B89FC =
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, +4 = PGameScriptThingManager (engine/CQuestManager.h).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CQuestManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"

class CGameScriptThingManager {                    // no engine header
public:
    CScriptThing CreateScriptThing(const CThing& thing);   // 0x00704860 (landed)
};

CScriptThing CThing::GetScriptThing() const
{
    return CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data->CreateScriptThing(*this);
}
