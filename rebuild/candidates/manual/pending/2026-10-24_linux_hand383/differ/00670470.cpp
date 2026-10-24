// Grade: DIFFER(222v228) -- x87 only: retail stores the float gap (`fst dword [esp+10h]`, frame 14h v 10h), loads the int sign first (`fild`) and re-reads the gap from memory for the square; the lane keeps the gap on the x87 stack across the sign branch and loads the sign after the clamp; with the clamp in a double helper the lane also compares/loads 1.0 as a qword (retail dword). Interface lookup, the call sequence, the exp sequence and the vector stores match.
// CTCCoopSpirit::UpdateHeightAboveGround() -- push the spirit's physics up or down towards its hover height above the ground below it, with an acceleration that grows as exp(2.25 d^2) - 1 (capped at 1) in the gap d
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
// tried: sign/clamp as statements (222v228); clamp in an inline double helper (222v228); if/else sign + float-limit helper
//   (222v228); the sign ternary inline as the left operand with a float GFMin (224v228). next: `--flags /Op` (the store
//   and memory re-read of the float gap look like /Op float-consistency code) or a float-typed helper taking the sign.
// Shape: the landed GetTCIfPresent (the component local deliberately unset when absent: retail reads the key's dead
//   slot); `float diff = height + 0.2f - pos.Z;` `long sign = diff < 0.0f ? -1 : 1;` then one expression
//   `sign * (e > 1.0f ? 1.0f : e)` over `e = exp(diff * diff * 2.25f) - 1.0` (intrinsic exp: the fldl2e / f2xm1 / fscale
//   sequence; sign is loaded first and the clamp stays on the x87 stack); the acceleration is a C3DVector(0, 0, z)
//   temporary. No pragma.
#include <math.h>
#pragma intrinsic(exp)
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
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
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
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
    void UpdateHeightAboveGround();
};

void CTCCoopSpirit::UpdateHeightAboveGround()
{
    CThingCreatureBase* creature = ParentThing;
    CTCPhysicsBase* physics = (CTCPhysicsBase*)creature->GetTCIfPresent(2);
    const C3DVector& pos = physics->Position;
    float diff = creature->GetHeightZBelowAt(pos, false, true) + GetHeightOffset() - pos.Z;
    long sign = diff < 0.0f ? -1 : 1;
    double e = exp(diff * diff * 2.25f) - 1.0;
    physics->ApplyAcceleration(C3DVector(0.0f, 0.0f, (float)(sign * (e > 1.0f ? 1.0f : e))));
}
