// CSoundManagerLH::AddBank(const CCharString& name, unsigned long channels) -- create a sound bank through the device's factory, switch on each channel whose bit is set in the mask, and file the bank under its name in the name map (+0x64); false when there is no device, the name is taken or creation fails
// Leaf: ?AddBank@CSoundManagerLH@@QAE_NABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3F140, 272 bytes, thiscall `ret 8`. Vtable 0x0129D2C4 (CSoundManagerLH,
//   RTTI) slot 37; no direct callers. Descriptive name: the audio layer is absent from the PDBs (as the landed FindByName
//   0x00A3ED30 / FindByNamePair 0x00A3EE50 of the same class), so the method, the +0x40 device and its slots are named
//   from what the body does.
// Callees: CSoundManagerLH::FindByName 0x00A3ED30, the device's slot 3 (+0xC, factory) then the factory's slot 14 (+0x38,
//   new bank), the bank's slot 5 (+0x14) and that object's slot 0 (channel, true), CCharString copy ctor 0x0099EC30,
//   vector<pair<CCharString, CSoundBankLH*> >::_M_insert_overflow 0x00A41480, ~CCharString 0x0099EAE0,
//   std::sort<pair<CCharString, CSoundBankLH*>*, CKeyPairCompareLess<..> > 0x00A41E20.
// Label: catalog "CParticleMorphs_AddMorphEntry" is propagated and wrong (RTTI vtable slot of CSoundManagerLH).
// Shape: two separate early `return false` tests (retail duplicates the `xor al,al` epilogue for each); the mask walk is
//   the landed FindFirstSetBit 0x00A3F860 inlined with numBytes 16 (`cmp ecx,80h`), then `channels ^= 1 << channel`;
//   the CVectorMap add is the inline STLport push_back of a temporary pair (placement-new null test, `add [esi+4],8`,
//   overflow with (__false_type, 1, true)) plus `Dirty = true`, then the inline sort-when-dirty. Retail inlines these
//   here, so Add and push_back are __forceinline (GOTCHAS: one inlined call site).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CSoundBankLH;

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    __forceinline void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                              // 0x00A41480
};

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);     // 0x00A41E20
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
    void Sort()
    {
        if (Dirty) {
            std::sort(Data.begin(), Data.end(), Comp);
            Dirty = false;
        }
    }
};

inline int __fastcall FindFirstSetBit(unsigned long mask, unsigned long numBytes)
{
    for (unsigned long i = 0; i < numBytes * 8; ++i)
        if (mask & (1 << i))
            return (int)i;
    return -1;
}

class CSoundChannelSet {                                    // descriptive: what the bank's slot 5 returns
public:
    virtual void SetChannel(int channel, bool on);          // slot 0
};

class CSoundBankLH {                                        // descriptive
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual CSoundChannelSet* GetChannels();                // slot 5 (+0x14)
};

class CSoundBankFactory {                                   // descriptive
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual CSoundBankLH* CreateBank();                     // slot 14 (+0x38)
};

class CSoundDevice {                                        // descriptive
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CSoundBankFactory* GetFactory();                // slot 3 (+0xC)
};

class CSoundManagerLH {                                     // vtable 0x0129D2C4
public:
    unsigned char _00[0x40];                                // +0x00 (vptr ..)
    CSoundDevice* Device;                                   // +0x40
    unsigned char _44[0x10];                                // +0x44
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>                          NameMap;       // +0x64
    CSoundBankLH* FindByName(const CCharString& name);      // 0x00A3ED30
    bool AddBank(const CCharString& name, unsigned long channels);
};

bool CSoundManagerLH::AddBank(const CCharString& name, unsigned long channels)
{
    if (Device == 0)
        return false;
    if (FindByName(name) != 0)
        return false;
    CSoundBankLH* bank = Device->GetFactory()->CreateBank();
    if (bank == 0)
        return false;
    while (channels) {
        int channel = FindFirstSetBit(channels, 16);
        bank->GetChannels()->SetChannel(channel, true);
        channels ^= 1 << channel;
    }
    NameMap.Add(name, bank);
    NameMap.Sort();
    return true;
}