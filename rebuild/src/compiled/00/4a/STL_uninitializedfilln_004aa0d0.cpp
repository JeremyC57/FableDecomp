// std::__uninitialized_fill_n(CTestQuest*, unsigned int, const CTestQuest&, const __false_type&) -- STLport uninitialised fill of n 28-byte test-quest records: copy-construct x into each slot from first (placement, null-tested); returns the end
// Flags: /Gr
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA0D0, 108 bytes, `ret 8` (__fastcall: first in ecx -> esi, n in edx ->
//   ebx, then the value reference and the __false_type tag reference on the stack).
// Callees: 0x0099EC30 CCharString copy ctor (six times: the compiler-generated CTestQuest copy constructor is inlined,
//   members in declaration order, the long ColourIndex copied through ecx).
// Caller: vector<CTestQuest>::_M_insert_overflow 0x004ADB50 (this chunk; `__uninitialized_fill_n(new_finish, fill_len,
//   x, __false_type())`).
// Layout: Ego_r.pdb CTestQuest (28 bytes): QuestName +0, HSPName +4, ColourIndex +8, DisplayName +0xC, IniFile +0x10,
//   OutiFile +0x14, QuestCardDefName +0x18.
// Shape: STLport `_ForwardIter __cur = __first; for ( ; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return
//   __cur;` (the try/unwind compiled out); unsigned n makes `__n > 0` the `test edx,edx ; jbe` entry test, and VC7.1
//   counts the loop down in ebx.
// Label correction: catalogued as `Vector_Uninitialized_Fill` (right idea); the element is CTestQuest.
#include <new>

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

struct __false_type { __false_type() { } };

class CTestQuest { public:
    CCharString QuestName;                          // +0x00
    CCharString HSPName;                            // +0x04
    long        ColourIndex;                        // +0x08
    CCharString DisplayName;                        // +0x0C
    CCharString IniFile;                            // +0x10
    CCharString OutiFile;                           // +0x14
    CCharString QuestCardDefName;                   // +0x18
};

inline void _Construct(CTestQuest* __p, const CTestQuest& __val) { new (__p) CTestQuest(__val); }

CTestQuest* __fastcall __uninitialized_fill_n(CTestQuest* __first, unsigned int __n, const CTestQuest& __x, const __false_type&)
{
    CTestQuest* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}