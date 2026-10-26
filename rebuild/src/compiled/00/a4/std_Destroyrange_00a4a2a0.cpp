// std::_Destroy_range(SNamedSurfaceEntry* first, SNamedSurfaceEntry* last, const allocator&) -- run the element destructor on [first, last) of a vector of 16-byte {CCharString, word, CCountedPointer} records
// Leaf: _Destroy_range
// Retail: Fable.exe @ 0x00A4A2A0, 84 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Seven call sites 0x00A495C5 .. 0x00A4AD2A in the same module.
// Element (from the body): a CCharString at +0 (~CCharString @ 0x0099EAE0), a plain word at +4 and a CCountedPointer
// {object +8, count block +0xC} -- 16 bytes. The catalog's `pair<long, CCompositeSurfaceInfo>` is propagated: a long at
// +0 would not be destroyed (the body runs ~CCharString on +0), and CCompositeSurfaceInfo's vptr word is absent.
// Callees: ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC (the count block).
// Shape: the implicit element destructor runs the members in reverse: the inlined CCountedPointer release (count
// decrement, re-read, destroy hook [block+4] with the object in ecx, delete the block, both words zeroed) then the
// out-of-line ~CCharString. VC7.1 keeps the count-block address as a second induction register (`lea esi,[edi+0Ch]`)
// and hoists `mov ecx,edi` above the two zero stores; `mov edi,edi` is the /O2 loop-head alignment pad.
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

class CCompositeSurfaceCache;
struct SNamedSurfaceEntry {                     // 16 bytes
    CCharString                             Name;       // +0x00
    long                                    Value;      // +0x04
    CCountedPointer<CCompositeSurfaceCache> Cache;      // +0x08
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SNamedSurfaceEntry*, SNamedSurfaceEntry*, const allocator<SNamedSurfaceEntry>&);
}