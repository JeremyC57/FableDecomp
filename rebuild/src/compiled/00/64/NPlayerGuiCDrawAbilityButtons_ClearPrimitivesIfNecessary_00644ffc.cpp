// NPlayerGui::CDrawAbilityButtons::ClearPrimitivesIfNecessary() -- when not drawing this frame, clear the two button primitive handles and every ability primitive handle; then the base drawer's clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644FFC, 157 bytes. Virtual slot 0 of NPlayerGui::CDrawAbilityButtons' vtable
// 0x0125A6C8 (the manifest's "vfunc_0").
// Callees: the primitive's virtual slot 3 (handle released while it is the last reference) and slot 1 (Destroy), SGI
// for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > @ 0x0043F360 with CEnginePrimitiveHandle::Clear
// @ 0x0041BE70, NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() @ 0x00644627 (the base's slot 0, called non-virtually).
// Layout: Drawing +0x18 (CDrawBase); two inline CEnginePrimitiveHandle members (vptr + counted Data) at +0x2c and +0x34; a 12-byte
// VC6 handle vector at +0x3c. The member names are descriptive (the PDB carries no CDrawAbilityButtons layout).
// Shape: the model of the landed CDrawQuickAccessIcons::ClearPrimitivesIfNecessary 0x0064D62F (inline Clear: null-safe GetRefCount
// == 1 -> slot 3, then assignment from a default-constructed counted pointer: release through a local copy with its own zero
// store, then the unconditional zero store), here in a /Os TU WITHOUT a frame pointer (`optimize("s",on)` only): the shared zero
// lives in ebx, each handle's address in esi, the for_each result in the `push ecx` word.
#pragma optimize("s", on)
class CEnginePrimitiveHandle;

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

class CEnginePrimitiveHandle : public CBase { public:            // vtable 0x0122F598
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

class CDrawBase { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0: 0x00644627
    unsigned char       _body04[0x18 - 4];
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
};

class CDrawAbilityButtons : public CDrawBase { public:           // vtable 0x0125A6C8
    long                   Columns;                // +0x28
    CEnginePrimitiveHandle ButtonPrimitiveHandle;  // +0x2c
    CEnginePrimitiveHandle HighlightPrimitiveHandle;   // +0x34
    CRetailHandleVector    AbilityPrimitiveHandles;    // +0x3c
    virtual void ClearPrimitivesIfNecessary();
};

void CDrawAbilityButtons::ClearPrimitivesIfNecessary()
{
    if (!Drawing) {
        ButtonPrimitiveHandle.Clear();
        HighlightPrimitiveHandle.Clear();
        for_each(AbilityPrimitiveHandles.begin(), AbilityPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    }
    CDrawBase::ClearPrimitivesIfNecessary();
}

}