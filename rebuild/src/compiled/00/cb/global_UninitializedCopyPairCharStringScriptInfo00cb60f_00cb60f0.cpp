// _global::UninitializedCopy_PairCharStringScriptInfo_00cb60f0(pair<CCharString,CScriptInfo>*, pair<..>*, pair<..>*, const __false_type&) -- STLport __uninitialized_copy: placement copy-construct the script registry's (name, script info) pairs
// Leaf: UninitializedCopy_PairCharStringScriptInfo_00cb60f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB60F0, length 106 bytes (+ int3 padding).
// STLport `__uninitialized_copy(first, last, result, const __false_type&)` (/Gr __fastcall: first in ecx, last in
// edx, result + the trait reference on the stack, `ret 8`): a pointer-compare loop that placement-copy-constructs each
// element (`_Construct` = placement new, so the destination gets a null test) and returns the end of the built range.
// Element: std::pair<CCharString, CScriptInfo> (0x18; Ego_r.pdb CScriptInfo: AllocFunc +0, MasterScriptDataAlloc +4,
// CCharString Name +8, long ID +0xC, bool MasterScript +0x10). STLport's pair copy constructor is user-defined
// (`first(p.first), second(p.second)`); the key and Name go through the out-of-line CCharString copy constructor
// @ 0x0099EC30. The manifest's `_Copy_backward<pair<long,CRegisteredMusicEntry>*>` label is propagated (the layout is
// the CScriptInfo pair of 0x00CB62F0 in the same TU).
#include <new>

class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString(const CCharString& other);   // 0x0099EC30
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo { public:                         // Ego_r.pdb, 0x14 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                   // +0x08
    long             ID;                     // +0x0C
    bool             MasterScript;           // +0x10
};

struct PairCharStringScriptInfo {            // std::pair<CCharString, CScriptInfo>
    CCharString first;                       // +0x00
    CScriptInfo second;                      // +0x04
    PairCharStringScriptInfo(const PairCharStringScriptInfo& p) : first(p.first), second(p.second) { }
};

struct __false_type { __false_type() { } };

inline void _Construct(PairCharStringScriptInfo* p, const PairCharStringScriptInfo& value)
{
    new (p) PairCharStringScriptInfo(value);
}

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringScriptInfo* __fastcall UninitializedCopy_PairCharStringScriptInfo_00cb60f0(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result, const __false_type&)
{
    PairCharStringScriptInfo* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}