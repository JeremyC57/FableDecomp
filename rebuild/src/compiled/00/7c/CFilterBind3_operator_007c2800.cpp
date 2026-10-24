// CFilter_Bind3::operator()(const CThing& thing) -- a thing is a shot-collision candidate when it is not the excluded thing, it does not evade all hits, its physics interacts or collides with things, it is a creature/object or is driven by a type-0x69 driver, and it is not the source thing
// Leaf: ??$?RVCThing@@@?$CFilter_Bind3@VCThingFilter_IsNot@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C2800, 128 bytes, thiscall `ret 4`, bool in eax.
// Full name (Ego_r.pdb name set): CFilter_Bind3<CThingFilter_IsNot, (anonymous namespace)::CThingFilter_
//   ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>::operator()<CThing>(const CThing&) const. No call or
//   address reference reaches it in retail (an out-of-line instance); GetAllThingsInRoughArea<CThing, vector<CThing*>,
//   this Bind3> @ 0x007C2D10 inlines the same body (its call to 0x0085FA00 at 0x007C2E95).
// Callees: CThingFilter_IsNot::operator() @ 0x006A76A0, NCreatureMode::IsToEvadeAllHits(const CThing&) __fastcall @
//   0x0085FA00 (as named by the landed 0x0080CA20), CThingFilter_SolidToThing::operator() @ 0x00661640 (out of line
//   here; it reads only SourceThing, the landed 0x00669040 model), CTCPhysicsBase virtuals GetCollidesWithThings
//   (slot 57, +0xe4) / GetInteractsWithThings (slot 60, +0xf0), the driver's GetInterfaceType (CTCBase slot 21, +0x54).
// Label correction: the catalog "CThingFilter_Pred_IsNot_IsEvading" is descriptive.
// Layout (Ego_r.pdb): CFilter_Bind3 { F1 +0, F2 +4, F3 +8 } (filter pointers; the stateless F2 is never loaded).
//   Retail CThing (PDB - 0x10): PTCDriver +0x5c, PhysicsTC +0x60, ThingTypeFlags +0x6c.
// Shape: `return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);` (the `&&` value gives the `mov eax,1` / `xor eax,eax`
//   exits); the shots filter is __forceinline and reads the type flags and the physics component into locals before
//   the evade call.
class CThing;

class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual long GetInterfaceType() const;              // slot 21 (+0x54)
};
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
    virtual void v59();
    virtual bool GetInteractsWithThings() const;        // slot 60 (+0xf0)
};
class CThing {
public:
    unsigned char   _pad00[0x5c];
    CTCBase*        PTCDriver;                  // +0x5c CTCDriverBase*
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x6c - 0x64];
    unsigned long   ThingTypeFlags;             // +0x6c
};

namespace NCreatureMode {
    bool __fastcall IsToEvadeAllHits(const CThing& thing);   // 0x0085FA00
}

class CThingFilter_IsNot {
public:
    CThing* Thing;                              // +0x00
    bool operator()(const CThing& thing) const; // 0x006A76A0
};
class CThingFilter_SolidToThing {
public:
    CThing*       SourceThing;                  // +0x00
    void*         PTCSolidToThing;              // +0x04
    bool operator()(const CThing& thing) const; // 0x00661640
};
namespace {
class CThingFilter_ShotsToCheckForCollisionWith {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        unsigned long flags = thing.ThingTypeFlags;
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (NCreatureMode::IsToEvadeAllHits(thing))
            return false;
        if (!physics)
            return false;
        if (!physics->GetInteractsWithThings() && !physics->GetCollidesWithThings())
            return false;
        if (flags & 0xC)
            return true;
        CTCBase* driver = thing.PTCDriver;
        return driver && driver->GetInterfaceType() == 0x69;
    }
};
}

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

template bool CFilter_Bind3<CThingFilter_IsNot, CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>::operator()<CThing>(const CThing&) const;