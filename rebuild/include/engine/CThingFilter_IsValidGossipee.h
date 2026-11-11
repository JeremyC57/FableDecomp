#ifndef FABLE_ENGINE_CTHINGFILTER_ISVALIDGOSSIPEE_H
#define FABLE_ENGINE_CTHINGFILTER_ISVALIDGOSSIPEE_H
// New header (chunk Q15, row 0x0090C900). Ego_r.pdb CThingFilter_IsValidGossipee (12 bytes, retail = PDB: no containers):
// Thing +0, PFaction +4, PTCVillage +8. Members: ??0CThingFilter_IsValidGossipee@@QAE@ABVCThingAICreature@@@Z (0x0090C870) and
// ??RCThingFilter_IsValidGossipee@@QBE_NABVCThingAICreature@@@Z (0x0090C900). The PDB layout dump drops const; PFaction takes
// CTCEnemy::GetPFaction's const.
#include "rebuild_abi.h"
class CThingAICreature;
class CFaction;
class CTCVillage;
class CThingFilter_IsValidGossipee {
public:
    CThingFilter_IsValidGossipee(const CThingAICreature& thing);        // 0x0090C870
    bool operator()(const CThingAICreature& thing) const;               // 0x0090C900
private:
    const CThingAICreature* Thing;                                      // +0x00
    const CFaction*         PFaction;                                   // +0x04
    CTCVillage*             PTCVillage;                                 // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsValidGossipee) == 12);
#endif
