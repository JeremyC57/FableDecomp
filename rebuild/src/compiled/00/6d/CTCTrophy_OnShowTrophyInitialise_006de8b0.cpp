// CTCTrophy::OnShowTrophyInitialise(CThing&) -- plays the trophy def's SoundShowTrophy (the result dropped; a tail jump).
// Leaf: ?OnShowTrophyInitialise@CTCTrophy@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DE8B0, 15 bytes: mov eax,[ecx+0Ch] ; add eax,4Ch ; mov [esp+4],eax ; jmp 0x006DDCD0.
// Name: Ego_r.pdb 0x0070C2C0 + FableWin.pdb 0x0249D4B0 ?OnShowTrophyInitialise@CTCTrophy@@QAEXAAVCThing@@@Z; the callee is the private
//   ?PlaySoundW@CTCTrophy@@AAEKABVCCharString@@@Z (both PDBs) with PDef->SoundShowTrophy (engine/CTrophyDef.h +0x4C).
//   NF33 (2026-11-16): moved onto engine/CTCTrophy.h + engine/CTrophyDef.h (was a stand-in `struct S` with a `Helper` callee).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"
void CTCTrophy::OnShowTrophyInitialise(CThing&)
{
    PlaySoundW(PDef->SoundShowTrophy);
}
