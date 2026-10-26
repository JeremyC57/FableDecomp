// SNamedListPair_00a46f30::SNamedListPair_00a46f30() -- default constructor of two {CCharString name; three-pointer vector} records: each name built by the out-of-line CCharString constructor, each vector zeroed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46F30, length 46 bytes.
// Leaf: ??0SNamedListPair_00a46f30
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2).
// The manifest label "CShaderResource::CShaderResource" is propagated (that constructor is 0x00A5D870, with a
// vtable); this body has no vtable store and no callers or pointer references in the image. Named by its shape.
// Shape: each record's vector is an STLport vector built by its default constructor (`vector(const allocator_type& =
// allocator_type())` -> _Vector_base(alloc): start, finish and the _STLP_alloc_proxy end-of-storage nulled); with that
// real structure the second string constructor's `mov ecx,edi` lands between the 2nd and 3rd zero stores as in retail
// (a plain three-pointer class with an inline constructor schedules it above all three -- the 24i "lane gap").
class CCharString { public:
    void* m_pData;
    CCharString();                              // 0x0099E4B0
};

template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
};

template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};

template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;
    T*             m_pFinish;
    CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};

template <class T> struct CSTLportVector : CVectorBase<T> {     // STLport vector<T>
    explicit CSTLportVector(const CAllocator<T>& alloc = CAllocator<T>()) : CVectorBase<T>(alloc) { }
};


struct SNamedList {                             // 0x10 bytes
    CCharString   m_name;                       // +0x00
    CSTLportVector<void*> m_items;              // +0x04
};

struct SNamedListPair_00a46f30 {
    SNamedList m_first;                         // +0x00
    SNamedList m_second;                        // +0x10
    SNamedListPair_00a46f30();
};

SNamedListPair_00a46f30::SNamedListPair_00a46f30()
{
}