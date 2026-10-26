// std::sort_heap(SKeyedPair* first, SKeyedPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over (unsigned long key, named counted pointer) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34D80, length 111 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
// skeleton (twin of 0x00A72AA0). The only non-string callee is the landed SGI `__pop_heap` @ 0x00A34420 over 16-byte
// (unsigned long, {CCharString, CCountedPointer<CFaction>}) pairs, whose element model is reused here.
// Callees: __pop_heap @ 0x00A34420 (out of line, __fastcall, `ret 1Ch`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride: `and -16 ; cmp 16`). The value copy in the argument area is the pair's
// implicit copy: the key, then the record's implicit copy through its own address (`lea edi,[eax+4]`: the string
// copy constructor, then CCountedPointer's inline copy -- both words, `test ; je ; inc [count]`).
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

struct SKeyedPair {                                     // std::pair<unsigned long, SNamedPointer>, 0x10 bytes
    unsigned long first;                                // +0x00
    SNamedPointer second;                               // +0x04
};

struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __pop_heap(SKeyedPair* first, SKeyedPair* last, SKeyedPair* result, SKeyedPair value,
                           CKeyPairCompareLess comp, long*);   // 0x00A34420

inline void __pop_heap_aux(SKeyedPair* first, SKeyedPair* last, SKeyedPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SKeyedPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SKeyedPair* first, SKeyedPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SKeyedPair*)0, comp);
}

void __fastcall sort_heap(SKeyedPair* first, SKeyedPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}