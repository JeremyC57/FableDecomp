// _global::UninitCopy_CVectorMap12_00674ce0(const CVectorMap12* first, const CVectorMap12* last, CVectorMap12* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte CVectorMap records: placement-copy each map (vector copy constructor out of line, then the Dirty flag), return the advanced result
// Leaf: UninitCopy_CVectorMap12_00674ce0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00674CE0, length 52 bytes, `ret 8`.
// Callers: the allocate-and-copy helper @ 0x00674EB0 (call 0x00674ED9) and the vector assignment @ 0x006753F0 (calls
//   0x00675447 / 0x006754D9) of the same TU.
// Callees: the 12-byte-element STLport vector copy constructor @ 0x00673E10 (thiscall, source pushed: zero the three
//   words, `malloc(n * 12)`, set start/finish/end, copy the elements).
// Layout: CVectorMap (16 bytes, the retail STLport layout of GOTCHAS 00a85360): vector Entries +0x0 (12 bytes), the empty
//   comparator +0xC (never copied), bool Dirty +0xD.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). first/last/cur in esi/ebx/edi around the call; the
//   implicit CVectorMap copy constructor calls the vector's copy constructor, copies nothing for the empty comparator and
//   then the Dirty byte (`mov al,[esi+0Dh] ; mov [edi+0Dh],al`).
// Label correction: the catalog's `_Uninit_copy<CFeatDef*,CFeatDef*,allocator<CFeatDef>>` is propagated (CFeatDef is 100
//   bytes in the PDB; this element is 16).
inline void* operator new(unsigned int, void* where) { return where; }

struct SEntry12 { long a, b, c; };             // 12-byte vector element (opaque here)

struct CEntryVector {                          // STLport vector<SEntry12>, 12 bytes
    SEntry12* _M_start;
    SEntry12* _M_finish;
    SEntry12* _M_end_of_storage;
    CEntryVector(const CEntryVector& other);   // 0x00673E10
};

struct CKeyPairCompareLess { };

struct CVectorMap12 {                          // CVectorMap<K, V, CKeyPairCompareLess<K, V> >, 16 bytes
    CEntryVector Entries;                      // +0x0
    CKeyPairCompareLess Compare;               // +0xC
    bool Dirty;                                // +0xD
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CVectorMap12* __fastcall UninitCopy_CVectorMap12_00674ce0(const CVectorMap12* first, const CVectorMap12* last, CVectorMap12* result, const __false_type&)
{
    CVectorMap12* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}