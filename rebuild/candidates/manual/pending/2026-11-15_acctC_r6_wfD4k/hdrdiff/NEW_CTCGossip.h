#ifndef FABLE_ENGINE_CTCGOSSIP_H
#define FABLE_ENGINE_CTCGOSSIP_H
// New header (wfD4, row 0x0090D030 CAIStateGroup_MinionGossip::Update): the gossip component (interface TCI_GOSSIP = 0xC1).
// Ego_r.pdb layout (struct_layouts_egor.tsv): 24 bytes, CTCBase + CIntelligentPointer<CThingAICreature> Gossipee +0x0C,
// bool MyTurnToSpeak +0x14. Ego_r / FableWin ??0CTCGossip@@QAE@AAVCThing@@@Z, ?StatGetInterfaceType@CTCGossip@@SI?AW4ETCInterfaceType@@XZ,
// ??$GetTC@VCTCGossip@@@CThing@@QAE_NPAPAVCTCGossip@@@Z. Retail reads +0x0C (GetPItem / SetPItem) and writes +0x14 in 0x0090D030.
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingAICreature;
class CTCGossip : public CTCBase {
public:
    CTCGossip(CThing& thing);
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_GOSSIP; }
    CIntelligentPointer<CThingAICreature> Gossipee;          // +0x0c
    bool MyTurnToSpeak;                                      // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CTCGossip) == 0x18);
#endif // FABLE_ENGINE_CTCGOSSIP_H
