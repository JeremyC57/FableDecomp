// _global::__pop_heap_aux(PairFloatCharString* first, PairFloatCharString* last, PairFloatCharString*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (float, name) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0D60, length 110 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 8 bytes (`sar esi,3`) = { 4-byte key, CCharString }; the callee 0x004D0590 carries the manifest name
//   `_Adjust_heap<pair<float,CCharString>*,int,pair<float,CCharString>,CKeyPairCompareLess<float,CCharString>>`, so the row's
//   `_Pop_heap_0<pair<long,CMusicManager::CRegisteredMusicEntry>*,...>` label (a 0x18-byte element) is propagated.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x2), CCharString::operator= @ 0x0099EFB0,
//   __adjust_heap @ 0x004D0590, CCharString::~CCharString @ 0x0099EAE0.
// Shape: STLport __pop_heap_aux with __pop_heap inlined: the T(*(last - 1)) temporary is the 8-byte frame local (key copied
//   as a dword, string copy-constructed), the pair's implicit assignment writes the root into the back slot, the local's
//   copy for __adjust_heap is built in the argument area, and the local is destroyed at the end.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct PairFloatCharString {                            // std::pair<float, CCharString>, 8 bytes
    float       first;                                  // +0x00
    CCharString second;                                 // +0x04
    PairFloatCharString(const PairFloatCharString& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<float, CCharString>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairFloatCharString* first, long holeIndex, long len, PairFloatCharString value,
                              CKeyPairCompareLess comp);   // 0x004D0590

inline void __fastcall __pop_heap(PairFloatCharString* first, PairFloatCharString* last, PairFloatCharString* result,
                                  PairFloatCharString value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairFloatCharString* first, PairFloatCharString* last, PairFloatCharString*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairFloatCharString(*(last - 1)), comp, (long*)0);
}