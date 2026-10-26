// NPlayerGui::CDrawQuickAccessIconsPC::ClearPrimitivesIfNecessary() -- when not drawing this frame, clear the PC quick-access handle vector; then the CDrawQuickAccessIcons clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657550, 48 bytes. Virtual slot 0 of NPlayerGui::CDrawQuickAccessIconsPC's vtable
// 0x0125A804 (the manifest's "vfunc_0").
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > @ 0x0043F360 with
// CEnginePrimitiveHandle::Clear @ 0x0041BE70, NPlayerGui::CDrawQuickAccessIcons::ClearPrimitivesIfNecessary @ 0x0064D62F
// (the base's slot 0, called non-virtually; landed).
// Layout: Drawing +0x18 (CDrawBase); the PC subclass's 12-byte VC6 handle vector at +0x94, past the 0x94-byte CDrawQuickAccessIcons
// (whose last landed member, QuickAccessSpellCrossHandles, sits at +0x70). The member name is descriptive.
// Shape: a speed TU (16-byte aligned, `mov al ; test al,al` flag test, `last` loaded into eax before `first` into edx).
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

class CDrawBase { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0: 0x00644627
    unsigned char       _body04[0x18 - 4];
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
};

class CDrawQuickAccessIcons : public CDrawBase { public:         // vtable 0x0125A704
    unsigned char       _state28[0x94 - 0x28];     // charge state, handles, spell-cross handles
    virtual void ClearPrimitivesIfNecessary();     // 0x0064D62F
};

struct CDrawQuickAccessIconsPC : CDrawQuickAccessIcons {   // vtable 0x0125A804
    CRetailHandleVector PCPrimitiveHandles;        // +0x94
    virtual void ClearPrimitivesIfNecessary();
};

void CDrawQuickAccessIconsPC::ClearPrimitivesIfNecessary()
{
    if (!Drawing)
        for_each(PCPrimitiveHandles.begin(), PCPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    CDrawQuickAccessIcons::ClearPrimitivesIfNecessary();
}

}