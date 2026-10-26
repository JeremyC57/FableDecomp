// NPlayerGui::CDrawQuestInfo::CQuestInfoElement::Clear() -- release the element's primitives: clear every handle it owns (`for_each(PrimitiveHandles, mem_fun_ref(&CEnginePrimitiveHandle::Clear))`)
// Leaf: Clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644FE5, 23 bytes. Virtual slot 2 of NPlayerGui::CDrawQuestInfo::CQuestInfoElement's vtable
// 0x0125A2B4 and of six derived element vtables (0x0125A44C..0x0125A520 reference it); slot 2 is the landed rows' Clear.
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > @ 0x0043F360 (/Gr `__fastcall`:
// hidden result in ecx, first in edx, last and the 4-byte functor on the stack), CEnginePrimitiveHandle::Clear @ 0x0041BE70
// (pushed as the functor's pointer-to-member constant). Same for_each spelling as the landed CDrawQuickAccessIcons
// ::ClearPrimitivesIfNecessary 0x0064D62F.
// Layout: CQuestInfoElement (retail 0x1c bytes) PrimitiveHandles at +0x10, as in the landed CDrawQuestInfo::UpdateBar 0x00644D91.
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

class CDrawQuestInfo { public:
class CQuestInfoElement { public:                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes)
    virtual void v00();                            // slot 0
    virtual bool IsActive();                       // slot 1
    virtual void Clear();                          // slot 2
    virtual long GetType();                        // slot 3
    long                ID;                        // +0x04
    long                ScriptID;                  // +0x08
    float               Scale;                     // +0x0c
    CRetailHandleVector PrimitiveHandles;          // +0x10
};
};

void CDrawQuestInfo::CQuestInfoElement::Clear()
{
    for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
}

}