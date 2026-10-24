// _global::__pop_heap_aux(PairCharStringULong* first, PairCharStringULong* last, PairCharStringULong*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (name, unsigned long) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6D80, length 109 bytes, `ret 8` (the unused T* and the empty comparator on the
//   stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 8 bytes (`sar esi,3`) = { CCharString, dword }; the callee 0x009B5C70 carries the manifest name
//   `_Adjust_heap<pair<CCharString,unsigned long>*,int,...,CKeyPairCompareLess<CCharString,unsigned long>>`, so the row's
//   `_Pop_heap_0<pair<CCharString,C3DHelperPoint>*,...>` label (a 0x14-byte element) is propagated. Byte twins: 0x009B6D80 /
//   0x00A41260 (one instantiation per TU, different adjust callees).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x2), CCharString::operator= @ 0x0099EFB0,
//   __adjust_heap @ 0x009B5C70, CCharString::~CCharString @ 0x0099EAE0.
// Shape: STLport
//     __pop_heap_aux(first, last, T*, comp) { __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, __DISTANCE_TYPE(first)); }
//   with __pop_heap (`*result = *first; __adjust_heap(first, 0, last - first, value, comp);`) inlined: the T(*(last - 1))
//   temporary becomes the frame local (`sub esp,8`), the pair's implicit assignment writes the root into the back slot, the
//   copy of the local for __adjust_heap is built in the argument area, and the local is destroyed at the end.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct PairCharStringULong {                            // std::pair<CCharString, unsigned long>, 8 bytes
    CCharString   first;                                // +0x00
    unsigned long second;                               // +0x04
    PairCharStringULong(const PairCharStringULong& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<CCharString, unsigned long>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairCharStringULong* first, long holeIndex, long len, PairCharStringULong value,
                              CKeyPairCompareLess comp);   // 0x009B5C70

inline void __fastcall __pop_heap(PairCharStringULong* first, PairCharStringULong* last, PairCharStringULong* result,
                                  PairCharStringULong value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairCharStringULong* first, PairCharStringULong* last, PairCharStringULong*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairCharStringULong(*(last - 1)), comp, (long*)0);
}