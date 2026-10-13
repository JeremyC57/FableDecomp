// CWorld::GetChecksum()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049E200, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[ecx+50h] ; call CThingManager::GetNonLocalThingChecksum ; mov ecx,[013B89BCh] ; add eax,ecx ; ret
// engine/CWorld.h (PDB): +0x50 scoped_ptr<CThingManager> PThingManager. The world checksum is the
// thing manager's non-local-thing checksum plus a global salt in .data (0x013B89BC).
#include "engine/CWorld.h"

extern unsigned long g_WorldChecksumSalt;   // 0x013B89BC (.data; name from use)

struct CThingManagerCalls {
    unsigned long GetNonLocalThingChecksum();   // 0x0051E530
};

struct CWorld_Methods : CWorld {
    unsigned long GetChecksum();
};

unsigned long CWorld_Methods::GetChecksum()
{
    // Two named locals: a one-expression sum folds the salt into `add eax,[salt]` (15 bytes).
    unsigned long checksum = reinterpret_cast<CThingManagerCalls*>(this->PThingManager_ptr)->GetNonLocalThingChecksum();
    unsigned long salt = g_WorldChecksumSalt;
    return checksum + salt;
}