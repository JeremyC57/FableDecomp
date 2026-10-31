// CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& cameraPos, const CMatrix3x4& transform, const C3DPrimitive2& primitive, float scale, const C3DMeshMaterial* material, const CGraphicDataBank* bank) -- the mipmap level a mesh primitive's diffuse texture needs: the distance from the camera to the transformed bounding sphere's surface, stretched by the primitive's average texture stretch, sets the on-screen texel ratio; log2 of the frame width over it (clamped 1..255) through the highest-set-bit table, 0 when the primitive is too close or untextured
// Grade: DIFFER(399v403) -- the bounding-sphere copy: retail loads Centre.X / .Y on the x87 stack (homed in the dead `primitive` slot and [ebp-4]) and copies .Z / .Radius through esi into frame words, then keeps the transformed X and the camera offsets X / Y in memory; the implicit struct copy here is a block move (frame 0x2C v 0x20) and memberwise copies or a by-value transform helper enregister every component (372v403 / 366v403). Smaller residues: retail re-reads the GraphicList begin pointer after the bounds test and returns the zero result from edx (`xor edx,edx` early, `mov eax,edx`); the FloorToLong retail_asm block and the clamp / table lookup match.
// Leaf: ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIKABVC3DVector@@ABVCMatrix3x4@@ABVC3DPrimitive2@@MPBVC3DMeshMaterial@@PBVCGraphicDataBank@@@Z
// Retail-asm: 0x00BA9DE2..0x00BA9DFC `fld [ebp+14h] ; fsub [ebp+10h] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp+8] ; fstp [ebp-4] ; mov eax,[ebp+14h] ; cmp eax,[ebp-4] ; jne ; inc dword ptr [ebp+0Ch]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants at 0x00BA9DC3 / 0x00BA9DD3, a bare dword fistp into a named long; same block as GFFloatToLongFloor 0x0048D940 and the landed FloorToLong rows 0x006A68F0 / 0x00646010); VC7.1 emits no bare dword fistp from C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA9CB0, 403 bytes, static __fastcall (cameraPos ecx, transform edx, the rest on
//   the stack), `ret 10h`, ebp frame (the inline asm).
// Identity (two sources): (1) FableWin.pdb ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIKABVC3DVector@@ABVCMatrix3x4@@ABVC3DPrimitive2@@MPBVC3DMeshMaterial@@PBVCGraphicDataBank@@@Z
//   (static __fastcall, returns unsigned long; two register and four stack arguments = `ret 10h`); (2) the body reads
//   exactly those types at their Ego_r.pdb offsets -- C3DPrimitive2::BoundingSphere +0x0C and AverageTextureStretch
//   +0x1C, C3DMeshMaterial::DiffuseMapBankIndex +0x10, the bank's CArray<CGraphicInfo> (0x2C each) and
//   CGraphicHeader::FrameWidth +6, CEnginePrimitiveRendererSharedData::MipmapSizeMultiplier +0xB0 -- and it sits after
//   its siblings GetDiffuseTexture .. Get2DAlphaMaskTexture (0x00BA9360..0x00BA9A70). The manifest label
//   "ComputeParticleVisibilityDistance" is descriptive.
// Globals: g_pPrimitiveRendererSharedData 0x01436E78, g_highestSetBitTable 0x0143B990 (the landed spellings), 1e-4f
//   0x0129BA3C (literal: the __real@ pool).
// Layout: local models (no engine headers for C3DVector / CMatrix3x4 / C3DPrimitive2 / C3DMeshMaterial / CGraphicDataBank;
//   the bank's GraphicList is at retail +0x1E0).
#include <math.h>
class C3DVector {
public:
    float X, Y, Z;
};
class CMatrix3x4 {                                 // rows: X axis, Y axis, Z axis, translation
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
class C3DBoundingSphere {                          // Ego_r.pdb, 16 bytes
public:
    C3DVector Centre;                              // +0x00
    float     Radius;                              // +0x0c
};
class C3DPrimitive2 {
public:
    unsigned char     _pad00[0x0c];
    C3DBoundingSphere BoundingSphere;              // +0x0c
    float             AverageTextureStretch;       // +0x1c
};
class C3DMeshMaterial {
public:
    unsigned char _pad00[0x10];
    unsigned long DiffuseMapBankIndex;             // +0x10
};
class CGraphicHeader {                             // Ego_r.pdb, 28 bytes
public:
    unsigned short Width;                          // +0x00
    unsigned short Height;                         // +0x02
    unsigned short Depth;                          // +0x04
    unsigned short FrameWidth;                     // +0x06
    unsigned char  _pad08[0x1c - 0x08];
};
class CGraphicInfo {                               // Ego_r.pdb, 44 bytes
public:
    CGraphicHeader Header;                         // +0x00
    unsigned char  _pad1c[0x2c - 0x1c];
};
template <class T> class CArray {
public:
    T* Begin;                                      // +0x00
    T* End;                                        // +0x04
    T* Capacity;                                   // +0x08
    unsigned long Size() const { return End - Begin; }
    const T* Get(unsigned long index) const { return index < Size() ? &Begin[index] : 0; }
};
class CGraphicDataBank {
public:
    unsigned char        _pad000[0x1e0];
    CArray<CGraphicInfo> GraphicList;              // +0x1e0
};
class CEnginePrimitiveRendererSharedData {
public:
    unsigned char _pad000[0xb0];
    float         MipmapSizeMultiplier;            // +0xb0
};
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData;   // 0x01436E78
extern signed char g_highestSetBitTable[256];                                // 0x0143B990

// the engine's inline-assembly FPU floor helper (standalone copy GFFloatToLongFloor 0x0048D940; the landed FloorToLong):
// VC7.1 expands the __forceinline body in place; the by-value parameter and the locals become frame words.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

class CEnginePrimitiveManagerMeshBase {
public:
    static unsigned long __fastcall GetAppropriateMipmap(const C3DVector& cameraPos, const CMatrix3x4& transform,
                                                         const C3DPrimitive2& primitive, float scale,
                                                         const C3DMeshMaterial* material, const CGraphicDataBank* bank);
};

unsigned long __fastcall CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& cameraPos,
                                                                              const CMatrix3x4& transform,
                                                                              const C3DPrimitive2& primitive, float scale,
                                                                              const C3DMeshMaterial* material,
                                                                              const CGraphicDataBank* bank)
{
    C3DBoundingSphere sphere = primitive.BoundingSphere;
    C3DVector centre;
    centre.X = sphere.Centre.X * transform.E11 + sphere.Centre.Y * transform.E21 + sphere.Centre.Z * transform.E31 + transform.E41;
    centre.Y = sphere.Centre.X * transform.E12 + sphere.Centre.Y * transform.E22 + sphere.Centre.Z * transform.E32 + transform.E42;
    centre.Z = sphere.Centre.X * transform.E13 + sphere.Centre.Y * transform.E23 + sphere.Centre.Z * transform.E33 + transform.E43;
    unsigned long mipmap = 0;
    C3DVector offset;
    offset.X = cameraPos.X - centre.X;
    offset.Y = cameraPos.Y - centre.Y;
    offset.Z = cameraPos.Z - centre.Z;
    float distance = (sqrtf(offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z) - sphere.Radius * scale)
                   * primitive.AverageTextureStretch;
    if (distance > 0.0001f) {
        long index = material->DiffuseMapBankIndex;
        if (index > 0) {
            const CGraphicInfo* info = bank->GraphicList.Get(index);
            long level = FloorToLong(info->Header.FrameWidth
                                     / (g_pPrimitiveRendererSharedData->MipmapSizeMultiplier / distance * scale));
            if (level < 1)
                level = 1;
            else if (level > 255)
                level = 255;
            return (unsigned char)g_highestSetBitTable[(unsigned char)level];
        }
    }
    return mipmap;
}
