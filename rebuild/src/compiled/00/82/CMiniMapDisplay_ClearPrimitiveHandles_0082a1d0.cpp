// CMiniMapDisplay::ClearPrimitiveHandles() -- clear every map and marker primitive handle the minimap display owns
// Leaf: ?ClearPrimitiveHandles@CMiniMapDisplay@@QAEXXZ
// Retail: Fable.exe @ 0x0082A1D0, 49 bytes, plain `ret`. Catalog label "CWorldMap::PrepareForLoad" is propagated; the only
// caller is NPlayerGui::CDrawMiniMap::ClearPrimitivesIfNecessary 0x00643DE5 (`lea ecx,[esi+28h]` = the retail MiniMap
// member, PDB +0x2C), and FableWin.pdb lists ?ClearPrimitiveHandles@CMiniMapDisplay@@QAEXXZ.
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > @ 0x0043F360 (/Gr
// `__fastcall`: hidden result in ecx, first in edx, last and the 4-byte functor on the stack), CEnginePrimitiveHandle::Clear
// @ 0x0041BE70 (pushed as the functor's pointer-to-member constant). Same spelling as the landed
// CDrawMiniMap::ClearPrimitives 0x00644660 (that TU is /Os; this one is the default speed build, loads before pushes).
// Layout (PDB CMiniMapDisplay with retail 12-byte vectors): vptr +0, MapPrimitiveHandles +4, MarkerPrimitiveHandles +0x10.
// engine/CMiniMapDisplay.h carries the Ego_r 16-byte vectors (MarkerPrimitiveHandles +0x14), so `this` is a local model.
// Shape: both dropped functor results share the one `push ecx` frame word.
struct CEnginePrimitiveHandle {                    // 8 bytes: vptr + counted primitive pointer
    void* _vtable;
    void* m_pPrimitive;
    void Clear();                                  // 0x0041BE70
};

template <class R, class T> class mem_fun_ref_t {  // SGI functor holding a 4-byte pointer-to-member
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};

template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}

template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x0043F360 (SGI, /Gr)

struct CRetailHandleVector {                       // retail vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
};

struct CMiniMapDisplay {
    void*               _vtable;                   // +0x00
    CRetailHandleVector MapPrimitiveHandles;       // +0x04
    CRetailHandleVector MarkerPrimitiveHandles;    // +0x10
    long                MiniMapGraphic;            // +0x1c
    void ClearPrimitiveHandles();
};

void CMiniMapDisplay::ClearPrimitiveHandles()
{
    for_each(MapPrimitiveHandles.begin(), MapPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    for_each(MarkerPrimitiveHandles.begin(), MarkerPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
}