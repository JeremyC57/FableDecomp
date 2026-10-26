// std::sort_heap(SBindingPair* first, SBindingPair* last, CCompareString comp) -- SGI sort_heap over (name, CCountedPointer<CEntityScriptBindingBase>) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBC580, length 130 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32>>*,...>` is
// propagated over the byte-identical skeleton. The only non-string callee is the landed SGI `__pop_heap` @ 0x00CBBAD0
// of pair<CCharString, CCountedPointer<CEntityScriptBindingBase>> under CCompareString (stride 12); element model reused.
// Callees: __pop_heap @ 0x00CBBAD0 (out of line, __fastcall, `ret 18h`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the name string, then the
// counted pointer's inline copy (both words, `test ; je ; inc [count]`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
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

class CEntityScriptBindingBase;

struct SBindingPair {                                   // std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >, 12 bytes
    CCharString                               first;    // +0x00
    CCountedPointer<CEntityScriptBindingBase> second;   // +0x04
    SBindingPair& operator=(const SBindingPair& other);   // 0x00CBA700 (out of line)
};

struct CCompareString { };                              // CCompareString<CCountedPointer<CEntityScriptBindingBase> >, empty, by value

void __fastcall __pop_heap(SBindingPair* first, SBindingPair* last, SBindingPair* result, SBindingPair value,
                           CCompareString comp, long*);   // 0x00CBBAD0

inline void __pop_heap_aux(SBindingPair* first, SBindingPair* last, SBindingPair*, CCompareString comp)
{
    __pop_heap(first, last - 1, last - 1, SBindingPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SBindingPair* first, SBindingPair* last, CCompareString comp)
{
    __pop_heap_aux(first, last, (SBindingPair*)0, comp);
}

void __fastcall sort_heap(SBindingPair* first, SBindingPair* last, CCompareString comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}