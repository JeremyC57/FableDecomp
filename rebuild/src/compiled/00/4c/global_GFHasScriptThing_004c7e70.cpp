// GFHasScriptThing(CThing*) -- g_World(+0x04)->ScriptThingManager->HasScriptThing
// Leaf: ?CThing_HasScriptThing@@YI_NPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7E70, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[013B89FCh] ; mov ecx,[ecx+4] ; push eax ; call CGameScriptThingManager::HasScriptThing ; ret
// __fastcall(CThing*): the global object at 0x013B89FC holds the script-thing manager at +0x04;
// its HasScriptThing(CThing*) @ 0x00704790 answers.
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CThing;

struct CGameScriptThingManagerCalls {
    bool HasScriptThing(CThing* thing);     // 0x00704790
};

struct CScriptThingManagerOwner {
    void*                          __vftable;             // +0x00
    CGameScriptThingManagerCalls*  ScriptThingManager;    // +0x04
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

bool __fastcall CThing_HasScriptThing(CThing* thing)
{
    // The manager is fetched into a local first; retail parks `thing` in eax across the loads.
    CGameScriptThingManagerCalls* manager = reinterpret_cast<CScriptThingManagerOwner*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->ScriptThingManager;
    return manager->HasScriptThing(thing);
}