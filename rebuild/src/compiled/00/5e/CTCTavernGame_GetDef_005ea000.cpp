// CTCTavernGame::GetDef(CDefPointer<const CTavernGameDef>* def) -- fetch the tavern game's CTavernGameDef sub-def of the owner's base def
// Leaf: ?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EA000, 11 bytes: `mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ; jmp 0x005ED4A0`
//   (ParentThing->PBaseDef->GetSubDef(def), the result discarded: a tail call).
// Identity (two sources): Ego_r.pdb ?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z (0x00647980, the class's only def
//   getter; engine/CTCTavernGame.h already spells it), and the jump target 0x005ED4A0 is the landed
//   ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z on ParentThing (+0x04) -> PBaseDef (+0x70);
//   its 55 callers are the derived tavern games (CTCTavernGameShoveHaPenny::EnterGame / GetBestScore ..., CTCTavernGameCoinGolf ...), which
//   pass a local CDefPointer<const CTavernGameDef>. Retail keeps this out-of-line copy for the other TUs; the CTCTavernGame TU's own
//   FrameUpdate expands it. The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label; landed callers bind it as the invented
//   ?GetTavernGameDef@CTCTavernGame@@QAEXPAUCDefPointer@@@Z (NAME_FIXES.tsv).
// Callees: ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z 0x005ED4A0 (landed).
// Header: engine/CTCTavernGame.h (FABLE_RETAIL_CTCTAVERNGAME + FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#include "engine/CTCTavernGame.h"
void CTCTavernGame::GetDef(CDefPointer<const CTavernGameDef>* def)
{
    ParentThing->PeekBaseDef().GetSubDef(def);
}