// CFilter_Bind2::operator()(const CThing& thing) const -- the thing overlaps the circle (2D distance below the summed radii) and passes the world-map filter
// Leaf: ??$?RVCThing@@@?$CFilter_Bind2@VCThingFilter_OverlappingCircle@@VCThingFilterBase@CWorldMap@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F2C0, 116 bytes (+ int3 padding), `ret 4`. No direct or absolute
// references (an unreferenced COMDAT copy).
// Identity: Ego_r.pdb `??$?RVCThing@@@?$CFilter_Bind2@VCThingFilter_OverlappingCircle@@VCThingFilterBase@CWorldMap@@@@QBE_NABVCThing@@@Z`
// (the member template operator()<CThing> of CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase>);
// the catalog label "CThingFilter_Distance_Test" is descriptive.
// Layout (Ego_r.pdb): CFilter_Bind2 { F1* F1 +0; F2* F2 +4 }; CThingFilter_OverlappingCircle { C3DVector SpherePos +0;
// float Radius +0xC }; CWorldMap::CThingFilterBase is polymorphic with a virtual `operator()(const CThing&) const`
// (`??RCThingFilterBase@CWorldMap@@UBE_NABVCThing@@@Z`, slot 0). Retail CThing PhysicsTC +0x60 (the landed 0x006A7930 /
// 0x006A4D90 models); CTCPhysicsBase::Radius +0x34 (engine/CTCPhysicsBase.h).
// Callees: CTCPhysicsBase::GetMapwhoPosition() 0x006AFCB0 (C3DVector by hidden pointer), CWorldMap::CThingFilterBase's
// virtual operator() (`call [edx]`).
// Shape: CThingFilter_OverlappingCircle::operator() inline (`__forceinline`, the landed 0x0063EF50 body: the position
// temporary bound to a helper's reference parameter, read through the returned pointer; dx / dy / the summed radius as
// named floats; the squared distance accumulated in two statements); the Bind2 returns its `&&` whole (`mov eax,1` /
// `xor eax,eax`).
struct C3DVector {
    float X;
    float Y;
    float Z;
};

class CTCPhysicsBase
{
public:
    unsigned char _pad00[0x34];
    float         Radius;                                    // +0x34 (engine/CTCPhysicsBase.h)
    C3DVector GetMapwhoPosition() const;                     // 0x006AFCB0
};

class CThing
{
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60 (retail CThing)
};

class CThingFilter_OverlappingCircle
{
public:
    C3DVector SpherePos;                                     // +0x00
    float     Radius;                                        // +0x0C
    // the landed 0x0063EF50 operator() structure: the position temporary is bound to the helper's reference parameter
    // (read through the returned pointer) and the squared distance accumulates in two statements (dx squared first).
    __forceinline bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
    {
        float dx = pos.X - SpherePos.X;
        float dy = pos.Y - SpherePos.Y;
        float r = physics->Radius + Radius;
        float d2 = dx * dx;
        d2 += dy * dy;
        if (d2 < r * r)
            return true;
        return false;
    }
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        if (physics)
            return Overlaps(physics->GetMapwhoPosition(), physics);
        return false;
    }
};

class CWorldMap
{
public:
    class CThingFilterBase
    {
    public:
        CThingFilterBase();
        virtual bool operator()(const CThing& thing) const;  // slot 0
    };
};

template <class F1, class F2> class CFilter_Bind2
{
public:
    const F1* PF1;                                           // +0x00 (PDB F1)
    const F2* PF2;                                           // +0x04 (PDB F2)
    template <class T> bool operator()(const T& thing) const
    {
        return (*PF1)(thing) && (*PF2)(thing);
    }
};

template bool CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase>::operator()(const CThing&) const;