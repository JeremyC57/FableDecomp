// std::_Destroy_range(pair<long, CCountedPointer<CRegionConnectionNode> >* first, .. last, const allocator&) -- run the element destructor on [first, last) of a vector of {long, counted pointer} pairs
// Leaf: _Destroy_range
// Retail: Fable.exe @ 0x00A80E40, 80 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Thirteen call sites 0x00A792CD .. 0x00A84021 (the nav quad-tree module; the
// landed CNavQuadTree constructor 0x00A7A7F0 references it).
// Element (12 bytes, as the catalog's pair<long, CCountedPointer<CRegionConnectionNode> >): a long +0 and the counted
// pointer {Data +4, Info +8}.
// Callees: operator delete @ 0x00BFE9BC (the count block).
// Shape: only the CCountedPointer member has a destructor: the inlined release on the count-block induction register
// (`lea esi,[edi+8]`), both words zeroed, stride 12.
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
struct CCountedPointer {
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CRegionConnectionNode;
struct SRegionConnectionPair {                  // std::pair<long, CCountedPointer<CRegionConnectionNode> >, 12 bytes
    long                                   first;    // +0x00
    CCountedPointer<CRegionConnectionNode> second;   // +0x04
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SRegionConnectionPair*, SRegionConnectionPair*, const allocator<SRegionConnectionPair>&);
}