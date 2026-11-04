// CVectorMap::Insert(const unsigned long& key, const CDefClassInfo& value) -- CVectorMap<unsigned long,CDefinitionManager::CDefClassInfo>::Insert: append the (index, class info) pair to the backing vector and mark the map dirty (unsorted)
// Flags: /Gr
// Leaf: ?Insert@?$CVectorMap@KVCDefClassInfo@CDefinitionManager@@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9290, 101 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: the CVectorMap<CCharString,unsigned long>::Insert shape (row 0x009B83B0, FableWin.pdb
//   ?Insert@?$CVectorMap@...@@QAEXABV..@ABK@Z) over the pair<unsigned long, CDefinitionManager::CDefClassInfo> vector of
//   0x009B7320: Ego_r.pdb CDefinitionManager::DefClassInfo (+132) is this CVectorMap, and FableWin.pdb has
//   ?Insert@?$CVectorMap@KVCDefClassInfo@CDefinitionManager@@...@@QAEXABKABVCDefClassInfo@CDefinitionManager@@@Z; Dirty at
//   +0xD.  No direct callers in the xref scan.  Catalogued as `StdVector_Insert_Complex_Pair`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, vector<unsigned long>(const vector&) @ 0x00535470,
//   vector<pair<...>>::push_back @ 0x009B9170 (out of line for this 24-byte element), free @ 0x00BFEA14 /
//   ~CCharString @ 0x0099EAE0 (the temporary).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the temporary built in the frame, destroyed before the store.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

#include "engine/CArray.h"


class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                               // Ego_r.pdb (24 bytes; retail 20: CArray 16 -> 12)
    public:
        CCharString            Name;                    // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();          // +0x04
        CArray<unsigned long>  GlobalIndices;           // +0x08
    };
};

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    void Insert(const K& key, const V& value);
    C    Compare;                                       // +0x0C
    bool Dirty;                                         // +0x0D
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V, class C> void CVectorMap<K, V, C>::Insert(const K& key, const V& value)
{
    this->push_back(std::pair<K, V>(key, value));
    Dirty = true;
}

template void CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo>::Insert(const unsigned long&,
                                                                                   const CDefinitionManager::CDefClassInfo&);