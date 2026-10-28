// CGetBestIciclePointForTarget::CGetBestIciclePointForTarget(CThing* target, float time) -- the search functor for the best icicle drop point: the target's position extrapolated by its last-frame XY movement times `time`, best score -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2A90, length 122 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof", propagated); the placement record
//          (+0x60) holds the previous position at +0x18. The 0.0f pool constant @ 0x0122DEDC comes from the literal Z of the
//          movement vector (`fld time ; fmul 0.0f`: VC keeps the literal as the memory operand).
// Shape: BestPosScore is the init-list member (stored first, `mov [esi+0Ch],0BF800000h`); TargetPos is assigned in the body
// from `pos + C3DVector(dx, dy, 0) * time` -- the operator results are hidden-pointer temporaries folded onto the FPU and
// copied into the member through a frame temporary (three dword moves).
// Layout: Ego_r.pdb (TargetPos +0, BestPosScore +0x0c, BestPos +0x10).
// Leaf: ??0CGetBestIciclePointForTarget
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CThingPlacement_Retail {
    unsigned char _pad00[0x18];
    C3DVector     m_prevPosition;                  // +0x18
};
class CThing { public:
    unsigned char           _pad00[0x60];
    CThingPlacement_Retail* m_pPlacement;          // +0x60
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CGetBestIciclePointForTarget {
    C3DVector TargetPos;                           // +0x00
    float     BestPosScore;                        // +0x0c
    C3DVector BestPos;                             // +0x10
    CGetBestIciclePointForTarget(CThing* target, float time);
};

CGetBestIciclePointForTarget::CGetBestIciclePointForTarget(CThing* target, float time)
    : BestPosScore(-1.0f)
{
    const C3DVector& pos = target->GetPos();
    const C3DVector& prev = target->m_pPlacement->m_prevPosition;
    TargetPos = pos + C3DVector(pos.X - prev.X, pos.Y - prev.Y, 0.0f) * time;
}