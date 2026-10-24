// _STL::__insertion_sort(SShortKeyedRecord* first, SShortKeyedRecord* last, CShortKeyLess comp) -- STLport insertion sort over 12-byte records keyed by a signed short at +4, __linear_insert inlined: a value below the first element shifts the run up one slot (out-of-line __copy_backward) and goes first, anything else goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Grade: DIFFER(136v141) -- register assignment only: retail keeps first in edi, val.m_value0 in ebp and the key dword in ebx, ours first in ebp, val.m_value0 in ebx, the key in edi; retail's else arm re-reads [esi]/[esi+4] for the argument copy (ours reuses the registers) and its loop head is aligned behind `jmp ; lea ebx,[ebx]` (+5 bytes). The float third member (value homed in the frame dword) and the out-of-line __copy_backward body (tag over the live comparator slot) already match; next: SGI `__linear_insert(first, last, T*, comp)` with `T val = *last` inside, or a pair<long, {short-key word, float}> model
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007212D0, 141 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x0071ED80 __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*), 0x0071EC60
// __unguarded_linear_insert(last, value, comp) (`ret 10h`; it compares `cmp dx,[eax+4] ; jl`, the same signed short key).
// Callers: the introsort driver 0x0071E000 and the landed __final_insertion_sort 0x00721890.
// Label: catalogued as `_Insertion_sort1<pair<float,CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<..> >`
// (propagated: that pair is 28 bytes); the element is a 12-byte POD whose sort key is the signed short at +4
// (`cmp bx,[edi+4] ; jge`) inside a dword that is always moved whole; the other members are not recoverable beyond
// their dword slots, so it is modelled as { long; {short key; short}; long } with a memberwise copy constructor.
// Shape: the by-value `*i` is the memberwise copy held in ebp/ebx and the frame dword `[esp+1Ch]` (loaded straight off
// esi; a POD element block-copies through `mov eax,esi`, an STLport pair<long, POD> keeps all three in registers and
// spills first/last instead); `*first = val` is
// three dword stores; copy_backward's empty iterator tag is a const-reference temporary homed over the LIVE comparator
// slot (`lea ecx,[esp+28h]`) -- only with the real __copy_backward body defined out of line in the TU (GOTCHAS "empty
// tag over a LIVE argument slot").
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct SShortKeyWord {                    // 4-byte POD: the signed short sort key and its companion half-word
    short m_key;                          // +0x00 sort key (signed)
    short m_aux;                          // +0x02
};

struct SShortKeyedRecord {                // 12 bytes, explicit memberwise copy constructor
    long          m_value0;               // +0x00
    SShortKeyWord m_key;                  // +0x04
    float         m_value2;               // +0x08 (a float: VC7.1 keeps it in the value's frame home, not a register)
    SShortKeyedRecord(const SShortKeyedRecord& o) : m_value0(o.m_value0), m_key(o.m_key), m_value2(o.m_value2) { }
};

struct CShortKeyLess {                    // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const SShortKeyedRecord& a, const SShortKeyedRecord& b) const { return a.m_key.m_key < b.m_key.m_key; }
};

// The real __copy_backward body (0x0071ED80), out of line in the TU.
#pragma auto_inline(off)
__declspec(noinline) SShortKeyedRecord* __fastcall __copy_backward(SShortKeyedRecord* first, SShortKeyedRecord* last, SShortKeyedRecord* result,
                                                                   const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
#pragma auto_inline(on)

void __fastcall __unguarded_linear_insert(SShortKeyedRecord* last, SShortKeyedRecord value, CShortKeyLess comp);   // 0x0071EC60

inline SShortKeyedRecord* copy_backward(SShortKeyedRecord* first, SShortKeyedRecord* last, SShortKeyedRecord* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

inline void __linear_insert(SShortKeyedRecord* first, SShortKeyedRecord* last, SShortKeyedRecord value, CShortKeyLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(SShortKeyedRecord* first, SShortKeyedRecord* last, CShortKeyLess comp)
{
    if (first == last)
        return;
    for (SShortKeyedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
