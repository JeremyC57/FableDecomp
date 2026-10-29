// CTCCoopSpirit::UpdateHeightAboveGround() -- push the spirit's physics up or down towards its hover height above the ground below it, with an acceleration that grows as exp(2.25 d^2) - 1 (capped at 1) in the gap d
// Grade: RELOCATION_MATCH (agent902, 3rd grade) -- the sign as a named float local (`float direction = (float)sign;`) and the clamped term as a float `e`: VC7.1 keeps both on the x87 stack (fild early, popped at scope end), stores the gap with `fst` and re-reads it for the square, compares e against the dword 1.0f, as retail.
// Leaf: ?UpdateHeightAboveGround@CTCCoopSpirit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670470, 228 bytes (+ int3 padding to 240), thiscall `ret`. Name: Ego_r.pdb
//   `?UpdateHeightAboveGround@CTCCoopSpirit@@AAEXXZ` (private void; the only caller 0x00670A6E is the spirit's update
//   sequence after UpdateAppliedMovement-style members; the landed CTCCoopSpirit::UpdateAppliedMovement is 0x006702E0).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (interface 2, the physics component),
//   CThingCreatureBase::GetHeightZBelowAt(const C3DVector&, bool, bool) const @ 0x00666D40 (PDB name set; pos, false,
//   true), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (landed). Floats: 0.2f @ 0x012305A0 (the
//   hover height offset), 0.0f @ 0x0122DEDC, 2.25f @ 0x0125B2D4, 1.0f @ 0x0122DED8, 1.0 (double) @ 0x01237F00.
// Label correction: the catalog "CThingCreatureBase_UpdateGravity" is descriptive.
// Layout: CTCBase ParentThing +4; retail CThing interface bitset +0x20, map +0x44; CTCPhysicsBase Position +0x0c.
// Tried (chunk agent): sign/clamp as statements, a double clamp helper, if/else sign + float-limit helper, the sign
//   ternary inline as the left operand (all 222-224v228).
// Shape: the landed GetTCIfPresent (the component local deliberately unset when absent: retail reads the key's dead
//   slot); `float diff = height + 0.2f - pos.Z;` `long sign = diff < 0.0f ? -1 : 1;` `float direction = (float)sign;`
//   `float e = exp(diff * diff * 2.25f) - 1.0;` then `direction * (e > 1.0f ? 1.0f : e)` -- the two named float locals
//   take the x87 registers (direction fild'ed right after the sign branch and popped at scope end, e compared against the
//   dword 1.0f), so VC7.1 homes diff with `fst` before the compare and re-reads it for the square; intrinsic exp (fldl2e /
//   f2xm1 / fscale); the acceleration is a C3DVector(0, 0, z) temporary. No pragma.
#include <math.h>
#pragma intrinsic(exp)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase {
public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                 // +0x0c
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                        // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
class CThingCreatureBase : public CThing {
public:
    float GetHeightZBelowAt(const C3DVector& pos, bool a, bool b) const;   // 0x00666D40
};
class CTCCoopSpirit {
public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                        // +0x04 (CTCBase)
    float GetHeightOffset() const { return 0.2f; }
private:
    void UpdateHeightAboveGround();                         // Ego_r.pdb ?UpdateHeightAboveGround@CTCCoopSpirit@@AAEXXZ (private)
};

void CTCCoopSpirit::UpdateHeightAboveGround()
{
    CThingCreatureBase* creature = ParentThing;
    CTCPhysicsBase* physics = (CTCPhysicsBase*)creature->GetTCIfPresent(2);
    const C3DVector& pos = physics->Position;
    float diff = creature->GetHeightZBelowAt(pos, false, true) + GetHeightOffset() - pos.Z;
    long sign = diff < 0.0f ? -1 : 1;
    float direction = (float)sign;
    float e = (float)(exp(diff * diff * 2.25f) - 1.0);
    physics->ApplyAcceleration(C3DVector(0.0f, 0.0f, direction * (e > 1.0f ? 1.0f : e)));
}