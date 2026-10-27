// CTCPhysicsStandard::GetOffsetVector3D() const -- the thing's 3D offset vector
// Leaf: ?GetOffsetVector3D@CTCPhysicsStandard@@MBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724D50, 24 bytes, thiscall, `ret 4` (hidden C3DVector result).
// Identity (proven): slot 29 of the vtables of CTCPhysicsStandard 0x01265F84, CTCPhysicsControlled 0x0126616C and
//   CTCPhysicsNavigator 0x01266314 (RTTI; inherited by the last two); Ego_r.pdb ?GetOffsetVector3D@CTCPhysicsStandard@@MBE?AVC3DVector@@XZ
//   (protected virtual const) and CTCPhysicsStandard::OffsetVector3D is the C3DVector at +0x44 (Ego_r.pdb +68) copied here;
//   slot 28 is GetOffsetVector (family M183). The manifest's "vfunc_29" is a placeholder.
// Shape (family M199, representative 0x0069E1F0): `return OffsetVector3D;`.
class C3DVector {                                          // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // the engine's member-wise copy (the implicit one allocates differently)
};
class CThingPhysicsMeshInfo;
class CTCPhysicsStandard {
protected:
    virtual C3DVector GetOffsetVector3D() const;
public:
    unsigned char Base[0x3C];                             // +0x04 CTCPhysicsBase (after the vptr)
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;              // +0x40
    C3DVector OffsetVector3D;                             // +0x44
};
C3DVector CTCPhysicsStandard::GetOffsetVector3D() const
{
    return OffsetVector3D;
}