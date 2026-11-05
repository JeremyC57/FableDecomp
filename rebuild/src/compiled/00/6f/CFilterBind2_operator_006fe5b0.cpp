// CFilter_Bind2::operator()(thing) -- Require both the first predicate and sphere overlap.
// Leaf: ??$?RVCThingGameObject@@@?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@QBE_NABVCThingGameObject@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
#pragma pack(push,1)
#include "engine/C3DVector.h"
#pragma pack(pop)
class CTCPhysicsBase {public:void GetBoundingInfo(C3DVector&,float&)const;};
// Layout: retail CThing PhysicsTC is+0x60; the available donor CThing header is quarantined.
class CThing {public:unsigned char _body[0x60];CTCPhysicsBase* PhysicsTC;};
class CThingGameObject : public CThing {};
class CUnprovenThing_00877eb0 : public CThing {};
class CUnprovenThing_00877f40 : public CThing {};
class CThingFilter_MinAllowedRadius {public:float MinRadius;bool operator()(const CThing&)const;};
class CThingFilter_IsOfType {public:unsigned long Type;bool operator()(const CThing&)const;};
class CThingFilter_OverlappingSphere {
public:
    C3DVector SpherePos;float Radius;
    __forceinline bool operator()(const CThing& thing)const {
        C3DVector center;float radius;
        thing.PhysicsTC->GetBoundingInfo(center,radius);
        C3DVector delta=center-SpherePos;
        float sum=radius+Radius;
        return delta.GetSquaredMagnitude()<sum*sum;
    }
};
template<class A,class B> class CFilter_Bind2 {
public:
    const A* F1;const B* F2;
    template<class T> bool operator()(const T& thing)const {return (*F1)(thing) && (*F2)(thing);}
};
template bool CFilter_Bind2<CThingFilter_MinAllowedRadius,CThingFilter_OverlappingSphere>::operator()<CThingGameObject>(const CThingGameObject&)const;