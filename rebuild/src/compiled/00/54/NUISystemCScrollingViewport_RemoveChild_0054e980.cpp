// NUISystem::CScrollingViewport::RemoveChild(CComponent*) -- vtable slot 89: find the component pointer in the child vector at +0x164; when present erase it (memmove the tail down, --finish) and clear the viewport's primitive handle at +0x15c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054E980, length 126 bytes (padded to 128).
// Leaf: RemoveChild
// Label correction: the catalog "CParticleEmitter::ClearAllPrimitives" is propagated and wrong; the body is slot 89 of
// NUISystem::CScrollingViewport's vtable 0x012468D4 (rtti-vtables.tsv). The method name is descriptive.
// Callees: STLport find(first, last, const T&, random_access_iterator_tag) @ 0x00535FE4 (`__fastcall`), memmove @ 0x00BFEAE6,
// the primitive's slot 3 (last-handle notification) and slot 1 (Destroy).
// Shape: `find(begin(), end()) != end()` then STLport erase(pos): __copy_trivial(pos + 1, _M_finish, pos) in its
// verbatim `(last == first) ? result : memmove + n` form, then `--_M_finish` (`add [esi+168h],-4`). Every _M_finish read
// folds into the one load cached in edi before the call. CEnginePrimitiveHandle::Clear is inlined (null-safe
// GetRefCount() == 1 -> slot 3, then the counted pointer assigned a default one: release + the second zero store), all
// inside the found branch. find is a same-TU noinline stand-in with a pure counted-loop body. Retail's find is
// LTCG-compiled (`__fastcall`), so its caller knows the call writes no memory and does not reload _M_finish. The stand-in
// gives the same knowledge. Its `last - first` count clobbers edx, so the caller still reloads edx before the call
// as retail does (a plain `first != last` loop lets VC7.1's register-usage tracking keep last in edx).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CEnginePrimitiveHandle;
struct CEnginePrimitiveBase {               // counted primitive: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    virtual void v02();
    virtual void OnLastHandleCleared(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                        // +0x04
};
struct CIVCountedPointer_Primitive {
    CEnginePrimitiveBase* m_p;
    CIVCountedPointer_Primitive() : m_p(0) { }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
    void Release()
    {
        CEnginePrimitiveBase* object = m_p;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer_Primitive& operator=(const CIVCountedPointer_Primitive& other)
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
struct CEnginePrimitiveHandle : CBase {     // vtable 0x0122F598
    CIVCountedPointer_Primitive Data;       // +4
    void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.m_p->OnLastHandleCleared(this);
        Data = CIVCountedPointer_Primitive();
    }
};
namespace NUISystem {
struct CComponent;
#pragma auto_inline(off)
__declspec(noinline) CComponent** __fastcall find(CComponent** first, CComponent** last, CComponent* const& value,
                             const random_access_iterator_tag&)   // 0x00535FE4 stand-in
{
    long n = last - first;
    for (; n > 0; --n, ++first)
        if (*first == value)
            break;
    return first;
}
#pragma auto_inline(on)
inline CComponent** __copy_trivial(const CComponent* const* first, const CComponent* const* last, CComponent** result)
{
    return (last == first) ? result :
        (CComponent**)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
}
struct CComponentVector {                   // retail STLport vector<CComponent*>
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    CComponent** erase(CComponent** pos)
    {
        __copy_trivial(pos + 1, _M_finish, pos);
        --_M_finish;
        return pos;
    }
};
struct CScrollingViewport {
    virtual void v00();
    unsigned char _viewport[0x158];         // +0x004
    CEnginePrimitiveHandle m_handle;        // +0x15c
    CComponentVector m_children;            // +0x164
    void RemoveChild(CComponent* child);
};
void CScrollingViewport::RemoveChild(CComponent* child)
{
    CComponent** it = find(m_children._M_start, m_children._M_finish, child, random_access_iterator_tag());
    if (it != m_children._M_finish) {
        m_children.erase(it);
        m_handle.Clear();
    }
}
}