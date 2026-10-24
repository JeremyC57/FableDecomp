// std::_Destroy_range(pair<CCharString, C3DMeshVolume>* first, pair<CCharString, C3DMeshVolume>* last, const allocator&) -- run the element destructor on [first, last) of a vector of named mesh volumes
// Leaf: _Destroy_range
// Retail: Fable.exe @ 0x00A70EE0, 96 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Eight call sites 0x00A6D5A5 .. 0x00A7348E.
// Types (Ego_r.pdb): pair<CCharString, C3DMeshVolume> (16 bytes) = first CCharString +0, second C3DMeshVolume +4
//   {Name CCharString +4, ConvexVolume CCountedPointer<CContainmentVolume> +8 (object) / +0xC (count block)}.
// Callees: ~CCharString @ 0x0099EAE0 (twice), operator delete @ 0x00BFE9BC (the count block).
// Shape: the implicit pair destructor: second (ConvexVolume's inlined release, then ~Name through `lea ecx,[esi-8]`)
// before first (`mov ecx,edi`). The count-block address is the second induction register (`lea esi,[edi+0Ch]`).
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

struct CCharString {
    void* m_pStringData;                        // +0x00
    ~CCharString();                             // 0x0099EAE0
};

class CContainmentVolume;
struct C3DMeshVolume {                          // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};
struct SMeshVolumePair {                        // std::pair<CCharString, C3DMeshVolume>, 16 bytes
    CCharString   first;                        // +0x00
    C3DMeshVolume second;                       // +0x04
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SMeshVolumePair*, SMeshVolumePair*, const allocator<SMeshVolumePair>&);
}