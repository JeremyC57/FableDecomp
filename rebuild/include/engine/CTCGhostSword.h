#ifndef FABLE_ENGINE_CTCGHOSTSWORD_H
#define FABLE_ENGINE_CTCGHOSTSWORD_H
// NEW (chunk 1012, row 0x008A8260 CGameScriptInterface::GetHeroSummonedCreaturesList). Without the opt-in the header only names the class.
#ifdef FABLE_RETAIL_CTCGHOSTSWORD
// Retail opt-in: RTTI .?AVCTCGhostSword@@ : CTCBase (needs FABLE_RETAIL_CTCBASE, forced below). FableWin.pdb
// ?StatGetInterfaceType@CTCGhostSword@@SI?AW4ETCInterfaceType@@XZ (inline: TC 154 = TCI_GHOST_SWORD, `mov [esp+14h],9Ah` at 0x008A835D)
// and ?GetSummonedCreatures@CTCGhostSword@@QBE?AV?$vector@V?$CIntelligentPointer@VCThingAICreature@@@@...@std@@XZ, inline in retail:
// the caller's local copy-constructed from the member (vector copy 0x008AC540 called with [comp+10h], 0x008A837F..0x008A8387).
// Members from Ego_r.pdb CTCGhostSword: PDef +0x0C, Minions +0x10 (retail agrees at +0x10). The members after Minions are not
// modelled (pointer use only). Needs the includer's std::vector (stlport/fable_containers.h).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CSpecialAbilitiesGhostSwordDef; class CThingAICreature;
class CTCGhostSword : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_GHOST_SWORD; }
    std::vector<CIntelligentPointer<CThingAICreature> > GetSummonedCreatures() const { return Minions; }
private:
    CDefPointer<const CSpecialAbilitiesGhostSwordDef>   PDef;            // +0x0c
    std::vector<CIntelligentPointer<CThingAICreature> > Minions;         // +0x10
};
#else
class CTCGhostSword;
#endif
#endif // FABLE_ENGINE_CTCGHOSTSWORD_H
