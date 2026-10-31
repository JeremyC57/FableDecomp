// CSoundManagerLH::vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b) -- register a new named sound: refuse a (bank, sound) pair that already exists, an unknown bank or sample, or a failed start; else record the started object under the pair in the sorted pair map
// Grade: DIFFER(236v236) -- scratch rotation only: soundName pushed through ecx (retail edx) and &entry through edx (retail eax)
// Leaf: ?vfunc_39@CSoundManagerLH@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3F250, 236 bytes, `ret 14h`. Vtable 0x0129D2C4 (CSoundManagerLH) slot 39.
// Name: no PDB evidence -- neither Ego_r.pdb nor FableWin.pdb holds any CSoundManager* symbol (the LH audio layer is absent), so
//   the catalog name vfunc_39 is kept (siblings slot 37 AddBank, slot 40 vfunc_40 are landed descriptive names).
// Callees: CSoundManagerLH::FindByNamePair 0x00A3EE50 and FindByName 0x00A3ED30 (landed descriptive names), the sample lookup
//   0x00991840, the started object's slot 1 (+4), std::pair<CCharString, CCharString>(const CCharString&, const CCharString&)
//   0x0076DD40 (out of line), CCharString(const CCharString&) 0x0099EC30, the map vector's push_back 0x00A41840,
//   ~CCharString 0x0099EAE0, CVectorMap::Sort 0x00A42100.
// Layout: the pair map CVectorMap<pair<CCharString, CCharString>, CSoundBankLH*> at +0x54 (Dirty +0x61), as the landed
//   FindByNamePair / AddBank model it (real STLport pair, inlined CVectorMap::Add).
// Tried this session: STLport pair template + specialised out-of-line key ctor (same residue), std::make_pair for the key
//   and for the entry (225v236).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <> pair<CCharString, CCharString>::pair(const CCharString& __a, const CCharString& __b);   // 0x0076DD40 (out of line)
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void push_back(const _Tp& __x);                         // 0x00A41840 (out of line)
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
    __forceinline void Add(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort();                                            // 0x00A42100 (out of line)
};

class CSoundBankLH {                                        // descriptive: the named sound objects of the two maps
public:
    virtual void v00();
    virtual CSoundBankLH* Start(long sampleId, long a, long b);   // slot 1 (+0x04)
};
struct CSoundSample {
    unsigned char _pad[0x10];
    long          Id;                                       // +0x10
};

class CSoundManagerLH {                                     // vtable 0x0129D2C4
public:
    virtual void v00();
    unsigned char _pad04[0x54 - 0x04];
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);   // 0x00A3EE50
    CSoundBankLH* FindByName(const CCharString& name);     // 0x00A3ED30
    CSoundSample* FindSample(long sample);                  // 0x00991840
    virtual bool vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b);
};

bool CSoundManagerLH::vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b)
{
    if (FindByNamePair(bankName, soundName))
        return false;
    CSoundBankLH* bank = FindByName(bankName);
    if (!bank)
        return false;
    CSoundSample* info = FindSample(sample);
    if (!info)
        return false;
    CSoundBankLH* sound = bank->Start(info->Id, a, b);
    if (!sound)
        return false;
    NamePairMap.Add(std::pair<CCharString, CCharString>(bankName, soundName), sound);
    NamePairMap.Sort();
    return true;
}
