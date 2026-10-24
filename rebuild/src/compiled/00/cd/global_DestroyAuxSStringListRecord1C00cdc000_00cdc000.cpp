// _global::DestroyAux_SStringListRecord1C_00cdc000(SStringListRecord1C*, SStringListRecord1C*, const SFalseType&) -- STLport __destroy_aux: destroy every 28-byte record of [first, last)
// Leaf: DestroyAux_SStringListRecord1C_00cdc000
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDC000, length 91 bytes.
// Callees: ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// STLport `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (__fastcall: first in ecx, last in
// edx, the empty __false_type reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);`
// with the element's implicit destructor inlined. The 28-byte element is {CCharString, 8 bytes of plain data,
// CCharString, vector<CCharString>}; members die in reverse order: the vector's strings (4-byte stride), its
// non-null storage (free), the CCharString at +0xC, then the one at +0. The manifest label
// (`_Move_backward<pair<unsigned long, CDefString>*>`) is propagated: there is no move (no destination, `ret 4`).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

struct CCharString {
    void* m_pData;                                         // +0
    ~CCharString();                                        // 0x0099EAE0
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

struct SStringListRecord1C {                               // 28-byte element
    CCharString                 m_name;                    // +0x00
    long                        m_data[2];                 // +0x04
    CCharString                 m_label;                   // +0x0C
    CRetailVector<CCharString>  m_strings;                 // +0x10
};

struct SFalseType { };

void __fastcall DestroyAux_SStringListRecord1C_00cdc000(SStringListRecord1C* first, SStringListRecord1C* last, const SFalseType& tag)
{
    for (; first != last; ++first)
        first->~SStringListRecord1C();
}