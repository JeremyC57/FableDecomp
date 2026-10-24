// _global::DestroyAux_SNestedVectorRecord10_00722130(SNestedVectorRecord10*, SNestedVectorRecord10*, const SFalseType&) -- STLport __destroy_aux: destroy every 16-byte record of [first, last)
// Leaf: DestroyAux_SNestedVectorRecord10_00722130
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722130, length 85 bytes.
// Callees: free @ 0x00BFEA14.
// STLport `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (__fastcall: first in ecx, last in
// edx, the empty __false_type reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);`
// with the element's implicit destructor inlined. The 16-byte element is a key followed by an STLport vector of
// 20-byte records at +4; each record holds a vector of plain data at +8, so destroying a record only frees that
// inner storage (`if (_M_start) free(_M_start)`), and then the outer vector frees its own storage the same way.
// The manifest label (`_Move_backward<pair<unsigned long, CDefString>*>`) is propagated: there is no move
// (no destination, `ret 4`).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

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

struct SVectorRecord14 {                                   // 20-byte inner record
    long                  m_key[2];                        // +0x00
    CRetailVector<long>   m_values;                        // +0x08
};

struct SNestedVectorRecord10 {                             // 16-byte element
    unsigned long                    m_key;                // +0x00
    CRetailVector<SVectorRecord14>   m_records;            // +0x04
};

struct SFalseType { };

void __fastcall DestroyAux_SNestedVectorRecord10_00722130(SNestedVectorRecord10* first, SNestedVectorRecord10* last, const SFalseType& tag)
{
    for (; first != last; ++first)
        first->~SNestedVectorRecord10();
}