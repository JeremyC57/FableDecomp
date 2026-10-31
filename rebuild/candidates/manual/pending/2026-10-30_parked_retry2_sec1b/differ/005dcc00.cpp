// Grade: DIFFER(278v264) 76 of 98 lines (was 291v264, 78): the loop iterator takes FindFirstAnimWithName's result and first copies it (retail stores the result into it and first together, no loop-head copy) -- residue: retail keeps this in ebp, the iterator in edi homed in the dead name slot [esp+28h] and spills avoided/first to the frame; VC7.1 here keeps avoided in ebp and homes the iterator in a frame word
// CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter, long avoidBankIndex) const -- a random filtered animation among those named `name`, avoiding the one in bank slot avoidBankIndex unless it is the only candidate
// Leaf: ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCC00, 264 bytes, thiscall, `ret 10h`, esp frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z
//   is the only CAnimationSet member taking (name, unsigned long&, filter, long) (GetRandomLoadedAnim adds a bool); the body
//   walks Anims (CVectorMap<unsigned long, CAnimationEntry>, 0x14-byte pairs) from FindFirstAnimWithName(name) while the key
//   matches, calls the filter's operator() (slot 0, Ego_r.pdb ??RCAnimFilter*@@UBE_NABVCAnimationEntry@@@Z), compares
//   CAnimationEntry::BankIndex (+0) and picks with the inline GFRandom(range, &seed) (GFROR13 0x00497890).
// Callees: vector<const pair<unsigned long, CAnimationEntry>*>::reserve 0x005E15E0 and ::_M_insert_overflow 0x005E1500,
//   CAnimationSet::FindFirstAnimWithName(const CCharString&) const 0x005DC020, GFROR13 0x00497890, free 0x00BFEA14.
// Label correction: catalogued as the gap label "AnimDef_FilterAnimationList".
// Parameter names are descriptive (the PDB carries none).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
long __fastcall GFROR13(unsigned long value);               // 0x00497890
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
class CCharString { public: void* PStringData; };
class CDefString { public: long Index; };
class CAnimComponentMap;
template <class T> class CIVCountedPointer { public: T* Object; };
class CAnimationEntry {
public:
    long BankIndex;                                         // +0x00
    CDefString AnimName;                                    // +0x04
    CDefString GroupName;                                   // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;        // +0x0C
};
class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;   // slot 0
};
namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start != 0)
            free(_M_start);
    }
    bool empty() const { return _M_start == _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return _M_start[__n]; }
    void reserve(size_type __n);                            // 0x005E15E0
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend);                  // 0x005E1500
public:
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const_iterator end() const { return Data._M_finish; }
    std::vector<value_type> Data;
};
class CAnimationSet {
public:
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const;   // 0x005DC020
    const CAnimationEntry& GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter,
                                         long avoidBankIndex) const;
    TAnimMap Anims;                                         // +0x00
};

const CAnimationEntry& CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed,
                                                    const CAnimFilterBase& filter, long avoidBankIndex) const
{
    std::vector<TAnimMap::const_iterator> candidates;
    TAnimMap::const_iterator avoided = Anims.end();
    candidates.reserve(5);
    TAnimMap::const_iterator it = FindFirstAnimWithName(name);
    TAnimMap::const_iterator first = it;
    for (; it != Anims.end() && it->first == first->first; ++it) {
        const CAnimationEntry& anim = it->second;
        if (filter(anim)) {
            if (anim.BankIndex == avoidBankIndex)
                avoided = it;
            else
                candidates.push_back(it);
        }
    }
    TAnimMap::const_iterator chosen = candidates.empty() ? avoided : candidates[GFRandom(candidates.size(), &seed)];
    return chosen->second;
}
