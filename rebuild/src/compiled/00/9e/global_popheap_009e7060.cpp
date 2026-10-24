// _global::__pop_heap(PairCharStringSymbolInfo* first, PairCharStringSymbolInfo* last, PairCharStringSymbolInfo* result, PairCharStringSymbolInfo value, CCompareString comp, long*) -- SGI pop_heap core for the symbol map's (name, symbol info) heap: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7060, length 158 bytes (+ int3 padding), `ret 1Ch` (result, the 0x10-byte
//   value, the comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is pair<CCharString, CSymbolMap::CSymbolInfo> (0x10 bytes, `sar esi,4`); the callee 0x009E6DB0 carries
//   the manifest name `_Adjust_heap<pair<CCharString,CSymbolMap::CSymbolInfo>*,...,CCompareString<CSymbolMap::CSymbolInfo>>`.
//   The row's `_Pop_heap_0` label is the aux front end's name; this body is the out-of-line core (callers 0x009E71C2,
//   0x009E7662, 0x009E79A9, 0x009E7BA1).
// Callees: CCharString::operator= @ 0x0099EFB0 (x3), CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3),
//   __adjust_heap @ 0x009E6DB0, CCharString::~CCharString @ 0x0099EAE0 (x3).
// Layout: Ego_r.pdb CSymbolMap::CSymbolInfo (0xc): CCharString GroupName +0, CCharString EntryName +4, long Val +8.
// Shape: the pair's IMPLICIT assignment is inlined (three string operator= calls, the nested record addressed through its own
//   pointer pair `lea ebx,[edi+4]` / `add ebp,4`, then Val), the by-value copy for __adjust_heap is built in the argument
//   area by STLport's explicit pair copy constructor and the record's implicit one, and the by-value `value` parameter is
//   destroyed by this callee at the end (EntryName, GroupName, then the key).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace CSymbolMap {
struct CSymbolInfo {                                    // Ego_r.pdb, 0xc bytes
    CCharString GroupName;                              // +0x00
    CCharString EntryName;                              // +0x04
    long        Val;                                    // +0x08
};
}

struct PairCharStringSymbolInfo {                       // std::pair<CCharString, CSymbolMap::CSymbolInfo>, 0x10 bytes
    CCharString             first;                      // +0x00
    CSymbolMap::CSymbolInfo second;                     // +0x04
    PairCharStringSymbolInfo(const PairCharStringSymbolInfo& other) : first(other.first), second(other.second) { }
};

struct CCompareString {                                 // CCompareString<CSymbolMap::CSymbolInfo>, empty
    CCompareString() { }
};

void __fastcall __adjust_heap(PairCharStringSymbolInfo* first, long holeIndex, long len, PairCharStringSymbolInfo value,
                              CCompareString comp);   // 0x009E6DB0

void __fastcall __pop_heap(PairCharStringSymbolInfo* first, PairCharStringSymbolInfo* last, PairCharStringSymbolInfo* result,
                           PairCharStringSymbolInfo value, CCompareString comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}