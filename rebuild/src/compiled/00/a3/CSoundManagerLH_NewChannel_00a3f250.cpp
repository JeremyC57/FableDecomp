// CSoundManagerLH::NewChannel() -- add a channel to a named atmos
// Name (agentSND, 2026-11-10): vtable slot 39 (+0x9C); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was AddSound.
// Leaf: ?NewChannel@CSoundManagerLH@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3F250, 235 bytes (+ int3 padding), `ret 14h`. Vtable 0x0129D2C4 (CSoundManagerLH,
//   rtti-vtables.tsv) slot 39. Descriptive name: the audio layer is absent from both PDBs (as the landed siblings
//   AddBank 0x00A3F140 slot 37, FindByName 0x00A3ED30, FindByNamePair 0x00A3EE50); the catalog has no label.
// Callees: CSoundManagerLH::FindByNamePair 0x00A3EE50, CSoundManagerLH::FindByName 0x00A3ED30, CASoundManager::GetBank
//   0x00991840 (on this), the bank's slot 1 (+4), pair<CCharString, CCharString>(const CCharString&, const CCharString&)
//   0x0076DD40 (out of line), CCharString(const CCharString&) 0x0099EC30, vector<pair<pair<..>, ..> >::push_back 0x00A41840,
//   ~CCharString 0x0099EAE0, CVectorMap<pair<CCharString, CCharString>, ..>::Sort 0x00A42100 (out of line).
// Shape: three early `return false` exits each with their own epilogue; the sound rides in ebx; CVectorMap::Add is inlined
//   (the key pair built out of line in the frame, the value pair copy-built from it with the sound, pushed, destroyed, Dirty
//   set, then the key destroyed at the end of the statement), the map then re-sorted out of line.
// Retail layout: the name-pair map CVectorMap at +0x54 (12-byte vector, comparator +0x60, Dirty +0x61), as AddBank's model.
// Fix (chunk 755): the sample id read into a named local before the bank call gives retail's edx/eax register choice in the Add.
#include "fable_compiler.h"
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CSoundBankLH;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <> pair<CCharString, CCharString>::pair(const CCharString& __a, const CCharString& __b);   // 0x0076DD40

template <class _Tp> class allocator { public: allocator() { } };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void push_back(const _Tp& __x);                         // 0x00A41840 (out of line here)
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    CKeyPairCompareLess() { }
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                           // +0x00
    C                        Comp;                          // +0x0C (empty comparator)
    bool                     Dirty;                         // +0x0D
    FABLE_FORCEINLINE void Add(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort();                                            // 0x00A42100 (out of line for the pair map)
};

class CASoundBank {                                         // what GetBank returns (landed 0x00991840 spelling)
public:
    unsigned char _pad[0x10];
    long          Id;                                       // +0x10
};
class CSoundBankLH {                                        // descriptive
public:
    virtual void v00();
    virtual CSoundBankLH* StartSound(long sampleId, long a, long b);   // slot 1 (+0x04); the object filed in the pair map
};
class CASoundManager {                                  // landed 0x00991840 owner; called on this unadjusted
public:
    virtual void v00();
    CASoundBank* GetBank(long id);                          // 0x00991840
};

class CSoundManagerLH : public CASoundManager {         // vtable 0x0129D2C4
public:
    unsigned char _04[0x54 - 0x04];                         // +0x04
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);   // 0x00A3EE50
    CSoundBankLH* FindByName(const CCharString& name);      // 0x00A3ED30
    virtual bool NewChannel(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b);
};

bool CSoundManagerLH::NewChannel(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b)
{
    if (FindByNamePair(bankName, soundName))
        return false;
    CSoundBankLH* bank = FindByName(bankName);
    if (!bank)
        return false;
    CASoundBank* info = GetBank(sample);
    if (!info)
        return false;
    long sampleId = info->Id;
    CSoundBankLH* sound = bank->StartSound(sampleId, a, b);
    if (!sound)
        return false;
    NamePairMap.Add(std::pair<CCharString, CCharString>(bankName, soundName), sound);
    NamePairMap.Sort();
    return true;
}