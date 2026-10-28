// CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) -- where on the defender's combat wheel an attacker stands: the attacker's prong direction on that ring, scaled by the defender's radius plus the attacker's preferred combat distance for an inner ring, else by the middle of the ring band (half the sum of this and the previous ring's radius)
// Grade: DIFFER(344v344) -- epilogue scheduling only (same instructions, same length): retail interleaves `pop esi` after the first `fmul st(1)` of the result scaling and the else path's `mov eax,[esp+0x2c]` before the `faddp`; the lane pops esi one slot earlier and loads eax after the first `fld`. Tried: radius read in the + expression (342v344), a sequenced defender_radius local, ternary inner radius, a same-TU noinline GetRingRadius 0x006E8850 body (this fixed the x87 inner radius kept across the call), the if-form inner radius.
// Leaf: ?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E89B0, 344 bytes, thiscall (ecx = the wheel), hidden C3DVector return, `ret 0x10`.
//   One caller, 0x006EA855 (the combat manager's placement update; this = info + 0x40 = DefenderInfo.CombatWheel).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@
//   ABVCThingCreatureBase@@0J@Z (Ego_r donor 0x00711BC0): const, two CThingCreatureBase refs and a long, C3DVector by value --
//   exactly the `ret 0x10` frame; its donor neighbours GetRing 0x007115A0, GetRingRadius 0x007117B0 (retail 0x006E8850),
//   CCombatRing::GetAttackerProng 0x00711AA0/AE0 (retail 0x006E87D0, one ICF body) and GetAttackerProngVector 0x00711B20 (retail
//   0x006E8810, whose body -- GetAttackerProng, then CProng::GetProngVector(RotationAngle) or (1,0,0) -- is inlined here).
//   The manifest label CDefendingCombatantInfo_ComputeAttackVector is descriptive and wrong.
// Callees: CCombatRing::GetAttackerProng const 0x006E87D0, CProng::GetProngVector(float) const 0x006E7FD0, CThingCreatureBase::
//   GetCombatTypeDef() const 0x00666D10, CCombatWheel::GetRingRadius(long) const 0x006E8850 (same-TU noinline body).
// Inlined (PDB names, no body of their own here): CCombatWheel::GetRing(long) const (signed >= 0, then unsigned < size()),
//   CCombatRing::GetAttackerProngVector, CThing::GetPhysicsTC() const, CTCPhysicsBase::GetRadius() const, CThingCreatureBase::
//   GetPrefferedCombatDistance() const (PDB spelling; PreferredCombatDistance below 0.0001 means "unset": use the middle of the
//   front far/middle limits) and the anonymous-namespace GetPreferredCombatDistanceForAttacker(attacker, defender).
// Layout: CCombatRing (retail 0x1c) RotationAngle +0x00, DeltaRotation +0x04, Index +0x08, RingThickness +0x0C, Prongs +0x10
//   (STLport vector of 0x1c-byte CProng); CCombatWheel = { vector<CCombatRing> CombatRings }; engine/CCombatTypeDef.h
//   (DefenderWheelOuterRingStartIndex +0x48, FrontFarLimit +0x6C, FrontMiddleLimit +0x70, PreferredCombatDistance +0x80);
//   engine/CTCPhysicsBase.h (Radius +0x34); retail CThing PhysicsTC +0x60 (Ego_r +0x70 less the 0x10 the PC STLport containers
//   and CVectorMap save before it).
#include "engine/CCombatTypeDef.h"
#include "engine/CTCPhysicsBase.h"

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

class CThing {
public:
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};

class CThingCreatureBase : public CThing {
public:
    const CCombatTypeDef& GetCombatTypeDef() const;          // 0x00666D10
    float GetPrefferedCombatDistance() const
    {
        const CCombatTypeDef& def = GetCombatTypeDef();
        float distance = def.PreferredCombatDistance;
        if (distance < 0.0001f)
            distance = (def.FrontMiddleLimit + def.FrontFarLimit) * 0.5f;
        return distance;
    }
};

namespace {
inline float GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& attacker, const CThingCreatureBase& defender)
{
    float defender_radius = defender.GetPhysicsTC()->Radius;
    return defender_radius + attacker.GetPrefferedCombatDistance();
}
}   // namespace

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    const _Tp& operator[](size_type n) const { return *(_M_start + n); }
};
}   // namespace std

class CDefendingCombatantInfo {
public:
    class CProng {
    public:
        C3DVector GetProngVector(float rotation_angle) const;          // 0x006E7FD0
        unsigned char _body[0x1c];
    };
    class CCombatRing {
    public:
        float RotationAngle;                                 // +0x00
        float DeltaRotation;                                 // +0x04
        long  Index;                                         // +0x08
        float RingThickness;                                 // +0x0C
        std::vector<CProng, std::allocator<CProng> > Prongs; // +0x10
        const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const;   // 0x006E87D0
        C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const
        {
            const CProng* prong = GetAttackerProng(attacker);
            if (prong)
                return prong->GetProngVector(RotationAngle);
            return C3DVector(1.0f, 0.0f, 0.0f);
        }
    };
    class CCombatWheel {
    public:
        std::vector<CCombatRing, std::allocator<CCombatRing> > CombatRings;   // +0x00
        const CCombatRing* GetRing(long index) const
        {
            if (index >= 0 && (unsigned long)index < CombatRings.size())
                return &CombatRings[index];
            return 0;
        }
        float GetRingRadius(long index) const;                                // 0x006E8850
        C3DVector GetAttackerPlacementVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) const;
    };
};

// Same-TU body of the real retail GetRingRadius 0x006E8850 (the sum of the ring thicknesses from ring 0 out to `index`, unrolled
// by four), not inlined: with its x87 depth known, VC7.1 keeps the inner ring's radius on the FPU stack across the second call,
// as retail does (out of view, it spills it to the frame).
#pragma auto_inline(off)
__declspec(noinline) float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long index) const
{
    float radius = 0.0f;
    for (long i = index; i >= 0; --i)
        radius += GetRing(i)->RingThickness;
    return radius;
}
#pragma auto_inline(on)

C3DVector CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector(const CThingCreatureBase& attacker,
                                                                              const CThingCreatureBase& defender,
                                                                              long ring_index) const
{
    const CCombatRing* ring = GetRing(ring_index);
    C3DVector prong_vector = ring->GetAttackerProngVector(attacker);
    if (ring->Index < defender.GetCombatTypeDef().DefenderWheelOuterRingStartIndex)
        return prong_vector * GetPreferredCombatDistanceForAttacker(attacker, defender);
    float inner_radius = 0.0f;
    if (ring_index > 0)
        inner_radius = GetRingRadius(ring_index - 1);
    return prong_vector * (inner_radius + GetRingRadius(ring_index)) * 0.5f;
}
