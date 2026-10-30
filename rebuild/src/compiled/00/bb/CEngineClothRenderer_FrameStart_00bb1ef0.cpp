// CEngineClothRenderer::FrameStart() -- pick this frame's shadow vertex-shader tables: the colour-buffer sets when the shadow renderer runs technique 2, else the depth-buffer sets
// Leaf: ?FrameStart@CEngineClothRenderer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1EF0, 90 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 of vtable 0x012A56EC (RTTI .?AVCEngineClothRenderer@@ : CEnginePrimitiveRenderUtil :
//   CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ?FrameStart@CEngineClothRenderer@@UAEXXZ.
// Globals: 0x01436E60 the CEngineShadowRenderer (engine/CEngineShadowRenderer.h: RenderingTechnique +0x334).
// Layout (Ego_r.pdb names, retail 12-byte CArrays): TriVertexShaders +0x04, TriVertexShadersDepthBufferShadow +0x10,
//   TriVertexShadersSpotDepthBufferShadow +0x1C, TriVertexShadersColourBufferShadow +0x28,
//   TriVertexShadersSpotColourBufferShadow +0x34, TriVertexShadersShadow +0x40, TriVertexShadersSpotShadow +0x44
//   (CVertexShader 8 bytes).
// Shape: both arms assign both pointers through CArray::GetAsCArray() (FableWin.pdb ?GetAsCArray@?$CArray@...@@QAE...: the
//   first element when size() > 0 (`sar ; test ; jbe`; a bare size() test folds to `test esi,0FFFFFFF8h`), or the private
//   GetZeroSizeArray() = the array object itself); VC7.1 tail-merges the two
//   second assignments into one block fed by the array address in eax.
#define CEngineShadowRenderer CEngineShadowRenderer_HeaderLayout
#include "engine/CEngineShadowRenderer.h"
#undef CEngineShadowRenderer

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    T* GetAsCArray() { return this->size() > 0 ? this->_M_start : const_cast<T*>(GetZeroSizeArray()); }
private:
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
};

class CEngineShadowRenderer : public CEngineShadowRenderer_HeaderLayout {
public:
    long PeekRenderingTechnique() const { return *reinterpret_cast<const long*>(RenderingTechnique); }
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;     // 0x01436E60

class CVertexShader {
public:
    void* Data[2];
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePrimitiveRenderUtil : public CBaseClass {
public:
    virtual void FrameStart();
};

class CEngineClothRenderer : public CEnginePrimitiveRenderUtil {
public:
    virtual void FrameStart();
    CArray<CVertexShader> TriVertexShaders;                         // +0x04
    CArray<CVertexShader> TriVertexShadersDepthBufferShadow;        // +0x10
    CArray<CVertexShader> TriVertexShadersSpotDepthBufferShadow;    // +0x1C
    CArray<CVertexShader> TriVertexShadersColourBufferShadow;       // +0x28
    CArray<CVertexShader> TriVertexShadersSpotColourBufferShadow;   // +0x34
    CVertexShader* TriVertexShadersShadow;                          // +0x40
    CVertexShader* TriVertexShadersSpotShadow;                      // +0x44
};

void CEngineClothRenderer::FrameStart()
{
    if (g_pShadowRenderer_01436e60->PeekRenderingTechnique() == 2) {
        TriVertexShadersShadow = TriVertexShadersColourBufferShadow.GetAsCArray();
        TriVertexShadersSpotShadow = TriVertexShadersSpotColourBufferShadow.GetAsCArray();
    } else {
        TriVertexShadersShadow = TriVertexShadersDepthBufferShadow.GetAsCArray();
        TriVertexShadersSpotShadow = TriVertexShadersSpotDepthBufferShadow.GetAsCArray();
    }
}