// CFilter_Bind2::operator()(thing) -- Require both the first predicate and sphere overlap.
// Leaf: ??$?RVCThingGameObject@@@?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@QBE_NABVCThingGameObject@@@Z
#pragma pack(push,1)
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector operator-(const C3DVector& rhs) const {return C3DVector(X-rhs.X,Y-rhs.Y,Z-rhs.Z);}
    float GetLengthSquared()const{return X*X+(Y*Y+Z*Z);}
};
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
        return delta.GetLengthSquared()<sum*sum;
    }
};
template<class A,class B> class CFilter_Bind2 {
public:
    const A* F1;const B* F2;
    template<class T> bool operator()(const T& thing)const {return (*F1)(thing) && (*F2)(thing);}
};
template bool CFilter_Bind2<CThingFilter_MinAllowedRadius,CThingFilter_OverlappingSphere>::operator()<CThingGameObject>(const CThingGameObject&)const;