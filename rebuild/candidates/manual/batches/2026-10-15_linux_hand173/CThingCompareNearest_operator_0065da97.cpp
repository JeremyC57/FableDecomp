// CThingCompare_Nearest::operator()<CThingAICreature>(CThingAICreature* a, CThingAICreature* b) const -- the nearer of two things to the comparator's position (a null side loses)
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DA97, 145 bytes (`ret 8`); size-optimised TU with an ebp frame (`leave`).
// Callees: CThing::GetPosition @ 0x004C73D0 (label "CVertexBufferWin32::DoSizeof" propagated).
// Layout: CThingCompare_Nearest (Ego_r.pdb 12 bytes) = C3DVector Pos.
// Shape: operator- and GetLengthSquared `__forceinline` (the size TU outlines them otherwise); a null -> b, b null -> a; both offsets from Pos (the first one spilled to the frame across the second
//   GetPosition), then `distA < distB ? a : b` (A summed first) (`fcompp ; test ah,41h ; je`).
#pragma optimize("s", on)
#pragma optimize("y", off)   // ebp frame
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingAICreature : CThing { };

struct CThingCompare_Nearest {
    C3DVector Pos;                              // +0x00
    template<class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        C3DVector offsetA = a->GetPosition() - Pos;
        C3DVector offsetB = b->GetPosition() - Pos;
        if (offsetA.GetLengthSquared() < offsetB.GetLengthSquared())
            return a;
        return b;
    }
};

template CThingAICreature* CThingCompare_Nearest::operator()(CThingAICreature* a, CThingAICreature* b) const;
