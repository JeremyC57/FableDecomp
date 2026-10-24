// _global::UninitCopy_PairCharStringPersistInfo_004b8210(const SPersistPair* first, const SPersistPair* last, SPersistPair* result, const __false_type&) -- STLport __uninitialized_copy over CRuntimeQuestPersistence's 12-byte pair<CCharString, CPersistInfo> map entries: placement-copy each entry (three strings through the out-of-line CCharString copy), return the advanced result
// Leaf: UninitCopy_PairCharStringPersistInfo_004b8210
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8210, length 73 bytes (+ int3 padding), `ret 8`.
// Callers: the STLport vector::_M_insert_overflow @ 0x004BB400 (calls 0x004BB46D / 0x004BB4DC) behind the push_back of the
//   landed NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript @ 0x004B5080.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (three times per element).
// Layout (from the landed 0x004B5080 / 0x004B4E30 models): the CVectorMap entry pair<CCharString key +0x0,
//   CRuntimeQuestPersistence::CPersistInfo { QuestName +0x4, PersistScript +0x8 }>.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack); first/last/cur in esi/ebp/edi. STLport's explicit pair
//   copy constructor copies the key, then CPersistInfo through its copy constructor, whose `this` (dest+4) is kept in ebx
//   across the first member call (`lea ebx,[edi+4] ; mov ecx,ebx` ... `lea ecx,[ebx+4]`): the value is a nested record
//   with its own copy, a flat three-string record addresses all three members from edi (73 v 73, register-only diff).
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, unsigned int*, ..>` is propagated.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);     // 0x0099EC30
    void* PData;                               // +0x0
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    struct CPersistInfo {                      // 8 bytes
        CCharString QuestName;                 // +0x0
        CCharString PersistScript;             // +0x4
    };
};
}
typedef std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> SPersistPair;   // 12 bytes

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

SPersistPair* __fastcall UninitCopy_PairCharStringPersistInfo_004b8210(const SPersistPair* first, const SPersistPair* last, SPersistPair* result, const __false_type&)
{
    SPersistPair* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}