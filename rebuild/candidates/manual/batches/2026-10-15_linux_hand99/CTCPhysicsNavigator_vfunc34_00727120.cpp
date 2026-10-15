// CTCPhysicsNavigator::vfunc_34(const C3DVector& pos, bool usePhysics) -- forces a move: stores ForcedMoveToPos, sets ForcedMoveToPosUsePhysics and ForcedMoveToPosSet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00727120, length 57 bytes (vtable slot 34 of CTCPhysicsNavigator, rtti-vtables.tsv).
// Callees: none.
// Shape: the 12-byte vector is copied dword by dword through a `lea eax,[ecx+0A1h]` base (the implicit POD assignment of
// C3DVector), then the two `bool : 1` flags of the byte at +0xad are written: the argument bit 1 by the xor-merge
// store and bit 0 by `or al,1`, both folded into ONE byte store (the second store reuses the merged register).
// Layout: Ego_r.pdb CTCPhysicsNavigator (0xb0 bytes, packed): Navigating +0x88, PositionToNavigateTo +0x89, AdditionalImpulse
// +0x95, ForcedMoveToPos +0xa1, ForcedMoveToPosSet / ForcedMoveToPosUsePhysics bits 0 / 1 of +0xad. engine/CTCPhysicsNavigator.h
// matches these offsets but flattens the vectors and has no virtuals, so the class is modelled locally with #pragma pack(1)
// (the members sit at odd offsets in retail too).
struct C3DVector { float X, Y, Z; };          // plain POD: the implicit assignment is the lea-based dword copy

#pragma pack(push, 1)
struct CTCPhysicsNavigator {
    virtual void v00();
    unsigned char _base[0x84];                 // +0x04 CTCPhysicsStandard body
    bool      Navigating;                      // +0x88
    C3DVector PositionToNavigateTo;            // +0x89
    C3DVector AdditionalImpulse;               // +0x95
    C3DVector ForcedMoveToPos;                 // +0xa1
    bool ForcedMoveToPosSet : 1;               // +0xad bit 0
    bool ForcedMoveToPosUsePhysics : 1;        // +0xad bit 1
    void vfunc_34(const C3DVector& pos, bool usePhysics);
};
#pragma pack(pop)

void CTCPhysicsNavigator::vfunc_34(const C3DVector& pos, bool usePhysics)
{
    ForcedMoveToPos = pos;
    ForcedMoveToPosUsePhysics = usePhysics;
    ForcedMoveToPosSet = true;
}
