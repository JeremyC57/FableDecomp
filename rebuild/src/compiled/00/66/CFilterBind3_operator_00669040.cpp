// CFilter_Bind3::operator()(const CThingPhysical& thing) -- a physical thing passes when it is not the source thing, both physics components collide with things, and its map-who footprint circle overlaps the filter circle
// Leaf: ??$?RVCThingPhysical@@@?$CFilter_Bind3@VCThingFilter_SolidToThing@@VCThingFilter_PhysicsFlagsSolidToThing@@VCThingFilter_OverlappingCircle@@@@QBE_NABVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00669040, 183 bytes (+ int3 padding), thiscall `ret 4`, bool in eax.
// Full name (Ego_r.pdb name set): CFilter_Bind3<CThingFilter_SolidToThing, CThingFilter_PhysicsFlagsSolidToThing,
//   CThingFilter_OverlappingCircle>::operator()<CThingPhysical>(const CThingPhysical&) const. No call or address
//   reference reaches it in retail (an out-of-line instance).
// Callees: CTCPhysicsBase::GetMapwhoPosition() const @ 0x006AFCB0 (landed; C3DVector through the hidden pointer, the
//   12-byte frame local), CTCPhysicsBase virtual GetCollidesWithThings (slot 57, +0xe4) / GetCollidesWithThingsOneWay
//   (slot 58, +0xe8) as named in the landed 0x006A8370.
// Label correction: the catalog "CThingFilter_PhysicsDistance_Test" is descriptive.
// Layout (Ego_r.pdb): CFilter_Bind3 { F1 +0, F2 +4, F3 +8 }; CThingFilter_SolidToThing { SourceThing +0,
//   PTCSolidToThing +4 }; CThingFilter_PhysicsFlagsSolidToThing { SourceThing +0, SourceThingPhysicsTC +4 };
//   CThingFilter_OverlappingCircle { SpherePos +0, Radius +0xc }; retail CThing PhysicsTC +0x60, physics radius +0x34
//   (the landed 0x0063EF50 model).
// Shape: `return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);` (the `&&` value gives the `mov eax,1` / `xor eax,eax`
//   exits); the three filter operators are __forceinline (plain inline leaves calls). The SolidToThing test reads only SourceThing in retail (PTCSolidToThing is never loaded). The flags
//   test reads the thing's component into a local before the null tests, then `a || b` on the source component. The
//   circle test is the landed 0x0063EF50 body (dx squared first needs the two-statement `d2`). No pragma.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
class CThing;
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
    unsigned char _pad04[0x34 - 0x04];
    float         Radius;                               // +0x34 (the landed 0x0063EF50 m_radius)
    C3DVector GetMapwhoPosition() const;                // 0x006AFCB0
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
class CThingPhysical : public CThing { };
class CTCSolidToThing;

class CThingFilter_SolidToThing {
public:
    CThing*          SourceThing;               // +0x00
    CTCSolidToThing* PTCSolidToThing;           // +0x04
    __forceinline bool operator()(const CThing& thing) const
    {
        return &thing != SourceThing;
    }
};
class CThingFilter_PhysicsFlagsSolidToThing {
public:
    CThing*         SourceThing;                // +0x00
    CTCPhysicsBase* SourceThingPhysicsTC;       // +0x04
    __forceinline bool operator()(const CThing& thing) const
    {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (!SourceThingPhysicsTC || !physics)
            return false;
        if (!(SourceThingPhysicsTC->GetCollidesWithThings() || SourceThingPhysicsTC->GetCollidesWithThingsOneWay()))
            return false;
        return physics->GetCollidesWithThings();
    }
};
class CThingFilter_OverlappingCircle {
public:
    C3DVector SpherePos;                        // +0x00
    float     Radius;                           // +0x0c
    bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
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
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (physics)
            return Overlaps(physics->GetMapwhoPosition(), physics);
        return false;
    }
};

template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A* F1;                                // +0x00
    const B* F2;                                // +0x04
    const C* F3;                                // +0x08
    template <class T> bool operator()(const T& thing) const
    {
        return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);
    }
};

template bool CFilter_Bind3<CThingFilter_SolidToThing, CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>::operator()<CThingPhysical>(const CThingPhysical&) const;