// _STL::__insertion_sort(SLongKeyAt4Record* first, SLongKeyAt4Record* last, CKeyAt4Less comp) -- STLport insertion sort over 12-byte POD records keyed by a signed long at +4, __linear_insert inlined: a value below the first element shifts the run up one slot (out-of-line __copy_backward) and goes first, anything else goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADBE70, 135 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00ADAB20 __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) (landed as
// global_CopyBackwardSElement1200adab20), 0x00ADAA30 __unguarded_linear_insert(last, value, comp) (`ret 10h`).
// Callers: 0x00ADC0A9 / 0x00ADC0C7 (the __final_insertion_sort pair). Sibling shape: 0x007212D0 (short key, parked).
// Label: catalogued as `_Insertion_sort1<pair<float,CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<..> >`
// (propagated: that pair is 28 bytes); the element is a 12-byte POD whose sort key is the signed long at +4
// (`cmp ebx,[edi+4] ; jge`); its other members are not recoverable beyond their dword slots.
// Shape: the by-value `*i` is a POD block copy (`mov eax,esi` first) held in ebp/ebx and the value's frame home
// `[esp+1Ch]`; `*first = val` is three dword stores; the else arm block-copies `*i` again into the argument area
// (`mov ecx,esi ; mov eax,[ecx]`). copy_backward's empty iterator tag is a const-reference temporary homed over the
// LIVE comparator slot (`lea edx,[esp+28h]`) -- only with the real __copy_backward body defined out of line in the TU
// (GOTCHAS "empty tag over a LIVE argument slot"; an extern declaration gives the tag its own frame dword).
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct SLongKeyAt4Record {                // 12 bytes, POD
    long m_value0;                        // +0x00
    long m_key;                           // +0x04 sort key (signed)
    long m_value2;                        // +0x08
};

struct CKeyAt4Less {                    // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const SLongKeyAt4Record& a, const SLongKeyAt4Record& b) const { return a.m_key < b.m_key; }
};

// The real __copy_backward body (0x00ADAB20), out of line in the TU.
#pragma auto_inline(off)
__declspec(noinline) SLongKeyAt4Record* __fastcall __copy_backward(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record* result,
                                                                   const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
#pragma auto_inline(on)

void __fastcall __unguarded_linear_insert(SLongKeyAt4Record* last, SLongKeyAt4Record value, CKeyAt4Less comp);   // 0x00ADAA30

inline SLongKeyAt4Record* copy_backward(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

inline void __linear_insert(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record value, CKeyAt4Less comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(SLongKeyAt4Record* first, SLongKeyAt4Record* last, CKeyAt4Less comp)
{
    if (first == last)
        return;
    for (SLongKeyAt4Record* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}