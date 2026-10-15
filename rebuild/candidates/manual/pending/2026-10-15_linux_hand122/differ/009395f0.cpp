// CAIStateGroup_SitRoundCampfire::GetPNearCampfire() -- nearest campfire within 22 units of the creature
// Grade: DIFFER(122v122) -- scheduling only: retail loads each position's three dwords before storing them (interleaving the next PBrain load) and the zero store after the creature load; the lane alternates load/store (filter radius now stored late as in retail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009395F0, length 122 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (x2), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestLeafThing<CIsFreeCampfire, CThingCompare_Nearest, CThingObject>(const filter&,
// const compare&, CThingObject**) @ 0x00939AF0 (manifest names the instantiation "CIsFreeHopscotchBoard":
// the template label is propagated across byte-identical instantiations; the filter here is a campfire one).
//
// Shape: the result pointer is a zero-initialised local (frame+0) handed to the search by address and
// returned afterwards. The Nearest comparator (frame+4) holds a copy of the creature's position; the filter
// (frame+0x10) holds a squared-radius float (484.0f = 22^2, stored as an immediate after the first pushes)
// and a second copy of the position; the radius is a constructor-BODY assignment so its immediate store sinks
// below the pushes as in retail. Tried: memberwise C3DVector copy constructor, POD vector, init-list radius. No engine-header members are used beyond PBrain; the base and the
// search tools are modelled locally.
#include "engine/CAIStateGroup_SitRoundCampfire.h"


struct C3DVector {                              // plain POD: copied as three dword loads, then three stores
    float X, Y, Z;
};

struct CThing {
    C3DVector* GetPosition();                   // 0x004C73D0
};

struct CThingObject;

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CThingCompare_Nearest {                  // 12 bytes: the reference position
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) {}
};

struct CIsFreeCampfire {                        // 16 bytes: squared radius, then the centre
    float RadiusSquared;
    C3DVector Centre;
    CIsFreeCampfire(const C3DVector& centre, float radiusSquared) : Centre(centre) { RadiusSquared = radiusSquared; }
};

struct CThingSearchTools {
    bool GetPBestLeafThing(const CIsFreeCampfire& filter, const CThingCompare_Nearest& compare,
                           CThingObject** result);   // 0x00939AF0
};

struct CAIStateGroup_SitRoundCampfire_Methods : CAIStateGroup_SitRoundCampfire {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_pad_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CThingObject* GetPNearCampfire();
};

CThingObject* CAIStateGroup_SitRoundCampfire_Methods::GetPNearCampfire()
{
    CThingObject* campfire = 0;
    CThingCompare_Nearest compare(*GetPBrain()->PCreature->GetPosition());
    CIsFreeCampfire filter(*GetPBrain()->PCreature->GetPosition(), 484.0f);
    PeekThingSearchTools()->GetPBestLeafThing(filter, compare, &campfire);
    return campfire;
}
