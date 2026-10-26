// CSoundManagerLH::vfunc_52(const C3DVector& v) -- copy the 12-byte vector argument into the member at +0x74
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41800, length 26 bytes.
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 52.
// Shape: the source address read once, the destination formed with `add ecx,74h`, then three dword moves
// through edx/eax: the implicit (bitwise) assignment of a three-float POD (a user memberwise operator=
// would move the floats through the x87), `ret 4`.
class C3DVector { public:
    float x, y, z;
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    unsigned char _04[0x70];                    // +0x04..+0x73
    C3DVector     m_vector;                     // +0x74
    virtual void vfunc_52(const C3DVector& v);
};

void CSoundManagerLH::vfunc_52(const C3DVector& v)
{
    m_vector = v;
}