// CTCDParticleEmitter::func_00991640(const C3DVector& v) const -- return v scaled by the float at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991640, length 36 bytes (48 with padding); no direct reference found.
// Part of the catalogue row 00991620 (skipped ledger, chunk 901); the manifest class (CTCDParticleEmitter) is
// propagated -- the PDB layout has a CCharString at +0x0c and a long at +0x10 -- so `this` is modelled locally.
// Shape: the three products built on the x87 stack right to left (`fld [eax+8] ; fmul [ecx+10h]`, `fld [eax+4] ;
// fmul [ecx+10h]`, `fld [ecx+10h] ; fmul [eax]`), then the hidden result pointer loaded and the three stores
// x, y, z; `ret 8`.
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CScaledObject_Retail {
    unsigned char _pad_0x00[0x10];
    float         m_scale;                     // +0x10
    C3DVector func_00991640(const C3DVector& pos) const;
};

C3DVector CScaledObject_Retail::func_00991640(const C3DVector& pos) const
{
    return C3DVector(pos.X * m_scale, pos.Y * m_scale, pos.Z * m_scale);
}