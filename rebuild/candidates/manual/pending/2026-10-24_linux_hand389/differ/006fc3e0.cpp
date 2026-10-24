// std::__linear_insert(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp)
// Leaf: __linear_insert
// Grade: DIFFER(139v137) -- retail loads the key straight into ebx (`mov ebx,[esp+8]`); ours loads it into ecx and copies it (`mov ebx,ecx`), which shifts ecx/edx in the else branch (the delete[] / push of the temporary)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC3E0, 137 bytes, `ret 0x10` (/Gr: first, last in ecx/edx; val by value; comp).
// Label correction: catalog "Vector_Erase_WithMove" -> std::__linear_insert of the keyed event vector (called from
//   __insertion_sort 0x006FCB30).
// Callees: __copy_backward 0x006FB830 (same-TU noinline body so the empty tag shares a dead slot), CSmallVector::Copy
//   0x006FB5C0, __unguarded_linear_insert 0x006FB960, operator delete[] 0x00BFEB1C.
// Tried (4): the STLport original `if (comp(val, *first)) { copy_backward(...); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);`; the same with the __copy_backward body in the TU (fixed the tag
//   slot); this key-local form; a `pair(key, val.second)` constructor for the else-branch copy -- all 139 bytes.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

template<class T, int N> struct CSmallVector {
    T* Start;                                   // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    ~CSmallVector() { Clear(); }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedEvents& a, const SKeyedEvents& b) const { return a.first < b.first; }
};

struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

#pragma auto_inline(off)
// 0x006FB830 (landed separately; same-TU body so the empty tag can share a dead slot)
template<class RandomAccessIter, class BidirectionalIter, class Distance>
__declspec(noinline) BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last,
                                         BidirectionalIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = Distance(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
#pragma auto_inline(on)
void __fastcall __unguarded_linear_insert(SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp); // 0x006FB960

inline SKeyedEvents* copy_backward(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (long*)0);
}

void __fastcall __linear_insert(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp)
{
    unsigned long key = val.first;
    if (key < first->first) {
        copy_backward(first, last, last + 1);
        first->first = key;
        first->second = val.second;
    }
    else
        __unguarded_linear_insert(last, SKeyedEvents(val), comp);
}
