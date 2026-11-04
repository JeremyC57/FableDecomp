// COpinionReactionManager::SetThingOpinionSource(CThing* thing) -- make a thing the (exclusive) opinion source
// Leaf: ?SetThingOpinionSource@COpinionReactionManager@@QAEXPAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00700770, 11 bytes: `add ecx,714h ; jmp 0x00A01B90` (PThingOpinionSource.SetPItem(thing), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetThingOpinionSource@COpinionReactionManager@@QAEXPAVCThing@@@Z at Ego_r 0x00550510,
//   right before AttachOpinionSource 0x00550640, as retail 0x00700770 precedes the landed AttachOpinionSource 0x00700780; it sets the
//   CIntelligentPointer<CThing> PThingOpinionSource (Ego_r +0x71C, retail +0x714 after the 4-byte STLport Reactions list), which its
//   only caller CGameScriptInterface::OpinionSourceSetAsExclusive 0x00894E8B / 0x00894EB2 also reads at +0x714 (with GetPItem) before
//   clearing it (push 0). The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z 0x00A01B90.
// Header: engine/COpinionReactionManager.h (new), engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/COpinionReactionManager.h"
void COpinionReactionManager::SetThingOpinionSource(CThing* thing)
{
    PThingOpinionSource.SetPItem(thing);
}