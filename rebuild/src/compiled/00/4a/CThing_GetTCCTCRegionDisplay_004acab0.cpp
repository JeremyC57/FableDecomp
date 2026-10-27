// CThing::GetTC<CTCRegionDisplay>(CTCRegionDisplay** out) -- fetch the thing's region-display component (interface 0x8E) when the thing uses one
// Leaf: ??$GetTC@VCTCRegionDisplay@@@CThing@@QAE_NPAPAVCTCRegionDisplay@@@Z
// Retail: Fable.exe @ 0x004ACAB0, 72 bytes, __fastcall `ret 4`, bool in al.
// Identity (proven by interface number + PDB name set): CThing::GetTC<T>(T**); 0x8E is CTCRegionDisplay's interface
// number (GetInterfaceType, vtable 0x0125A0EC slot 21 = 0x0063A1F0 `mov eax,8Eh`), and the PDB name sets hold the only
// instantiation ??$GetTC@VCTCRegionDisplay@@@CThing@@QAE_NPAPAVCTCRegionDisplay@@@Z. No caller in retail (gap-scan COMDAT).
// Callee: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::LowerBound(const ETCInterfaceType&) @
// 0x0040F020 (bound ?LowerBound@...@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z).
// Shape: UsingComponents[0x8E] through the STLport bitset reference proxy (bit 14 of word 4 = `mov eax,[ecx+30h] ;
// test ah,40h`), the key copy in the pushed ecx slot, a miss (end or greater key) yields the end entry, whose value is
// handed out; true.
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44 (the landed
// CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 model; no CThing engine header exists).
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        unsigned int _M_bpos;
        reference(bitset<_Nb>& __b, unsigned int __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](unsigned int __pos) { return reference(*this, __pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
    const value_type* LowerBound(const K& key) const;         // 0x004365B0 for the TC map
};

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;

class CTCRegionDisplay {
public:
    enum { INTERFACE_TYPE = 0x8E };                         // GetInterfaceType 0x0063A1F0
};

class CThing {                                              // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                        // CClass / CBaseObject / CBaseClass part, UniqueID (+0x18)
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    template <class T> bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

template bool CThing::GetTC<CTCRegionDisplay>(CTCRegionDisplay** out);