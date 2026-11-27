#ifndef FABLE_ENGINE_CTCAVOIDREGIONEXIT_H
#define FABLE_ENGINE_CTCAVOIDREGIONEXIT_H
#ifdef FABLE_RETAIL_AVOID_REGION_FRAME_PASS29
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_map.h"
class CTCAvoidRegionExit : public CTCBase {
public:
    virtual void FrameUpdate();
    void UpdateMarkers();
    void UpdateBestDist();
private:
    CIntelligentPointer<CThing> EntranceMarker, ExitMarker;
    unsigned long NextFrameCheck;
    static bool MarkersAcquired;
    static long FrameLastUpdated;
    static std::list<CIntelligentPointer<CThing> > EntranceMarkers, ExitMarkers;
    static std::map<CThing*,float> DistMap;
};
FABLE_STATIC_ASSERT(sizeof(CTCAvoidRegionExit)==32);
#endif
#endif
