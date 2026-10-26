// std::sort_heap(SNamedKeyedPair* first, SNamedKeyedPair* last, CCompareString comp) -- SGI sort_heap over (CCharString key, named counted pointer) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72B10, length 115 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<float,CCharString>*,CCompareString<float,CCharString>>` is
// propagated over the byte-identical skeleton. The callee 0x00A71BE0 (manifest "std_Adjust_Heap_Pair") is the SGI
// `__pop_heap` of a 16-byte pair: `*result = *first` runs CCharString::operator= on the key and the out-of-line
// record assignment 0x00A32960 (the {CCharString, CCountedPointer<CFaction>} record of the landed 0x00A71BE0
// pop_heap) on the rest, then forwards to __adjust_heap 0x00A71370 (`ret 1Ch`). So the element is
// pair<CCharString, {CCharString Name, CCountedPointer<CFaction> Pointer}>; record model from 0x00A71BE0.
// Callees: __pop_heap @ 0x00A71BE0 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride). The value copy in the argument area is the implicit pair copy: the key
// string, then the record's implicit copy through its own address (`add edi,4`: string copy, then the counted
// pointer's inline copy -- both words, `test ; je ; inc [count]`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4 per-type deleter, object in ecx
    void* m_pObject;                                    // +8
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                                       // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class CFaction;

struct SNamedPointer {                                  // 12 bytes
    CCharString                 Name;                   // +0x00
    CCountedPointer<CFaction>   Pointer;                // +0x04
    SNamedPointer& operator=(const SNamedPointer& other);   // 0x00A32960 (out of line)
};

struct SNamedKeyedPair {                                // std::pair<CCharString, SNamedPointer>, 0x10 bytes
    CCharString   first;                                // +0x00
    SNamedPointer second;                               // +0x04
};

struct CCompareString { };                              // empty, passed by value

void __fastcall __pop_heap(SNamedKeyedPair* first, SNamedKeyedPair* last, SNamedKeyedPair* result, SNamedKeyedPair value,
                           CCompareString comp, long*);   // 0x00A71BE0

inline void __pop_heap_aux(SNamedKeyedPair* first, SNamedKeyedPair* last, SNamedKeyedPair*, CCompareString comp)
{
    __pop_heap(first, last - 1, last - 1, SNamedKeyedPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SNamedKeyedPair* first, SNamedKeyedPair* last, CCompareString comp)
{
    __pop_heap_aux(first, last, (SNamedKeyedPair*)0, comp);
}

void __fastcall sort_heap(SNamedKeyedPair* first, SNamedKeyedPair* last, CCompareString comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}