// _global::for_each(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalPlayerImpulseIfTooClose f) -- STLport for_each: push every listed creature away from the player when too close, return the functor by value
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00726610, 77 bytes (80 with padding), `ret 18h`.
// Label correction: the catalog "_For_each<CThingCreatureBase**,CApplyAdditionalImpulseIfInCombatAndTooClose>" is
//   propagated from the 24-byte twin 0x00727500; this body calls CApplyAdditionalPlayerImpulseIfTooClose::operator()
//   (landed 0x00726340) and copies a 20-byte functor (Ego_r.pdb: Position, Radius, Owner).
// Callee: CApplyAdditionalPlayerImpulseIfTooClose::operator()(CThingCreatureBase*) @ 0x00726340 (called in place on
//   the by-value functor's argument slot).
// Convention: /Gr (link-time __fastcall) returning a class with a user constructor: hidden result pointer in ecx
//   (ebx), first in edx, then last and the 20-byte functor on the stack (4 + 0x14 = 0x18).
// Shape: STLport `for (; first != last; ++first) f(*first); return f;` -- the return copy is the functor's implicit
//   MEMBERWISE copy (C3DVector has a user copy constructor, as in the landed 0x007FA290 comparator model): the three
//   position words are loaded into ecx/edx/eax before the first store, then Radius and Owner rotate through ecx/edx;
//   with a trivially copyable C3DVector VC7.1 emits a block copy through ecx instead (79 v 77 bytes).
class CThingCreatureBase;
class CTCPhysicsBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

class CApplyAdditionalPlayerImpulseIfTooClose { public:    // 20 bytes (PDB)
    C3DVector       Position;                       // +0x00
    float           Radius;                         // +0x0c
    CTCPhysicsBase* Owner;                          // +0x10
    CApplyAdditionalPlayerImpulseIfTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner)
        : Position(position), Radius(radius), Owner(owner) {}
    void operator()(CThingCreatureBase* other);     // 0x00726340
};

CApplyAdditionalPlayerImpulseIfTooClose __fastcall for_each(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalPlayerImpulseIfTooClose f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}