// CVectorMap::LowerBound(const ETCInterfaceType& key) const -- first entry of the sorted pair<ETCInterfaceType, CTCBase*> vector whose key is not less than key (binary search)
// Leaf: ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004365B0, 60 bytes, thiscall, `ret 4`, returns the entry pointer (end() when none).
// Identity (proven): the const overload of CThing::Components' LowerBound: FableWin.pdb ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE (_Vector_const_iterator, STLport's const pointer in retail); called by the const component lookups (CGameScriptInterface::GiveHeroGold 0x00435038, CPlayerGui::GetButtonAlpha 0x00439B34 ...); retail keeps it as a separate byte twin of 0x0040F020 (GOTCHAS: the lander rejects routing one through the other).
// Shape (family M008, representative 0x0040F020): CVectorMap derives from the STLport vector<pair<K, V> > (the PDB layout's
//   leading padding is that base; Compare +0xC, Dirty +0xD in retail). The search is Lionhead's own: the count is
//   `_M_finish - _M_start` (pointer difference, `sar 3`), computed before the cursor, halved with the signed `/ 2`
//   (cdq/sub/sar), the key read once (edi) and `first < key` steps past the middle (`count -= half + 1`).
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const
    {
        long count = this->_M_finish - this->_M_start;
        const value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
template class CVectorMap<ETCInterfaceType, CTCBase*>;