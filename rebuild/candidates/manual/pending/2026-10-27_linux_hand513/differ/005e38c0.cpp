// CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> >::Insert(const unsigned long& key, const CAnimationEntry& value) -- append a {key, entry} pair to the map's vector (unsorted) and mark the map dirty
// Grade: DIFFER(104v95) -- the pair temporary's release: retail `mov ecx,[esp+14h] ; test ecx,ecx ; je ; dec dword ptr [ecx+4] ; jne`; the build re-reads Object and the count after the `dec` (the temporary's address escaped to push_back). Tried (4): `Object && --count == 0`, a local copy of Object (non-RMW `mov/dec/test/mov`), a pointee DecRef() member (same), `optimize("s")` (frame + out-of-line pair ctor). Next: IVRefCount as long, or the Release() spelling FableWin.pdb names (?Release@?$CIVCountedPointer@...).
// Leaf: ?Insert@?$CVectorMap@KVCAnimationEntry@@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@@QAEXABKABVCAnimationEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E38C0, 95 bytes, thiscall `ret 8`.
// Identity (probable): the CVectorMap Insert shape of the landed CVectorMap<long, CSoundBankRecord>::Insert 0x009937D0
//   (`Data.push_back(value_type(key, value)); Dirty = true;`, Dirty +0x0D) over a pair<unsigned long, CAnimationEntry>
//   (0x14 bytes: Ego_r.pdb CAnimationEntry {BankIndex, AnimName, GroupName, Components CIVCountedPointer<CAnimComponentMap>});
//   CAnimationSet::Anims (Ego_r.pdb CVectorMap<unsigned long, CAnimationEntry, ...>) is the only map of that pair, and
//   FableWin.pdb has ?Insert@?$CVectorMap@KVCAnimationEntry@@... for it. The label "CAnimationSet::Add" is the caller's
//   name (CAnimationSet::Add(const CAnimationEntry&) inlines this call), not this body's.
// Callees: vector<pair<unsigned long, CAnimationEntry> >::push_back(const value_type&) 0x005E36B0 (out of line here).
// Shape: the pair temporary in a 0x14-byte frame (key dword, the entry's implicit copy with the Components count bumped),
//   destroyed after the call (the CIVCountedPointer release: --IVRefCount, slot 1 at zero).
class CIVCountedPointeeBase { public:
    virtual void _v000();
    virtual void DeleteThis();                          // slot 1 (+4)
    unsigned long IVRefCount;                           // +0x04
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object && --Object->IVRefCount == 0) Object->DeleteThis(); }
};
class CDefString { public:
    long TablePos;                                      // +0
};
class CAnimComponentMap;
class CAnimationEntry { public:
    long BankIndex;                                     // +0x00
    CDefString AnimName;                                // +0x04
    CDefString GroupName;                               // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0C
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                           // +0x00
    T2 second;                                          // +0x04
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> class allocator { public: allocator() { } };
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc { public:
    Value _M_data;
};
template <class T, class Alloc> class _Vector_base { public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};
template <class T, class Alloc = allocator<T> > class vector : protected _Vector_base<T, Alloc> { public:
    void push_back(const T& x);                         // 0x005E36B0
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap { public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                       // +0x00
    C Compare;                                          // +0x0C (empty)
    bool Dirty;                                         // +0x0D
    void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};
template class CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> >;
