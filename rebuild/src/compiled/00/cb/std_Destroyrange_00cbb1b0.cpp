// std::_Destroy_range(SCountedRecord16* first, SCountedRecord16* last, const allocator&) -- run the element destructor on [first, last) of a vector of 16-byte records whose only non-trivial member is a counted pointer at +8
// Leaf: _Destroy_range
// Retail: Fable.exe @ 0x00CBB1B0, 80 bytes, `ret 4`: __fastcall (first ecx, last edx, the empty allocator by const
// reference on the stack, never read). Fourteen call sites 0x00CB80DA .. 0x00CBD56A (the landed CRecordSet 0x00CBD510
// destructor and CActiveEntityScriptVector::_M_insert_overflow 0x00CBC850 among them).
// Element (from the body): 16 bytes, two plain words at +0 / +4 and a CCountedPointer {object +8, count block +0xC}.
// The catalog's `pair<long, CCountedPointer<CRegionConnectionNode> >` (12 bytes) is propagated from the 0x00A80E40 twin:
// this body strides 16 bytes -- an 8-byte key (the script module keys its records by unsigned __int64 ids).
// Callees: operator delete @ 0x00BFE9BC (the count block).
// Shape: the inlined CCountedPointer release on the count-block induction register (`lea esi,[edi+0Ch]`).
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

class CCountedObject;
struct SCountedRecord16 {                       // pair<unsigned __int64, CCountedPointer<..> >, 16 bytes
    unsigned long                   m_keyLow;   // +0x00
    unsigned long                   m_keyHigh;  // +0x04
    CCountedPointer<CCountedObject> second;     // +0x08
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
template void __fastcall _Destroy_range(SCountedRecord16*, SCountedRecord16*, const allocator<SCountedRecord16>&);
}