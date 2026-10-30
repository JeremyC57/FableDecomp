// CShaderRenderManager::UpdateLocalisedSphereMapping() -- when the current vertex-shader layout has the two-register localised sphere-mapping range, form ViewMatrix * ObjectMatrix and upload its first row and its negated second row, each normalised (table-driven fast reciprocal square root with one Newton step) and scaled to half with w = 0.5, then clear dirty flag 0x8000
// Grade: DIFFER(952v917) nd=229 (was 839v917 nd=392) -- retail_asm candidate: the SSE branch is now the hand-written __asm 4x4 product (byte-equal to retail except the three operand frame slots: ours [esp+14/18/1Ch] v retail [esp+8/0Ch/10h]); residue: frame 0x88 v 0x78 (retail shares the operand slots with the FastInvSqrt temporaries), the x87 fallback's per-element term order (retail 3,1,0,2 / 3,1,2,0 / 0,3,1,2 / 2,3,1,0 -- hand-unrolled), normalisation spills
// Leaf: UpdateLocalisedSphereMapping
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988E80, 917 bytes (+ int3 padding), plain `ret`; ebp frame aligned to 16 for the
//   SSE path.
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94) through the inlined SetVSConstants (as landed 0x0098A540).
// Globals: SSE-available flag 0x013D2880, reciprocal-square-root mantissa table 0x013CE558 (128 entries), float constants
//   0x0122DED4 / 0x0122F59C (the Newton step's 3 and 0.5).
// Layout: as the landed UpdateWorldSpaceLighting 0x0098A540 (CurrentVSConstantLayout +0x4, D3DDevice +0x1d4,
//   UpdateFlags +0x3d8); ObjectMatrix +0x1f0, ViewMatrix +0x230 (16-aligned 4x4); the layout's sphere-mapping range at
//   +0x98 (CShaderConstantRange: Offset +0x9c, Count +0xa0).
// Shape: retail's SSE branch is a hand-written inline-assembly block; this C++ form spells it with SSE intrinsics.
#include <xmmintrin.h>
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct CShaderConstantRange {
    virtual ~CShaderConstantRange();
    long          Offset;                        // +0x4
    unsigned long Count;                         // +0x8
};
struct CVertexShaderConstantLayout {
    unsigned char        _pad00[0x4c];
    unsigned long        LightSize;              // +0x4c
    CShaderConstantRange LightArray;             // +0x50
    CShaderConstantRange LightGlobals;           // +0x5c
    CShaderConstantRange LightAttenuations;      // +0x68
};
struct CShaderGenericLight {
    C4DVector     Parameters[2];                 // +0x00
    long          Types[2];                      // +0x20
    float         Attenuation2;                  // +0x28
    float         AttenuationBase;               // +0x2c
    unsigned long Size;                          // +0x30
};
struct CLightInfo {
    CShaderGenericLight Light;                   // +0x00
    bool          IsEnabled;                     // +0x34
    bool          UpdateFlag;                    // +0x35
};
template <class T> struct CArray {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T* GetData() const
    {
        const T* p = (const T*)this;
        if (size() > 0)
            p = _M_start;
        return p;
    }
};
struct IDirect3DDevice9 {
    virtual void d000() const; virtual void d001() const; virtual void d002() const; virtual void d003() const; virtual void d004() const; virtual void d005() const;
    virtual void d006() const; virtual void d007() const; virtual void d008() const; virtual void d009() const; virtual void d010() const; virtual void d011() const;
    virtual void d012() const; virtual void d013() const; virtual void d014() const; virtual void d015() const; virtual void d016() const; virtual void d017() const;
    virtual void d018() const; virtual void d019() const; virtual void d020() const; virtual void d021() const; virtual void d022() const; virtual void d023() const;
    virtual void d024() const; virtual void d025() const; virtual void d026() const; virtual void d027() const; virtual void d028() const; virtual void d029() const;
    virtual void d030() const; virtual void d031() const; virtual void d032() const; virtual void d033() const; virtual void d034() const; virtual void d035() const;
    virtual void d036() const; virtual void d037() const; virtual void d038() const; virtual void d039() const; virtual void d040() const; virtual void d041() const;
    virtual void d042() const; virtual void d043() const; virtual void d044() const; virtual void d045() const; virtual void d046() const; virtual void d047() const;
    virtual void d048() const; virtual void d049() const; virtual void d050() const; virtual void d051() const; virtual void d052() const; virtual void d053() const;
    virtual void d054() const; virtual void d055() const; virtual void d056() const; virtual void d057() const; virtual void d058() const; virtual void d059() const;
    virtual void d060() const; virtual void d061() const; virtual void d062() const; virtual void d063() const; virtual void d064() const; virtual void d065() const;
    virtual void d066() const; virtual void d067() const; virtual void d068() const; virtual void d069() const; virtual void d070() const; virtual void d071() const;
    virtual void d072() const; virtual void d073() const; virtual void d074() const; virtual void d075() const; virtual void d076() const; virtual void d077() const;
    virtual void d078() const; virtual void d079() const; virtual void d080() const; virtual void d081() const; virtual void d082() const; virtual void d083() const;
    virtual void d084() const; virtual void d085() const; virtual void d086() const; virtual void d087() const; virtual void d088() const; virtual void d089() const;
    virtual void d090() const; virtual void d091() const; virtual void d092() const; virtual void d093() const;
    virtual long __stdcall SetVertexShaderConstantF(unsigned int startRegister, const float* data, unsigned int vector4fCount); // slot 94 (+0x178)
};
struct CSphereMapLayout {
    unsigned char        _pad00[0x98];
    CShaderConstantRange LocalisedSphereMapping; // +0x98
};
struct CMatrix44 {
    float M[4][4];
};
extern bool g_SSEAvailable;                      // 0x013D2880
extern unsigned long g_InvSqrtTable[128];        // 0x013CE558
inline float FastInvSqrt(float x)
{
    unsigned long bits = *(unsigned long*)&x;
    unsigned long r = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) | (g_InvSqrtTable[(bits >> 17) & 0x7f] << 15);
    float y = *(float*)&r;
    return (3.0f - y * y * x) * y * 0.5f;
}
struct CShaderRenderManager {
    void*             __vftable;
    CSphereMapLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char     _pad008[0x1d4 - 0x8];
    IDirect3DDevice9* D3DDevice;                 // +0x1d4
    unsigned char     _pad1d8[0x1f0 - 0x1d8];
    CMatrix44         ObjectMatrix;              // +0x1f0
    CMatrix44         ViewMatrix;                // +0x230
    unsigned char     _pad270[0x3d8 - 0x270];
    unsigned long     UpdateFlags;               // +0x3d8
    void SetVSConstants(long startRegister, const C4DVector* data, unsigned long count)
    {
        if (count != 0)
            D3DDevice->SetVertexShaderConstantF(startRegister, (const float*)data, count);
    }
    void UpdateLocalisedSphereMapping();
};

void CShaderRenderManager::UpdateLocalisedSphereMapping()
{
    if (CurrentVSConstantLayout->LocalisedSphereMapping.Count >= 2) {
        __declspec(align(16)) CMatrix44 result;
        const CMatrix44& t = ViewMatrix;
        const CMatrix44& m = ObjectMatrix;
        if (g_SSEAvailable) {
            const CMatrix44* right = &m;
            CMatrix44* product = &result;
            const CMatrix44* left = &t;
            // Retail-asm: 0x00988ED3 movaps xmm0, xmmword ptr [edx] .. 0x00988FB4 movaps xmmword ptr [edi+30h], xmm4 -- the
            //   hand-written SSE 4x4 product (operands reloaded from their frame slots [esp+8/0Ch/10h] into edx/edi/eax, fixed xmm0..xmm7 schedule)
            __asm {
            mov eax, left
            mov edx, right
            mov edi, product
            movaps xmm0, [edx]
            movaps xmm1, [edx + 0x10]
            movaps xmm2, [edx + 0x20]
            movaps xmm3, [edx + 0x30]
            movaps xmm4, [eax]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xaa
            shufps xmm7, xmm7, 0xff
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi], xmm4
            movaps xmm4, [eax + 0x10]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xaa
            shufps xmm7, xmm7, 0xff
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi + 0x10], xmm4
            movaps xmm4, [eax + 0x20]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xaa
            shufps xmm7, xmm7, 0xff
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi + 0x20], xmm4
            movaps xmm4, [eax + 0x30]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xaa
            shufps xmm7, xmm7, 0xff
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi + 0x30], xmm4
            }
        } else {
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c)
                    result.M[r][c] = t.M[r][0] * m.M[0][c] + t.M[r][1] * m.M[1][c] + t.M[r][2] * m.M[2][c] + t.M[r][3] * m.M[3][c];
        }
        C4DVector axes[2];
        float s = FastInvSqrt(result.M[0][0] * result.M[0][0] + result.M[0][1] * result.M[0][1] + result.M[0][2] * result.M[0][2]);
        axes[0] = C4DVector(result.M[0][0] * s * 0.5f, result.M[0][1] * s * 0.5f, result.M[0][2] * s * 0.5f, 0.5f);
        float x = -result.M[1][0], y = -result.M[1][1], z = -result.M[1][2];
        s = FastInvSqrt(z * z + x * x + y * y);
        axes[1] = C4DVector(x * s * 0.5f, y * s * 0.5f, z * s * 0.5f, 0.5f);
        SetVSConstants(CurrentVSConstantLayout->LocalisedSphereMapping.Offset, axes, 2);
    }
    UpdateFlags &= ~0x8000;
}
