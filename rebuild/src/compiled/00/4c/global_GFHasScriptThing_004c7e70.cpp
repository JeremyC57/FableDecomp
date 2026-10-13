// GFHasScriptThing(CThing*) -- g_World(+0x04)->ScriptThingManager->HasScriptThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7E70, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[013B89FCh] ; mov ecx,[ecx+4] ; push eax ; call CGameScriptThingManager::HasScriptThing ; ret
// __fastcall(CThing*): the global object at 0x013B89FC holds the script-thing manager at +0x04;
// its HasScriptThing(CThing*) @ 0x00704790 answers.
struct CThing;

struct CGameScriptThingManagerCalls {
    bool HasScriptThing(CThing* thing);     // 0x00704790
};

struct CScriptThingManagerOwner {
    void*                          __vftable;             // +0x00
    CGameScriptThingManagerCalls*  ScriptThingManager;    // +0x04
};

extern CScriptThingManagerOwner* g_ScriptThingManagerOwner;   // 0x013B89FC (.data)

bool __fastcall GFHasScriptThing(CThing* thing)
{
    // The manager is fetched into a local first; retail parks `thing` in eax across the loads.
    CGameScriptThingManagerCalls* manager = g_ScriptThingManagerOwner->ScriptThingManager;
    return manager->HasScriptThing(thing);
}