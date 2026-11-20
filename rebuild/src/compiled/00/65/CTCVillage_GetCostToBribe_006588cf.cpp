// CTCVillage::GetCostToBribe() -- the village def's bribe cost (PDef->BribeCost)
// Leaf: ?GetCostToBribe@CTCVillage@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006588CF, 13 bytes: `mov eax,[ecx+94h] ; mov eax,[eax+0F8h] ; ret` (CTCVillage::PDef +0x94, then
//   CVillageDef::BribeCost, Ego_r +0x118 / retail +0xF8 with the smaller STLport containers).
// Name fix (NF195, 2026-11-20, q6328 / q6336): was the descriptive __fastcall CountReferences over local Outer / Inner structs (file
//   CPixelShader_CountReferences, catalog _global::GetMember_dword0x94_deref0xf8_006588cf). Identity: Ego_r.pdb ?GetCostToBribe@CTCVillage@@QAEJXZ
//   (name set), the body (the village def's BribeCost) and its callers: the exe has four `call 0x006588CF`, all in the minion bribe
//   code 0x0092EF7A / 0x0092EF91 / 0x009302CD / 0x00930353 (no vtable or data reference); the landed 0x0092FAB0 compares it with the
//   hero's money and formats it into the bribe text, and already binds this spelling (engine/CTCVillage.h FABLE_RETAIL_CTCVILLAGE_BRIBE).
//   Now on the shared headers (the IsStoryTime 0x00658A80 includes); PDef read in place (the by-value GetPDef() adds the reference
//   count pair: DIFFER 34v13).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_BRIBE
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#pragma optimize("s", on)

long CTCVillage::GetCostToBribe()
{
    return PDef->BribeCost;
}
