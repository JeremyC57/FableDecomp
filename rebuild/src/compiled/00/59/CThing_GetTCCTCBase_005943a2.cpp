// CThing::GetTC<CTCBase>(ETCInterfaceType type, CTCBase** out) -- fetch a thing's component of the given interface type when the thing has one
// Leaf: ??$GetTC@VCTCBase@@@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x005943A2, 73 bytes, `ret 8` (thiscall; returns bool in al).
// Name: FableWin.pdb `??$GetTC@V<T>@@@CThing@@QAE_NW4ETCInterfaceType@@PAPAV<T>@@@Z` (a member template; one ICF-folded
// body serves every T, so the instantiation is not recoverable from the bytes -- CTCBase stands for it). No direct
// callers. The constant-type siblings 0x0059422B/0x00594276/0x005943EB (landed as GetInterface_0x46/0x27/0x29) are
// the same body inlined with a literal type.
// Callees: std::bitset<274>::test(size_t) const @ 0x00410DE0 (UsingComponents, out of line in this TU),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0 (Components).
// Label: the catalog "CTCInterfaceBase_TryGetInterfaceElementByType" is descriptive.
// Body: `if (!UsingComponents.test(type)) return false;` then the inline vector-map find: lower bound, a miss (end,
// or a larger key) yields the end entry, whose value is handed out regardless; returns true.
// Shape: type in edi; the key the lower bound reads is a copy homed in the dead `type` slot (`mov [ebp+8],edi`), the
// map is a named pointer (`add esi,44h` once) and the miss test compares the parameter itself (edi; comparing the
// copy reloads it from the slot). Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
// Layout: retail CThing +0x20 UsingComponents (bitset<274>, 9 words), +0x44 Components (CVectorMap: STLport vector
// {first, last, end} of 8-byte {type, component} pairs) -- the Ego_r.pdb CThing is 4 bytes wider before them.
#pragma optimize("y", off)
#pragma optimize("s", on)

enum ETCInterfaceType { TCI_NONE };
class CTCBase;

namespace std {
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;             // 0x00410DE0 (bitset<274>)
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                            // +0x00 (STLport vector)
    value_type* _M_finish;                           // +0x04
    value_type* _M_end_of_storage;                   // +0x08
    value_type* end() const { return _M_finish; }
    value_type* LowerBound(const K& key) const;      // 0x004365B0
};

class CThing {
public:
    void*                                  __vftable;           // +0x00
    unsigned char                          _pad04[0x1c];        // +0x04 (UniqueID among them)
    std::bitset<274>                       UsingComponents;     // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;          // +0x44

    template <class T> bool GetTC(ETCInterfaceType type, T** out)
    {
        if (UsingComponents.test(type)) {
            ETCInterfaceType key = type;
            const CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

template bool CThing::GetTC<CTCBase>(ETCInterfaceType, CTCBase**);