// CFilter_Bind3::operator()(const CThing& thing) -- a thing passes when it can collide or interact with the searcher, its physics bounding sphere overlaps the filter sphere, and it has the wanted TC
// Leaf: ??$?RVCThing@@@?$CFilter_Bind3@
// Retail 0x008E6030, 0x92 bytes (+int3 padding), thiscall `ret 4`, int-widened bool (`mov eax,1` / `xor eax,eax`).
//   No call or address reference reaches it in retail (an out-of-line COMDAT instance, like the landed 0x0085B160 Bind4).
//   Full name (FableWin PDB name set): `??$?RVCThing@@@?$CFilter_Bind3@VCThingFilter_CanCollideOrInteractWithThing@@
//   VCThingFilter_OverlappingSphere@@VCThingFilter_HasTC@@@@QBE_NABVCThing@@@Z`, the filter of the neighbouring
//   CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind3<...> > (landed 0x008E60D0).
//   Manifest label "CollisionFilter_SphereTest" is descriptive.
// Callees: CThingFilter_CanCollideOrInteractWithThing::operator() 0x006A8370 (landed), CTCPhysicsBase::GetBoundingInfo
//   (C3DVector&, float&) const 0x006AF640 (landed), CThingFilter_HasTC::operator() 0x006A80A0 (landed).
// Layout (Ego_r.pdb): CFilter_Bind3 { F1* +0, F2* +4, F3* +8 }; CThingFilter_OverlappingSphere { C3DVector SpherePos +0,
//   float Radius +0xC }; retail CThing PhysicsTC +0x60 (the landed 0x006689E0 model).
// Shape: `return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);` with the overlapping-sphere operator __forceinline (plain
//   in-class `inline` stays an out-of-line call); its body is the landed 0x006689E0 one, so the bounding radius lands in
//   the dead thing-argument slot and the sum of squares comes out z, y, x.
class C3DVector {
public:
    float X, Y, Z;
};
class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
class CThingFilter_CanCollideOrInteractWithThing {
public:
    CTCPhysicsBase* TCPhysics;                          // +0x00
    bool ExcludeCreatures;                              // +0x04
    bool operator()(const CThing& thing) const;         // 0x006A8370
};
class CThingFilter_HasTC {
public:
    long Type;                                          // +0x00 ETCInterfaceType
    bool operator()(const CThing& thing) const;         // 0x006A80A0
};
class CThingFilter_OverlappingSphere {
public:
    C3DVector SpherePos;                                // +0x00
    float     Radius;                                   // +0x0C
    __forceinline bool operator()(const CThing& thing) const
    {
        C3DVector centre;
        float boundRadius;
        thing.PhysicsTC->GetBoundingInfo(centre, boundRadius);
        float dx = centre.X - SpherePos.X;
        float dy = centre.Y - SpherePos.Y;
        float dz = centre.Z - SpherePos.Z;
        float r = boundRadius + Radius;
        if (dx * dx + dy * dy + dz * dz < r * r)
            return true;
        return false;
    }
};
template <class F1, class F2, class F3> class CFilter_Bind3 {
public:
    const F1* PF1;                                      // +0x00
    const F2* PF2;                                      // +0x04
    const F3* PF3;                                      // +0x08
    template <class T> bool operator()(const T& thing) const
    {
        return (*PF1)(thing) && (*PF2)(thing) && (*PF3)(thing);
    }
};
template bool CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing, CThingFilter_OverlappingSphere,
                            CThingFilter_HasTC>::operator()<CThing>(const CThing&) const;