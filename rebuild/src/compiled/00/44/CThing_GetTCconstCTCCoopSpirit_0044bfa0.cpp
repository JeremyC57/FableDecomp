// CThing::GetTC<const CTCCoopSpirit>(const CTCCoopSpirit** out) -- fetch the thing's co-op spirit component (interface 0xC7) when the thing uses one
// Leaf: ??$GetTC@$$CBVCTCCoopSpirit@@@CThing@@QAE_NPAPBVCTCCoopSpirit@@@Z
// Retail: Fable.exe @ 0x0044BFA0, 71 bytes, __fastcall `ret 4`, bool in al.
// Identity (probable): CThing::GetTC<T>(T**) member template (PDB name sets: 267 instantiations). The interface number
// 0xC7 is CTCCoopSpirit's (its GetInterfaceType, vtable 0x0125B264 slot 21 = 0x00670B50 `mov eax,0C7h`). The PDB holds
// both ??$GetTC@VCTCCoopSpirit@@@CThing@@QAE_NPAPAVCTCCoopSpirit@@@Z and the const-T twin
// ??$GetTC@$$CBVCTCCoopSpirit@@@CThing@@QAE_NPAPBVCTCCoopSpirit@@@Z; this body calls the CONST
// CVectorMap::LowerBound 0x004365B0 (the non-const GetTC<CTCRegionDisplay> 0x004ACAB0 calls the non-const 0x0040F020),
// so it is taken as the const-T instantiation. No caller in retail (gap-scan COMDAT).
// Callee: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::LowerBound(const ETCInterfaceType&) const
// @ 0x004365B0 (bound ?LowerBound@...@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z).
// Shape: UsingComponents[0xC7] through the STLport bitset reference proxy (bit 7 of word 6 = the sign of byte +0x38:
// `mov al,[ecx+38h] ; test al,al ; jns`), the key copy in the pushed ecx slot, a miss (end or greater key) yields the
// end entry, whose value is handed out; true. The const map view is a model device for the const overload the retail
// instantiation reaches (how Lionhead's template selects it is not recoverable from the bytes).
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

class CTCCoopSpirit {
public:
    enum { INTERFACE_TYPE = 0xC7 };                         // GetInterfaceType 0x00670B50
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
            const TCMap* map = &Components;
            const TCMap::value_type* it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

template bool CThing::GetTC<const CTCCoopSpirit>(const CTCCoopSpirit** out);