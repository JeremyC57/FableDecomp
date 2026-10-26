// _global::DestroyAux_SPointerListRecord1C_006f44e0(SPointerListRecord1C*, SPointerListRecord1C*, const SFalseType&) -- STLport __destroy_aux: destroy every 28-byte record of [first, last)
// Leaf: DestroyAux_SPointerListRecord1C_006f44e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F44E0, length 77 bytes.
// Callees: ~CBaseIntelligentPointer @ 0x00A01C10, free @ 0x00BFEA14.
// STLport `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (__fastcall: first in ecx, last in
// edx, the empty __false_type reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);`
// with the element's implicit destructor inlined. The 28-byte element is 16 bytes of plain data followed by an
// STLport vector of 28-byte records at +0x10; the vector's `_Destroy(_M_start, _M_finish)` runs each record's
// destructor (only the intelligent pointer at +0x14 -- {vptr, PItem}, out-of-line base destructor, no vtable store
// -- is non-trivial), then the `_Vector_base` destructor frees non-null storage (STLport's malloc allocator).
// The manifest label (`_Move_backward<pair<unsigned long, CDefString>*>`) is propagated: there is no move
// (no destination, `ret 4`).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

class CThing;

struct CBaseIntelligentPointer {                           // 8 bytes (PDB)
    virtual void v00();
    CThing* PItem;                                         // +0x04
    ~CBaseIntelligentPointer();                            // 0x00A01C10
};

struct SPointerRecord1C {                                  // 28-byte inner record
    long                    m_data[5];                     // +0x00
    CBaseIntelligentPointer m_pThing;                      // +0x14
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

struct SPointerListRecord1C {                              // 28-byte element
    long                              m_header[4];         // +0x00
    CRetailVector<SPointerRecord1C>   m_records;           // +0x10
};

struct SFalseType { };

void __fastcall DestroyAux_SPointerListRecord1C_006f44e0(SPointerListRecord1C* first, SPointerListRecord1C* last, const SFalseType& tag)
{
    for (; first != last; ++first)
        first->~SPointerListRecord1C();
}