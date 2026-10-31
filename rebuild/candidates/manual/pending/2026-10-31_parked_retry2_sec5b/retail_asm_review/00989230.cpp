// CShaderRenderManager::UpdateObjectSpaceTextureTransform() -- for every texture-transform constant range of the current vertex-shader layout that is in use, upload TextureTransforms[i] * ObjectMatrix (at most four float4 rows) and move the dirty flags from 0x800 to 0x1000
// Grade: DIFFER(636v636) 54 diff lines (asm oracle; base intrinsics 535v636, 237) -- the 4x4 multiply as a __forceinline helper whose __asm names its three pointer parameters (mov eax,left ; mov edx,right ; mov edi,out: homed then reloaded, as retail 0x0098928D..0x009892A5) gives retail's length and SSE block; residue: x87 fallback term order (VC canonicalises a single sum: all 24 orders identical; stepwise in retail order 60->54), range slot 0x24 v 0x2c, the homing stores before the je
// Retail-asm: 0x0098929D..0x0098938A `mov eax,[esp+2Ch] ; mov edx,[esp+28h] ; mov edi,[esp+24h] ; movaps xmm0,[edx] ... shufps xmm4,xmm4,0 ; shufps xmm5,xmm5,55h ... addps xmm4,xmm6 ; movaps [edi+30h],xmm4` -- the hand SSE 4x4 multiply (same block as 0x00988A50, rasm1 retail_asm_confirmed): pointers homed and reloaded, fixed xmm0..xmm7, 3-operand-style shuffles of one register.
// Leaf: UpdateObjectSpaceTextureTransform
// Retail: Fable.exe (TLC, VC7.1) @ 0x00989230, 636 bytes (+ int3 padding), plain `ret`; ebp frame aligned to 16 for the
//   SSE path.
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94) through the inlined SetVSConstants (as landed 0x0098A540).
// Globals: SSE-available flag 0x013D2880.
// Layout: as the landed UpdateWorldSpaceLighting 0x0098A540 (CurrentVSConstantLayout +0x4, D3DDevice +0x1d4,
//   UpdateFlags +0x3d8); ObjectMatrix +0x1f0 (16-aligned 4x4), TextureTransforms +0x330 (0x40 each),
//   TextureTransformCount +0x38c; the layout's texture-transform ranges start at +0xa4 (12-byte CShaderConstantRange).
// Shape: retail's SSE branch is a hand-written inline-assembly block (movaps/shufps/mulps over xmm0..xmm7 with the operands
//   moved through frame slots); this C++ form spells it with SSE intrinsics, the x87 branch as the four-row product.
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
struct CTextureTransformLayout {
    unsigned char        _pad00[0xa4];
    CShaderConstantRange TextureTransforms[1];   // +0xa4
};
struct CMatrix44 {
    float M[4][4];
};
extern bool g_SSEAvailable;                      // 0x013D2880
struct CShaderRenderManager {
    void*                    __vftable;
    CTextureTransformLayout* CurrentVSConstantLayout;    // +0x004
    unsigned char            _pad008[0x1d4 - 0x8];
    IDirect3DDevice9*        D3DDevice;                  // +0x1d4
    unsigned char            _pad1d8[0x1f0 - 0x1d8];
    CMatrix44                ObjectMatrix;               // +0x1f0
    unsigned char            _pad230[0x330 - 0x230];
    CMatrix44                TextureTransforms[1];       // +0x330
    unsigned char            _pad370[0x38c - 0x370];
    unsigned long            TextureTransformCount;      // +0x38c
    unsigned char            _pad390[0x3d8 - 0x390];
    unsigned long            UpdateFlags;                // +0x3d8
    void SetVSConstants(long startRegister, const float* data, unsigned long count)
    {
        if (count != 0)
            D3DDevice->SetVertexShaderConstantF(startRegister, data, count);
    }
    void UpdateObjectSpaceTextureTransform();
};

static __forceinline void MultiplySSE(CMatrix44* out, const CMatrix44* left, const CMatrix44* right)
{
    __asm {
        mov     eax, left
        mov     edx, right
        mov     edi, out
        movaps  xmm0, [edx]
        movaps  xmm1, [edx+10h]
        movaps  xmm2, [edx+20h]
        movaps  xmm3, [edx+30h]
        movaps  xmm4, [eax]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi], xmm4
        movaps  xmm4, [eax+10h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+10h], xmm4
        movaps  xmm4, [eax+20h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+20h], xmm4
        movaps  xmm4, [eax+30h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+30h], xmm4
    }
}

void CShaderRenderManager::UpdateObjectSpaceTextureTransform()
{
    for (unsigned long i = 0; i < TextureTransformCount; ++i) {
        const CShaderConstantRange& range = CurrentVSConstantLayout->TextureTransforms[i];
        if (range.Count) {
            __declspec(align(16)) CMatrix44 result;
            const CMatrix44& t = TextureTransforms[i];
            const CMatrix44& m = ObjectMatrix;
            if (g_SSEAvailable) {
                MultiplySSE(&result, &t, &m);
            } else {
                for (int r = 0; r < 4; ++r)
                    for (int c = 0; c < 4; ++c) {
                        float v = t.M[r][3] * m.M[3][c];
                        v += t.M[r][1] * m.M[1][c];
                        v += t.M[r][2] * m.M[2][c];
                        v += t.M[r][0] * m.M[0][c];
                        result.M[r][c] = v;
                    }
            }
            SetVSConstants(range.Offset, result.M[0], range.Count < 4 ? range.Count : 4);
        }
    }
    UpdateFlags = (UpdateFlags & ~0x800) | 0x1000;
}
