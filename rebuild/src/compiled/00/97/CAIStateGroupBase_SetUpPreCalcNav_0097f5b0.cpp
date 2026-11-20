// CAIStateGroupBase::SetUpPreCalcNav(const CThing* routeThing) -- when the creature navigates (interface 7) and the route thing's precalculated route (0xA6) has a route, set the navigation up from the creature's position along it and return the route's destination; else the zero vector
// Leaf: ?SetUpPreCalcNav@CAIStateGroupBase@@IAE?AVC3DVector@@ABVCThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F5B0, length 219 bytes (`ret 8`, C3DVector by hidden pointer).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 7) and the const copy @ 0x004365B0
// (type 0xA6 on the const route thing), CTCPreCalculatedNavigationRoute::HasRoute @ 0x00800500 and ::GetRouteDestination
// @ 0x008004D0 (twice), CThing::GetPos @ 0x004C73D0, the navigation component's route setup @ 0x006CB9C0 (three
// arguments; manifest "CScriptThing::operator==" is propagated).
// Shape: the route component is deliberately unset when the interface is absent (retail reads a dead slot), the
// navigation component is kept in ebx; the creature is re-read into a named local before the setup call (retail loads
// it ahead of the argument calls, reusing the `this` register).
// Callee respelled (NF154, PRIORITY q4534): 0x006CB9C0 is ?InitialiseFromPreCalculatedRoute@CTCCreatureNavigation@@QAEXABVC3DVector@@ABV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@0@Z
//   (Ego_r.pdb 0x00701440 / FableWin.pdb 0x0247B2A0; PNavigator slot 15 forward; engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION);
//   was the stand-in ?SetUpRoute@CTCNavigation@@QAEXABVC3DVector@@00@Z. Its middle argument is the route component's Route +0x20 (Ego_r.pdb
//   CTCPreCalculatedNavigationRoute::Route, a 12-byte STLport vector<pair<C2DVector, unsigned char> >): the local model's +0x20 member is
//   respelled in place from the stand-in C3DVector RouteStart.
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"

class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPreCalculatedNavigationRoute              // interface type 0xA6
{ public:
    unsigned char _pad00[0x20];
    std::vector<std::pair<C2DVector, unsigned char> > Route;   // +0x20
    bool HasRoute();                                // 0x00800500
    C3DVector GetRouteDestination();                // 0x008004D0
};

#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CTCCreatureNavigation.h"

class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

class CAIStateGroupBase
{ public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    protected: C3DVector SetUpPreCalcNav(const CThing& routeThing); public:   // Ego_r.pdb 0x0084A7A0 + FableWin.pdb 0x02890F20 (protected, by reference)
};

C3DVector CAIStateGroupBase::SetUpPreCalcNav(const CThing& routeThing)
{
    CThing* creature = PBrain->Creature;
    if (creature->HasTC(TCI_NAVIGATION))
    {
        CTCCreatureNavigation* nav = (CTCCreatureNavigation*)((CTCCreatureNavigation*)&creature->GetTC(TCI_NAVIGATION));
        CTCPreCalculatedNavigationRoute* route;
        if (routeThing.HasTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE))
            route = (CTCPreCalculatedNavigationRoute*)((CTCPreCalculatedNavigationRoute*)&routeThing.GetTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE));
        if (route->HasRoute())
        {
            CThing* mover = PBrain->Creature;
            nav->InitialiseFromPreCalculatedRoute(mover->GetPos(), route->Route, route->GetRouteDestination());
            return route->GetRouteDestination();
        }
    }
    return C3DVector(0.0f, 0.0f, 0.0f);
}