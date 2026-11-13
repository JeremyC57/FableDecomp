#ifndef FABLE_ENGINE_CCREATUREINTERACTION_H
#define FABLE_ENGINE_CCREATUREINTERACTION_H
// CCreatureInteraction (new, chunk 2023): Ego_r.pdb TPI (struct_layouts_egor.tsv) Participants
// (std::list<CThingCreatureBase*>) +0x00, Type (ECreatureInteractionType) after it; retail STLport list is 4 bytes, so Type is at
// +0x04 (the landed CAIStateGroup_SellToBuyer::IsRunnable 0x0094C010 reads +0 / +4). Ego_r.pdb / FableWin.pdb
// ?GetParticipants@CCreatureInteraction@@QBEABV?$list@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@XZ and
// ?GetType@CCreatureInteraction@@QBE?AW4ECreatureInteractionType@@XZ have no Fable.exe copy (inline). Only these members are modelled.
#ifndef FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST
#endif
#include "stlport/fable_containers.h"
class CThingCreatureBase;
enum ECreatureInteractionType;                                           // MS forward enum
class CCreatureInteraction {
public:
    const std::list<CThingCreatureBase*>& GetParticipants() const { return Participants; }
    ECreatureInteractionType GetType() const { return Type; }
private:
    std::list<CThingCreatureBase*> Participants;                         // +0x00
    ECreatureInteractionType Type;                                       // +0x04
};
#endif
