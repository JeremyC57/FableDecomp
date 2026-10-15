#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::ClearPrimitivesIfNecessary() -- when not drawing this frame, stop the charging sound, clear the charge-up handle and every special / spell-cross primitive handle; then the base drawer's clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D62F, length 121 bytes; slot 0 of the CDrawQuickAccessIcons vtable 0x0125A704.
// Callees: CDrawQuickAccessIcons::ClearSoundEffects() @ 0x0064B668, the primitive's virtual slot 3 (handle released while it is the
// last reference) and slot 1 (Destroy), SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle>> @ 0x0043F360
// with CEnginePrimitiveHandle::Clear @ 0x0041BE70 (twice; the pointer-to-member constant kept in edi),
// NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() @ 0x00644627 (the base's slot 0, called non-virtually).
// Layout (retail, see the CDrawQuickAccessIcons constructor 0x00652F0A): Drawing +0x18, ChargeUpPrimitiveHandle +0x5c (vptr + counted
// Data pointer), SpecialPrimitiveHandles +0x64, QuickAccessSpellCrossHandles +0x70.
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`, the for_each functor result in the 4-byte frame slot); the
// charge-up handle's Clear is inlined on `lea edi,[esi+5Ch]`: the reference count read through a null-safe inline accessor
// (`test ; je ; mov eax,[eax+4] ; jmp ; xor eax,eax`), the slot-3 notification when it is 1, then the counted pointer assigned
// a default-constructed one (release through a local pointer copy with its own zero store, then the unconditional zero store);
// every helper is `__forceinline` (the size TU keeps plain inline ones out of line).
struct CEnginePrimitiveHandle;

struct CEnginePrimitiveBase {                      // counted primitive: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleCleared(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

struct CIVCountedPointer_Primitive {
    CEnginePrimitiveBase* m_p;
    __forceinline CIVCountedPointer_Primitive() : m_p(0) { }
    __forceinline long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
    __forceinline void Release()
    {
        CEnginePrimitiveBase* object = m_p;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            m_p = 0;
        }
    }
    __forceinline CIVCountedPointer_Primitive& operator=(const CIVCountedPointer_Primitive& other)
    {
        Release();
        m_p = other.m_p;
        if (m_p)
            ++m_p->m_refCount;
        return *this;
    }
};

struct CBase {
    virtual void v00();
};

struct CEnginePrimitiveHandle : CBase {            // vtable 0x0122F598
    CIVCountedPointer_Primitive Data;              // +4
    __forceinline void Clear()                     // 0x0041BE70 out of line (inlined here: size TU needs __forceinline)
    {
        if (Data.GetRefCount() == 1)
            Data.m_p->OnLastHandleCleared(this);
        Data = CIVCountedPointer_Primitive();
    }
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

struct CDrawBase {
    virtual void ClearPrimitivesIfNecessary();     // slot 0: 0x00644627
    unsigned char _body04[0x18 - 4];
    bool          Drawing;                         // +0x18
    unsigned char _body19[0x28 - 0x19];
};

struct CDrawQuickAccessIcons : CDrawBase {         // vtable 0x0125A704
    unsigned char          _state[0x5c - 0x28];    // charge state, flash fade
    CEnginePrimitiveHandle ChargeUpPrimitiveHandle;       // +0x5c
    CRetailHandleVector    SpecialPrimitiveHandles;       // +0x64
    CRetailHandleVector    QuickAccessSpellCrossHandles;  // +0x70
    void ClearSoundEffects();                      // 0x0064B668
    virtual void ClearPrimitivesIfNecessary();
};

void CDrawQuickAccessIcons::ClearPrimitivesIfNecessary()
{
    if (!Drawing) {
        ClearSoundEffects();
        ChargeUpPrimitiveHandle.Clear();
        for_each(SpecialPrimitiveHandles.begin(), SpecialPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        for_each(QuickAccessSpellCrossHandles.begin(), QuickAccessSpellCrossHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    }
    CDrawBase::ClearPrimitivesIfNecessary();
}

}
