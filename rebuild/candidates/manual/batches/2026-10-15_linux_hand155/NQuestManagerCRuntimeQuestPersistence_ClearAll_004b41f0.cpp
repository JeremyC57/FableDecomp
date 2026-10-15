// NQuestManager::CRuntimeQuestPersistence::ClearAll() -- empty both persist-script maps (erase every entry, then release their storage)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B41F0, length 107 bytes. Only caller 0x004B508E.
// Label: the catalog's "CQuestManager::RotateQuestPersistenceVectors" is descriptive; `this` is the
// CRuntimeQuestPersistence (CQuestManager +0xc) whose two CVectorMap<CCharString, CPersistInfo> members PersistScripts
// (+0x00) and MasterDataPersistScripts (+0x10; retail maps are 16 bytes: vector, comparator, Dirty +0xd) are cleared.
// Callees: STLport __copy(first, last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x004B84F0 (`__fastcall`),
// __destroy_aux(first, last, __false_type) @ 0x004B93E0 (`__fastcall`), CVectorMap::Clear @ 0x004BB6B0 (which
// itself erases, swaps the storage out and frees it).
// Shape: per map an inline member doing STLport erase(begin(), end()) (both empty tags share byte 3 of the `push ecx`
// frame word; written at the call site the second map re-reads its finish for __copy)
// then the out-of-line Clear; `this` moves to the second map with `add esi,10h`.
// Leaf: ClearAll
#include <stddef.h>

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

struct CCharString { void* PStringData; };

namespace NQuestManager {

struct CPersistPair {                               // pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>, 12 bytes
    CCharString first;
    CCharString second[2];
};

CPersistPair* __fastcall CopyPairs(CPersistPair* first, CPersistPair* last, CPersistPair* result,
                                   const random_access_iterator_tag& tag, ptrdiff_t* distance);   // 0x004B84F0
void __fastcall DestroyPairs(CPersistPair* first, CPersistPair* last, const __false_type& tag);    // 0x004B93E0

struct CPersistMap {                                // CVectorMap<CCharString, CPersistInfo>
    CPersistPair* _M_start;                         // +0x00
    CPersistPair* _M_finish;                        // +0x04
    CPersistPair* _M_end_of_storage;                // +0x08
    unsigned char Compare;                          // +0x0c
    bool          Dirty;                            // +0x0d
    CPersistPair* begin() { return _M_start; }
    CPersistPair* end() { return _M_finish; }
    CPersistPair* erase(CPersistPair* first, CPersistPair* last)
    {
        CPersistPair* i = CopyPairs(last, _M_finish, first, random_access_iterator_tag(), (ptrdiff_t*)0);
        DestroyPairs(i, _M_finish, __false_type());
        _M_finish = i;
        return first;
    }
    void Clear();                                   // 0x004BB6B0
    void EraseAndClear()
    {
        erase(begin(), end());
        Clear();
    }
};

struct CRuntimeQuestPersistence {
    CPersistMap PersistScripts;                     // +0x00
    CPersistMap MasterDataPersistScripts;           // +0x10
    void ClearAll();
};

void CRuntimeQuestPersistence::ClearAll()
{
    PersistScripts.EraseAndClear();
    MasterDataPersistScripts.EraseAndClear();
}

} // namespace NQuestManager
