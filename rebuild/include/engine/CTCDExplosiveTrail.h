#ifndef FABLE_ENGINE_CTCD_EXPLOSIVE_TRAIL_H
#define FABLE_ENGINE_CTCD_EXPLOSIVE_TRAIL_H
#ifdef FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CExplosiveTrailDef;
// Layout: Ego_r and FableWin name these members. Native AddThingToIgnore
// uses the vector at +0x20 after the four-byte STLport list at +0x1c.
class CTCDExplosiveTrail : public CTCBase {
public:
    virtual void FrameUpdate();
private:
    CIntelligentPointer<const CThing> PTarget;
    long Timer;
    bool Terminated;
    std::list<std::pair<CIntelligentPointer<CThing>,long> > ComponentTraps;
    std::vector<CIntelligentPointer<CThing> > ThingsToIgnore;
    CDefPointer<const CExplosiveTrailDef> PDef;
};
#endif
#endif
