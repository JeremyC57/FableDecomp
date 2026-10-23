// CPlayerGui::DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& info) -- size the handle vector to eight; with the safe-area debug display on, draw the 640x480 title-safe frame (four one-pixel white lines, alpha 0) and the action-safe frame (four lines, opaque) as 2D sprites, otherwise clear every handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043B050, length 1312 bytes, `ret 4`.
// Callees: vector<CEnginePrimitiveHandle>::resize 0x00443F90 (the catalog's "CEnginePrimitiveRenderer::AddPrimitive" is
//   propagated), CEnginePrimitive2DSprite ctor (16 arguments) 0x0041BEB0, std::for_each<CEnginePrimitiveHandle*,
//   mem_fun_ref_t> 0x0043F360 (/Gr __fastcall: hidden result in ecx, first in edx), CEnginePrimitiveHandle::Clear
//   0x0041BE70 (pushed as the functor); the renderer's virtual slot 23 (+0x5C) on info +0x34.
// Layout: CPlayerGui ShowDebugSafeAreas +0x2b0, SafeAreaHandles +0x310 (12-byte STLport vector of 8-byte handles).
// Model: CTCFireheartMinigame::DrawPreparePrimitivesForRendering 0x007E7C90 (sprite ctor, renderer slot) and
//   NPlayerGui::CStatBar::ClearGraphics 0x00645F8D (for_each + mem_fun_ref).
struct CEnginePrimitiveHandle {
    void Clear();                                   // 0x0041BE70
    long  m_header;                                 // +0x00
    void* Data;                                     // +0x04
};
struct CMemFunRefClear {                            // std::mem_fun_ref_t<void, CEnginePrimitiveHandle>
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f);   // 0x0043F360
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned long n);                   // 0x00443F90
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
class CEnginePrimitive2DSprite {
public:
    unsigned __int64 _id;                           // 8-aligned: the frame is realigned (`and esp,-8`)
    unsigned char _body[0x50];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long a, unsigned long b, long blend,
                             long interpolation, float c, float d, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned char alpha, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
class CPlayerGui {
public:
    unsigned char _pad000[0x2b0];
    bool ShowDebugSafeAreas;                        // +0x2b0
    unsigned char _pad2b1[0x310 - 0x2b1];
    CRetailVector<CEnginePrimitiveHandle> SafeAreaHandles;   // +0x310
    void DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& info);
};
void CPlayerGui::DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& info)
{
    SafeAreaHandles.resize(8);
    if (ShowDebugSafeAreas) {
        CEnginePrimitive2DSprite top(0, C2DBoxF(32.0f, 24.0f, 608.0f, 25.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite left(0, C2DBoxF(32.0f, 24.0f, 33.0f, 456.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite right(0, C2DBoxF(608.0f, 24.0f, 609.0f, 456.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite bottom(0, C2DBoxF(32.0f, 456.0f, 609.0f, 457.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite innerTop(0, C2DBoxF(48.0f, 36.0f, 592.0f, 37.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite innerLeft(0, C2DBoxF(48.0f, 36.0f, 49.0f, 444.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite innerRight(0, C2DBoxF(592.0f, 36.0f, 593.0f, 444.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        CEnginePrimitive2DSprite innerBottom(0, C2DBoxF(48.0f, 444.0f, 593.0f, 445.0f), 0x43, 0, 0, 3, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start, top, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 1, left, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 2, right, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 3, bottom, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 4, innerTop, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 5, innerLeft, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 6, innerRight, 0x40);
        info.Renderer->AddPrimitive(SafeAreaHandles._M_start + 7, innerBottom, 0x40);
    } else {
        for_each(SafeAreaHandles._M_start, SafeAreaHandles._M_finish, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    }
}
