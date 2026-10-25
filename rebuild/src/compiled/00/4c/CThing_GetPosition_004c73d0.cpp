// CThing::GetPosition() const -- the thing's position: a reference to its physics component's Position
// Leaf: ?GetPosition@CThing@@QBEABVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C73D0, 7 bytes: `mov eax,[ecx+60h] ; add eax,0Ch ; ret`.
// Source correction (FINDINGS chunks 391 / 407: "0x4C73D0/0x6A4D00 GetPosition (labelled DoSizeof)"): the landed body
//   was the propagated manifest model `int __fastcall DoSizeof(CVertexBufferWin32*)` returning `[+0x60] + 12` as an
//   int; the bytes are the address of CTCPhysicsBase::Position (+0x0C, struct_layouts_egor) inside the CThing's
//   PhysicsTC (struct_layouts_egor +0x70; +0x60 in retail, where the STLport list/containers ahead of it are smaller).
//   2,341 call sites use it as the thing's position (e.g. landed CThingDeadCreature::WorldUpdate 0x00836260 and
//   CThingAICreature::IsMessageEventWithinHearingRange 0x008332E0 declare it `const C3DVector& GetPosition() const`).
//   The inline accessor has no out-of-line name in Ego_r/Ego_d; the catalog name (CThing::GetPosition) is unchanged.
//   The byte twin 0x006A4D00 is a different symbol (no identical-code folding in retail), see UNRESOLVED.
// Model: CThing / CTCPhysicsBase are declared locally with their retail offsets (the PDB +0x70 does not hold here).
class C3DVector {                               // a class in Ego_r.pdb (mangled V)
public:
    float X, Y, Z;
};

class CTCPhysicsBase {
public:
    unsigned char _base[0x0c];                  // +0x00 CTCBase (vptr, ParentThing, flags)
    C3DVector     Position;                     // +0x0c
};

class CThing {
public:
    unsigned char   _pad[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60 (retail)
    const C3DVector& GetPosition() const;
};

const C3DVector& CThing::GetPosition() const
{
    return PhysicsTC->Position;
}
