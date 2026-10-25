// std::vector<pair<CCharString,CScriptInfo> >::push_back(const pair<CCharString,CScriptInfo>&) -- STLport 4.6 vector::push_back for CScriptInfoManager's Scripts map (CVectorMap<CCharString, CScriptInfo>): placement-copy the 24-byte pair at the finish when there is room, else _M_insert_overflow(finish, x, __false_type(), 1, true)
// Leaf: ?push_back@?$vector@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$allocator@U?$pair@VCCharString@@VCScriptInfo@@@std@@@2@@std@@QAEXABU?$pair@VCCharString@@VCScriptInfo@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7210, 109 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB7210 = std::vector<pair<CCharString,CScriptInfo>>::push_back"): the
//   landed CScriptInfoManager::AddScript 0x00CB5C90 calls it through CVectorMap::Add on the Scripts map's vector (+4) at
//   0x00CB5D47 (rebuild/oracles/reloc_bindings.tsv already binds this leaf to 0x00CB7210); the element copy is
//   pair<CCharString, CScriptInfo> with Ego_r.pdb's CScriptInfo (AllocFunc +0, MasterScriptDataAlloc +4, Name +8, ID +0xc,
//   MasterScript +0x10): CCharString copy ctor 0x0099EC30 for first (+0) and second.Name (+0xc), words +4 / +8 / +0x10, byte
//   +0x14. Callee: _M_insert_overflow 0x00CB6FD0. Model: the landed AddScript row's STLport model.
// Label correction: the landed CStringPairRecordVector_00cb7210 over a by-address 24-byte SStringPairRecord is this vector.
// Shape: with room left, placement new under VC's null test copies the pair through its copy constructor, which must be
//   `__forceinline` (the landed row found the implicit and a plain inline one both emitted out of line here); then finish
//   is re-read and advanced by 24. The overflow path gets the empty __false_type temporary homed in the dead `x` slot.
#include <stddef.h>

inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData {
public:
    const char*   Data;                                 // +0x00
    unsigned long Length;                               // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                     // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator { };

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    __forceinline pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00CB6FD0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};
}

template void std::vector<std::pair<CCharString, CScriptInfo> >::push_back(const std::pair<CCharString, CScriptInfo>& __x);
