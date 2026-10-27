// CTCPhysicsBase::GetOffsetVector() const -- an empty C2DVector
// Leaf: ?GetOffsetVector@CTCPhysicsBase@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B13D0, 20 bytes, thiscall, `ret 4` (the result by hidden pointer).
// Identity (probable): slot 28 of CTCPhysicsBase's vtable 0x0125D2C4 (also CTCPhysicsLight's); the landed CTCPhysicsBase::GetMapwhoPosition 0x006AFCB0 adds this slot's C2DVector to Position, CTCPhysicsStandard's override 0x00724D30 returns its OffsetVector3D.X/Y, and Ego_r.pdb names the override ?GetOffsetVector@CTCPhysicsRigidBody@@UBE?AVC2DVector@@XZ -- the base symbol itself is not in the PDBs. The manifest's "vfunc" labels are placeholders.
// Shape (family M183, representative 0x00661CA0): `return C2DVector(0.0f, 0.0f);` -- both words stored as zero into the hidden result. Speed TU.
class C2DVector {
public:
    C2DVector(float x, float y) : X(x), Y(y) { }
    float X, Y;
};
class CTCPhysicsBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual C2DVector GetOffsetVector() const;          // slot 28
};
C2DVector CTCPhysicsBase::GetOffsetVector() const
{
    return C2DVector(0.0f, 0.0f);
}