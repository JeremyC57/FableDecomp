// CTCCameraPointDefinitionBase::GetActiveCameraPosition() const -- the base camera point's position: a copy of the
// parent thing's position
// Leaf: ?GetActiveCameraPosition@CTCCameraPointDefinitionBase@@MBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C1E0, length 33 bytes, `ret 4` (hidden result pointer).
// Identity (FINDINGS chunk 393: "landed with a wrong name: 0x75C1E0 CTCCameraPointDefinitionBase::
//   GetActiveCameraPosition"; was CTCCameraPointShapeActivationBase::GetTrackPosition): vtable slot 35, shared by nine
//   camera-point vtables (rtti-vtables: CTCCameraPointShapeActivationBase 0x0123AEDC slot 35 = 0x0075C1E0) and overridden
//   only by CTCCameraPointTrack (0x0123B0E4 slot 35 = 0x0075DC60, chunk 393's CTCCameraPointTrack::
//   GetActiveCameraPosition); Ego_r.pdb has exactly two out-of-line GetActiveCameraPosition bodies,
//   `?GetActiveCameraPosition@CTCCameraPointDefinitionBase@@MBE?AVC3DVector@@XZ` (protected virtual const) and the
//   CTCCameraPointTrack override -- this is the base one (no GetTrackPosition member exists in either PDB).
// Callees: CThing::GetPos() const @ 0x004C73D0 (a reference to the physics position).
// Shape: the vector comes back through the hidden pointer (`ret 4`, eax = result). The copy is three dword moves
//   alternating edx/eax with the result pointer loaded once into ecx: a MEMBERWISE user copy constructor over dword
//   members (an implicit POD copy pulls in esi; float members would go through the x87), so the engine's float X/Y/Z
//   are modelled as long here (as the landed source).
// Model: CTCBase is declared locally (engine/CTCBase.h is flattened with an explicit vptr word, which would add a
//   second vptr under a virtual method).
class C3DVector {                                   // X, Y, Z (floats in the engine; copied as dwords)
public:
    long X;
    long Y;
    long Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CThing {
public:
    const C3DVector& GetPos() const;           // 0x004C73D0
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};

class CTCCameraPointDefinitionBase : public CTCBase {
protected:
    virtual C3DVector GetActiveCameraPosition() const;
};

C3DVector CTCCameraPointDefinitionBase::GetActiveCameraPosition() const
{
    return ParentThing->GetPos();
}
