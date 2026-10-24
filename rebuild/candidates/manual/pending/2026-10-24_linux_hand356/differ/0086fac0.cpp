// _STL::__insertion_sort(SLongKeyedRecord* first, SLongKeyedRecord* last, CKeyPairCompareLess comp) -- STLport insertion sort over 12-byte pair<long, {long, long}> records with __linear_insert inlined: a value below the first element shifts the run up one slot (out-of-line __copy_backward) and goes first, anything else goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Grade: DIFFER(122v122) -- only the copy_backward tag temporary: ours gives it its own frame dword (`sub esp,0Ch`, `lea eax,[esp+17h]`), retail homes it over the live comparator slot (`sub esp,8`, `lea eax,[esp+20h]`); next: define the real __copy_backward body out of line in the TU (`#pragma auto_inline(off)` + `__declspec(noinline)`), the fix that matched 004c5190/00adbe70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086FAC0, 122 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x0086EFF0 __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) (`ret 0Ch`),
// 0x0086F090 __unguarded_linear_insert(last, value, comp).
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (propagated); the element is
// 12 bytes (`add esi,0Ch`) keyed by a SIGNED long at +0 (`cmp edi,[ecx] ; jge`). The code is STLport's
// `__insertion_sort` with `__linear_insert(first, i, *i, comp)` inlined.
// Shape: the by-value `*i` lives in edi/ebx/ebp; `*first = val` is three dword stores; copy_backward's empty iterator tag
// is a const-reference temporary homed in the dead comparator slot (`lea eax,[esp+20h]`) next to a null Distance*;
// the unguarded insert gets the comparator and a fresh pair copy built in the argument area. `first` and `last` sit in
// the two frame words (`sub esp,8`).
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct SPayload8 {                        // second half of the record (POD)
    long m_value0;                        // +0x00
    long m_value1;                        // +0x04
};

struct SLongKeyedRecord {                 // std::pair<long, SPayload8>, 12 bytes
    long first;                           // +0x00 sort key (signed)
    SPayload8 second;                     // +0x04
    SLongKeyedRecord(const SLongKeyedRecord& o) : first(o.first), second(o.second) { }
};

struct CKeyPairCompareLess {              // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const SLongKeyedRecord& a, const SLongKeyedRecord& b) const { return a.first < b.first; }
};

SLongKeyedRecord* __fastcall __copy_backward(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord* result,
                                             const random_access_iterator_tag& tag, int* distance);   // 0x0086EFF0
void __fastcall __unguarded_linear_insert(SLongKeyedRecord* last, SLongKeyedRecord value, CKeyPairCompareLess comp);   // 0x0086F090

inline SLongKeyedRecord* copy_backward(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

inline void __linear_insert(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord value, CKeyPairCompareLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(SLongKeyedRecord* first, SLongKeyedRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SLongKeyedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
