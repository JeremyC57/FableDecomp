// _global::DestroyAux_SNamedListRecord18_0047c800(SNamedListRecord18*, SNamedListRecord18*, const SFalseType&) -- STLport __destroy_aux: destroy every 24-byte record of [first, last)
// Leaf: DestroyAux_SNamedListRecord18_0047c800
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047C800, length 85 bytes.
// Callees: ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// STLport `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (__fastcall: first in ecx, last in
// edx, the empty __false_type reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);`
// with the element's implicit destructor inlined. The 24-byte element is {key, CCharString name, long, vector<12-byte
// record>}: members die in reverse order, so the vector goes first -- its `_Destroy(_M_start, _M_finish)` runs the
// inner records' destructor (only the CCharString at +4 of each 12-byte record is non-trivial), then the
// `_Vector_base` destructor frees the storage when `_M_start` is non-null (STLport's malloc allocator = free) --
// and the name's ~CCharString runs last. The manifest label (`_Move_backward<pair<unsigned long, CDefString>*>`) is
// propagated: the body is no move at all (no destination, `ret 4`), and the element is not a pair<ulong, CDefString>.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

class CCharString { public:
    void* m_pData;                                         // +0
    ~CCharString();                                        // 0x0099EAE0
};

struct SNamedRecordC {                                     // 12-byte inner record
    long        m_key;                                     // +0
    CCharString m_name;                                    // +4
    long        m_value;                                   // +8
};

template <class T>
inline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T>
struct CRetailVector {                                     // STLport vector<T>: 12 bytes
    T* _M_start;                                           // +0
    T* _M_finish;                                          // +4
    T* _M_end_of_storage;                                  // +8
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct SNamedListRecord18 {                                // 24-byte element
    unsigned long                 m_key;                   // +0x00
    CCharString                   m_name;                  // +0x04
    long                          m_value;                 // +0x08
    CRetailVector<SNamedRecordC>  m_records;               // +0x0C
};

struct SFalseType { };

void __fastcall DestroyAux_SNamedListRecord18_0047c800(SNamedListRecord18* first, SNamedListRecord18* last, const SFalseType& tag)
{
    for (; first != last; ++first)
        first->~SNamedListRecord18();
}