// C3DSoundSource::SetVelocity(const C3DVector& velocity) -- when the velocity changes, store it, mirror it into the pending-parameter block (+0x2e8) and raise the velocity dirty bit (0x4000)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C53E40, 144 bytes; `ret 4`. No callees.
// Label: the catalog's "CSubtitleRenderer::SetVWindowsForDeprecatedDrawInterface" is propagated: CSubtitleRenderer is
//        0x128 bytes (Ego_r.pdb) and the real method takes two C2DBoxF; this body sits in the audio module
//        (0x00C53B60 decibels-to-amplitude) and updates a 3D vector with a dirty-flag word. Class and member names are descriptive.
// Leaf: SetVelocity
// Shape: the current velocity comes from an inline by-value getter (x stays on the FPU, y/z are copied to the frame);
//        (the twin of SetPosition 0x00C53D50) `!=` is the inline three-term C3DVector compare (`fucompp ; test ah,44h ; jp`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    bool operator!=(const C3DVector& o) const { return X != o.X || Y != o.Y || Z != o.Z; }
};
struct C3DSoundSource {
    unsigned char _pad000[0x224];
    C3DVector     Position;                        // +0x224
    C3DVector     Velocity;                        // +0x230
    unsigned char _pad23c[0x54];
    unsigned long DirtyFlags;                      // +0x290
    unsigned char _pad294[0x48];
    C3DVector     PendingPosition;                 // +0x2dc
    C3DVector     PendingVelocity;                 // +0x2e8
    C3DVector GetVelocity() const { return Velocity; }
    void SetVelocity(const C3DVector& velocity);
};

void C3DSoundSource::SetVelocity(const C3DVector& velocity)
{
    if (velocity != GetVelocity()) {
        Velocity = velocity;
        PendingVelocity = velocity;
        DirtyFlags |= 0x4000;
    }
}