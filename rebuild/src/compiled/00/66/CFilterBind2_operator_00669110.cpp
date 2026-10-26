// CFilter_Bind2::operator()(const CThingBuilding& thing) -- a building passes when its physics bounding sphere overlaps the sphere filter and the position filter says it contains the point
// Leaf: ??$?RVCThingBuilding@@@?$CFilter_Bind2@VCThingFilter_OverlappingSphere@@VCThingFilter_PosInContainmentVolume@@@@QBE_NABVCThingBuilding@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00669110, 134 bytes (+ int3 padding), thiscall `ret 4`, bool in eax.
// Full name (Ego_r.pdb name set): CFilter_Bind2<CThingFilter_OverlappingSphere, CThingFilter_PosInContainmentVolume>
//   ::operator()<CThingBuilding>(const CThingBuilding&) const -- `??$?RVCThingBuilding@@@?$CFilter_Bind2@...`.
//   No call or address reference reaches it in retail (an out-of-line instance; GetPFirstLeafThing 0x006698D0 inlines it).
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640 (landed),
//   CThingFilter_PosInContainmentVolume::operator() @ 0x006A7B20 (landed, out of line).
// Label correction: the catalog "CThingFilter_BoundingBox_Test" is descriptive.
// Layout: Ego_r.pdb CFilter_Bind2 { F1 +0, F2 +4 } (filter pointers); CThingFilter_OverlappingSphere { SpherePos +0,
//   Radius +0xc }; retail CThing PhysicsTC +0x60.
// Shape: `return (*F1)(thing) && (*F2)(thing);` (the `&&` value gives the `mov eax,1` / `xor eax,eax` exits) over the landed
//   0x006689E0 sphere body made __forceinline (plain inline stays a call): centre in a frame local, the bound radius homed in the dead argument slot, the
//   squares summed as one expression (VC squares Z, Y, X). No pragma.
class C3DVector { public:
    float X, Y, Z;
};
class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
class CThingBuilding : public CThing { };

class CThingFilter_OverlappingSphere {
public:
    C3DVector SpherePos;                        // +0x00
    float     Radius;                           // +0x0c
    __forceinline bool operator()(const CThing& thing) const
    {
        C3DVector centre;
        float boundRadius;
        thing.PhysicsTC->GetBoundingInfo(&centre, &boundRadius);
        float dx = centre.X - SpherePos.X;
        float dy = centre.Y - SpherePos.Y;
        float dz = centre.Z - SpherePos.Z;
        float r = boundRadius + Radius;
        if (dx * dx + dy * dy + dz * dz < r * r)
            return true;
        return false;
    }
};
class CThingFilter_PosInContainmentVolume {
public:
    C3DVector Pos;                              // +0x00
    bool operator()(const CThing& thing) const; // 0x006A7B20
};

template <class A, class B> class CFilter_Bind2 {
public:
    const A* F1;                                // +0x00
    const B* F2;                                // +0x04
    template <class T> bool operator()(const T& thing) const
    {
        return (*F1)(thing) && (*F2)(thing);
    }
};

template bool CFilter_Bind2<CThingFilter_OverlappingSphere, CThingFilter_PosInContainmentVolume>::operator()<CThingBuilding>(const CThingBuilding&) const;