// CTCGatherPointRadialArea::vfunc_28(const C3DVector& position) -- TRUE when the position lies strictly inside the gather radius around the owning thing (2D, squared distances)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFD60, length 75 bytes.
// Callees: the owner's position accessor @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof", propagated; it returns the
// thing's position by reference).
// Layout from Ego_r.pdb (CTCGatherPointRadialArea, 36 bytes; retail 0x20 with the 12-byte vector): CTCBase ParentThing +0x04,
// GatherRadius (float) +0x1c (the landed vfunc_1 @ 0x007FFE80 persists it).
// Shape: the owner is read into a local first (`mov eax,ecx` then the owner into ecx), the radius into a named local BEFORE the call (it is homed in the `push ecx` slot and re-read as
// `fld [esp] ; fmul [esp]`); the two deltas are named locals kept on the x87 stack; `dx*dx + dy*dy` written as one
// sum squares the top-of-stack delta (dy) first; `long` result (`mov eax,1` / `xor eax,eax`).
struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector& GetPosition();                     // 0x004C73D0
};

struct CTCGatherPointRadialArea {
    unsigned char _pad_0x00[0x04];                      // vptr
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x14];                      // flags, Users vector, MaxOccupation
    float         GatherRadius;                         // +0x1c
    long vfunc_28(const C3DVector& position);
};

long CTCGatherPointRadialArea::vfunc_28(const C3DVector& position)
{
    CThing* owner = ParentThing;
    float radius = GatherRadius;
    const C3DVector& centre = owner->GetPosition();
    float dx = position.X - centre.X;
    float dy = position.Y - centre.Y;
    if (dx * dx + dy * dy < radius * radius)
        return 1;
    return 0;
}