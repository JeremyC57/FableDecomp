// NPlayerGui::CDrawMiniMap::ClearPrimitives() -- clear every primitive handle the drawer owns (`for_each(PrimitiveHandles, mem_fun_ref(&CEnginePrimitiveHandle::Clear))`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644660, 23 bytes. Virtual slot 1 of NPlayerGui::CDrawMiniMap's vtable
// 0x0125A634 (the only reference); FableWin.pdb lists ?ClearPrimitives@CDrawMiniMap@NPlayerGui@@UAEXXZ.
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > @ 0x0043F360 (/Gr `__fastcall`:
// hidden result in ecx, first in edx, last and the 4-byte functor on the stack), CEnginePrimitiveHandle::Clear @ 0x0041BE70
// (pushed as the functor's pointer-to-member constant). Same for_each spelling as the landed CDrawQuickAccessIcons
// ::ClearPrimitivesIfNecessary 0x0064D62F.
// Layout: CDrawBase PrimitiveHandles (retail 12-byte VC6 vector) at +0x1c, as in the landed CDrawQuestInfo rows.
// Shape: /Os TU without a frame pointer (`optimize("s",on)`; the unaligned NPlayerGui module): the dropped functor result
// lives in the one `push ecx` frame word whose address is the hidden result pointer, `pop ecx ; ret`.
#pragma optimize("s", on)
class CEnginePrimitiveHandle { public:                    // 8 bytes: vptr + counted primitive pointer
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

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
};

class CDrawMiniMap { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0
    virtual void ClearPrimitives();                // slot 1
    unsigned char       _base04[0x18 - 4];         // CDrawBase: PlayerGui .. Bank
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c (CDrawBase)
};

void CDrawMiniMap::ClearPrimitives()
{
    for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
}

}