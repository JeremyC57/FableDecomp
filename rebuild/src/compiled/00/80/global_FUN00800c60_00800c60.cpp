// _global::FUN_00800c60(T*, T*) -- STLport vector<T>::erase(first, last) for a trivially destructible 9-byte element: slide the tail down, move _M_finish, return first
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800C60, 38 bytes, `ret 8`. Catalogued FUN_00800c60 (unreferenced; a masked twin
//   of the landed vector_push_back_copy 0x00411B00). Callee: the __fastcall random-access
//   __copy(last, _M_finish, first, random_access_iterator_tag(), (ptrdiff_t*)0) @ 0x008001F0 (manifest
//   _Move<CEngineTextureMorph*,...>; its loop copies 9-byte elements: two dwords and a byte). The element
//   destructor is trivial, so _Destroy(i, _M_finish) vanishes.
// Shape: the empty tag temporary is homed in the dead `first` argument slot (`lea eax,[esp+10h]` after first was
//   read into edi); `last` goes to ecx and _M_finish to edx.
struct SElement9;                                // 9-byte trivially destructible element

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SElement9* __fastcall __copy(SElement9* __first, SElement9* __last, SElement9* __result,
                             const random_access_iterator_tag&, long*);   // 0x008001F0

struct vector_SElement9 {
    SElement9* _M_start;                         // +0x00
    SElement9* _M_finish;                        // +0x04
    SElement9* _M_end_of_storage;                // +0x08
    SElement9* erase(SElement9* __first, SElement9* __last);
};

SElement9* vector_SElement9::erase(SElement9* __first, SElement9* __last)
{
    SElement9* __i = __copy(__last, _M_finish, __first, random_access_iterator_tag(), (long*)0);
    _M_finish = __i;
    return __first;
}