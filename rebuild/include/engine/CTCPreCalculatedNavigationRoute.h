#ifndef FABLE_ENGINE_CTCPRECALCULATEDNAVIGATIONROUTE_H
#define FABLE_ENGINE_CTCPRECALCULATEDNAVIGATIONROUTE_H
#ifdef FABLE_RETAIL_CTCPRECALCULATEDNAVIGATIONROUTE_LOAD_PASS26
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
class CPersistContext;
class CThing;
// Ego_r has the target pointer at +12, the UID at +24 and the route at +32.
// OnSerialise reads all three, and FableWin 026C19F0 uses the reference overload.
class CTCPreCalculatedNavigationRoute : public CTCBase {
    CIntelligentPointer<const CThing> PThingToCalculateRouteTo;
    unsigned __int64 ThingToCalculateRouteToUID;
    std::vector<std::pair<C2DVector, unsigned char> > Route;
#ifdef FABLE_RETAIL_WAREHOUSE_MOVE_CRATES_PASS29
    void LoadRoute(CPersistContext& context, long version);
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType(){return TCI_PRE_CALCULATED_NAVIGATION_ROUTE;}
    C3DVector GetRouteDestination() const;
#else
    void LoadRoute(CPersistContext& context, long version);
#endif
};
#endif
#endif
