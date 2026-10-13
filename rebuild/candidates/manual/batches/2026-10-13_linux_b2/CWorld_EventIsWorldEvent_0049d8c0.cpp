// CWorld::EventIsWorldEvent(CGameEvent*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D8C0, oracle length 23 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
//
// Retail bytes:  mov eax,[esp+4] ; mov eax,[eax] ; shl eax,6 ; add eax,offset g_GameEventInfo ; cmp dword ptr [eax],0 ; setne al ; ret 4
// `this` is unused. The event's first dword is its type id, which indexes a 64-byte-per-entry
// table at 0x013B9288 (.data); an event is a "world event" when the entry's first dword (its
// world-level handler/registration pointer) is non-null. The table element type is not in the
// PDB export, so a 64-byte record with the one used member is declared here.
// engine/CGameEvent.h (PDB): +0x00 long Type.
#include "engine/CGameEvent.h"

struct CGameEventInfo {
    void*         WorldHandler;     // +0x00 non-null => handled at world level
    unsigned char _rest[0x3C];      // 64-byte stride
};

extern CGameEventInfo g_GameEventInfo[];    // 0x013B9288

struct CWorld_Methods {
    bool EventIsWorldEvent(CGameEvent* event);
};

bool CWorld_Methods::EventIsWorldEvent(CGameEvent* event)
{
    return g_GameEventInfo[event->Type].WorldHandler != 0;
}
