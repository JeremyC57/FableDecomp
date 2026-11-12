// CTCTavernGame::HideMesh(CThing* thing) -- hide a tavern game piece's mesh: the thing's virtual SetAsDrawable(false)
// Leaf: ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9B10: mov eax,[ecx] ; push 0 ; call [eax+70h] ; ret (static __fastcall).
// Flags: none
// Name-fix pass (2026-11-12): Ego_r.pdb 0x00647540 + FableWin.pdb ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z (public static __fastcall,
//   engine/CTCTavernGame.h HIDEMESH opt-in); the landed CTCTavernGameCardBase::ShuffleCards 0x008F10B0 calls it as HideMesh(card->Thing)
//   (0x008F11A6) and binds this symbol. Was the free function HideMesh(CMeshEntity*) (module _global) on a local CMeshEntity stand-in.
// Callees: CThing vtable slot 28 (+0x70) SetAsDrawable(bool), 0x004C8E50 (engine/CThing.h DRAWABLE opt-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCTavernGame.h"

void __fastcall CTCTavernGame::HideMesh(CThing* thing)
{
    thing->SetAsDrawable(false);
}
