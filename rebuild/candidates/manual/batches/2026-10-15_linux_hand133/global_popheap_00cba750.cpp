// _global::__pop_heap(SPersistPair* first, SPersistPair* last, SPersistPair* result, SPersistPair value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (UID, persist info) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA750, length 137 bytes (+ int3 padding) (`ret 24h`: result, the 0x18-byte value, the
// comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CEditPrimitiveStats::CEditPrimitiveStats" is propagated and wrong; the callee 0x00CB9FA0 carries the
// manifest name `_Adjust_heap<std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,int,...,CKeyPairCompareLess<...>>`,
// so this is the matching __pop_heap of the same instantiation (the landed 0x00A70280 is the pair<CCharString, C3DDummyObject> twin).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// __adjust_heap @ 0x00CB9FA0, CCharString::~CCharString @ 0x0099EAE0.
// Element: pair<unsigned __int64, CPersistInfo>, 0x18 bytes (Ego_r.pdb CPersistInfo: UID +0x00, PersistScript +0x08; +0x14 is padding).
// Shape: the pair's IMPLICIT assignment and copy constructor are inlined (four dword moves, then the string through its out-of-line
// member); the element count is the reciprocal multiply by 0x2AAAAAAB with `sar edx,2` (divide by 24); the by-value `value` parameter
// is destroyed by this callee at the end (only its CCharString has a destructor).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CPersistInfo {                        // NScript::CRuntimeScriptedEntityPersist::CPersistInfo (0x10 bytes)
    unsigned __int64 UID;                    // +0x00
    CCharString      PersistScript;          // +0x08
};

struct SPersistPair {                        // std::pair<unsigned __int64, CPersistInfo> (0x18 bytes, 8-aligned)
    unsigned __int64 first;                  // +0x00
    CPersistInfo     second;                 // +0x08
};

struct CKeyPairCompareLess {                 // CKeyPairCompareLess<unsigned __int64, CPersistInfo>: empty, passed by value
    bool operator()(const SPersistPair& a, const SPersistPair& b) const { return a.first < b.first; }
};

void __fastcall __adjust_heap(SPersistPair* first, long holeIndex, long len, SPersistPair value,
                              CKeyPairCompareLess comp);   // 0x00CB9FA0

void __fastcall __pop_heap(SPersistPair* first, SPersistPair* last, SPersistPair* result, SPersistPair value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}
