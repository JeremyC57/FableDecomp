// _global::for_each(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalImpulseIfInCombatAndTooClose f) -- STLport for_each: push every listed creature away from the owner when both are in combat and too close, return the functor by value
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00727500, 84 bytes, `ret 1Ch`.
// Label correction: the catalog "_For_each<CThingCreatureBase**,CApplyAdditionalImpulseIfInCombatAndTooClose>" is right;
//   the 20-byte twin 0x00726610 carries the same propagated label but uses CApplyAdditionalPlayerImpulseIfTooClose.
// Callee: CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThingCreatureBase*) @ 0x00727220 (called in place on
//   the by-value functor's argument slot).
// Layout (Ego_r.pdb, 24 bytes): C3DVector Position +0x00, float Radius +0x0c, CTCPhysicsBase* Owner +0x10,
//   CThingCreatureBase* OwnerCreature +0x14.
// Convention: /Gr (link-time __fastcall) returning a class with a user constructor: hidden result pointer in ecx
//   (ebx), first in edx, then last and the 24-byte functor on the stack (4 + 0x18 = 0x1C).
// Shape: STLport `for (; first != last; ++first) f(*first); return f;`; the return copy is the functor's implicit
//   memberwise copy over C3DVector's user copy constructor (same model as 0x00726610).
class CThingCreatureBase;
class CTCPhysicsBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

class CApplyAdditionalImpulseIfInCombatAndTooClose { public:  // 24 bytes (PDB)
    C3DVector           Position;                      // +0x00
    float               Radius;                        // +0x0c
    CTCPhysicsBase*     Owner;                         // +0x10
    CThingCreatureBase* OwnerCreature;                 // +0x14
    CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner, CThingCreatureBase* ownerCreature)
        : Position(position), Radius(radius), Owner(owner), OwnerCreature(ownerCreature) {}
    void operator()(CThingCreatureBase* other);        // 0x00727220
};

CApplyAdditionalImpulseIfInCombatAndTooClose __fastcall for_each(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalImpulseIfInCombatAndTooClose f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}