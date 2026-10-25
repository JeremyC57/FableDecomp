// std::__uninitialized_fill_n(SNewQuestRecord*, unsigned int, const SNewQuestRecord&, const __false_type&) -- STLport uninitialised fill of n 12-byte records of three CCharStrings: copy-construct x into each slot from first (placement, null-tested); returns the end
// Flags: /Gr
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8260, 79 bytes (81 with padding), `ret 8` (__fastcall: first in ecx ->
//   esi, n in edx (the count kept in the `push ecx` frame word), then the value reference and the __false_type tag
//   reference on the stack).
// Callees: 0x0099EC30 CCharString copy ctor (three times: the record's compiler-generated copy constructor inlined,
//   members in declaration order).
// Caller: the record vector's _M_insert_overflow 0x004BB400 (`__uninitialized_fill_n(new_finish, fill_len, x,
//   __false_type())`), whose push_back callers 0x004B521D / 0x004B540D parse a quest list section ending in
//   "END_NEW_QUEST" (0x012394A0) into three strings per record.
// Layout: three CCharStrings (+0, +4, +8) nested as pair<CCharString, pair<CCharString, CCharString> >: the inlined copy
//   constructor builds the inner pair through its own this/other pair (`lea ebx,[esi+4]` / `lea edi,[ebp+4]`, then
//   `+4` on each), which a flat three-member struct does not give (69 v 81, the count then stays in a register). No PDB
//   name set carries this record in the quest code, so the typedef name is descriptive.
// Shape: STLport `_ForwardIter __cur = __first; for ( ; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return
//   __cur;` (the try/unwind compiled out), as the CTestQuest twin 0x004AA0D0.
// Label correction: catalogued as `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` (propagated); the element is the
//   three-string record.
#include <new>

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {      // STLport pair (declared copy constructor)
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
typedef pair<CCharString, pair<CCharString, CCharString> > SNewQuestRecord;   // +0 first, +4 second.first, +8 second.second

inline void _Construct(SNewQuestRecord* __p, const SNewQuestRecord& __val) { new (__p) SNewQuestRecord(__val); }

SNewQuestRecord* __fastcall __uninitialized_fill_n(SNewQuestRecord* __first, unsigned int __n, const SNewQuestRecord& __x, const __false_type&)
{
    SNewQuestRecord* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}