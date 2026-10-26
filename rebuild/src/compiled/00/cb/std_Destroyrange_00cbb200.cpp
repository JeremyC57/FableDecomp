// std::_Destroy_range(pair<CCharString, CCountedPointer<..> >* first, .. last, const allocator&) -- run the element destructor on [first, last) of a vector of 12-byte {CCharString, counted pointer} pairs
// Leaf: _Destroy_range
// Retail: Fable.exe @ 0x00CBB200, 84 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Eight call sites 0x00CBB706 .. 0x00CBD54A (the landed CRecordSet 0x00CBD510
// destructor among them).
// Element (from the body): 12 bytes, a CCharString at +0 and a CCountedPointer {object +4, count block +8}. The
// catalog's `pair<long, CCompositeSurfaceInfo>` is propagated: that pair is 16 bytes and a long at +0 has no destructor.
// Callees: ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC (the count block).
// Shape: the implicit pair destructor: second (the inlined release on the count-block induction register,
// `lea esi,[edi+8]`) then first (`mov ecx,edi` hoisted above the two zero stores).
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
class CCountedPointer { public:
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

class CCharString { public:
    void* m_pStringData;                        // +0x00
    ~CCharString();                             // 0x0099EAE0
};

class CCountedObject;
struct SNamedCountedPair {                      // std::pair<CCharString, CCountedPointer<..> >, 12 bytes
    CCharString                     first;      // +0x00
    CCountedPointer<CCountedObject> second;     // +0x04
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SNamedCountedPair*, SNamedCountedPair*, const allocator<SNamedCountedPair>&);
}