// CTCSpecialAbilities::CalculateWeaponState(CThingCreatureBase& creature) -- which weapon pose a spell cast uses: unarmed with nothing (alive) in the primary carrying slot, armed when the held thing is not the active ranged weapon, else the bow pose unless the ranged weapon's type is 4 (armed)
// Leaf: ?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072B940, 247 bytes, static __fastcall (ecx = creature), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z
//   (Ego_r donor 0x00742E70, a static __fastcall member of CTCSpecialAbilities returning the nested EWEAPON_STATE); the landed
//   CTCWillLightning::SetWeaponState 0x0079F680 / CTCFireballSpell::SetWeaponState 0x007C8660 take the same enum, and their
//   anim-name switches pick the left/right-handed (0), "ARMED" (1) and "WITH_BOW" (2) animations.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 / non-const 0x0040F020, CTCCarrying::
//   GetThingInPrimarySlot() const 0x006A1440, CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CTCWeapon::GetWeaponType()
//   const 0x005DA7D0.
// Layout: retail CThing UsingComponents +0x20 / Components +0x44 / Dead +0x91 bit 0; CTCInventoryWeapons (interface 0x13)
//   PActiveRangedWeapon +0x141 (Ego_r +0x159 less the 0x18 the PC STLport containers save, as PActiveMeleeWeapon +0x134 of the
//   landed 0x007CCD70); interfaces 0x46 CTCCarrying, 0x48 CTCWeapon.
// Shape: the inventory pointer is an uninitialised local filled only under HasTC(0x13) (the untaken branch reads the stale key
//   slot, GOTCHAS chunk 264); the creature's lookups are const (PeekTC), the weapon's is the non-const GetTC; both are
//   __forceinline (VC7.1 otherwise keeps the second and third out of line) over same-TU noinline LowerBound bodies (chunk 242).
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
enum EWeaponType { EWeaponType_Force32 = 0x7fffffff };   // value 4 is the one ranged type posed as armed (descriptive)

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    bool _Unchecked_test(unsigned int __pos) const { return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != 0; }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
    value_type* LowerBound(const K& key);               // 0x0040F020
    const value_type* LowerBound(const K& key) const;   // 0x004365B0
};
// Same-TU bodies of the two real retail LowerBound instances (binary search over the sorted pairs), not inlined: they let the
// three inlined lookups share one key frame word with the uninitialised inventory local, as retail does.
#pragma auto_inline(off)
template <class K, class V, class TCompare>
__declspec(noinline) const typename CVectorMap<K, V, TCompare>::value_type* CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const value_type* it = Data._M_start;
    long count = Data._M_finish - Data._M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
template <class K, class V, class TCompare>
__declspec(noinline) typename CVectorMap<K, V, TCompare>::value_type* CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    value_type* it = Data._M_start;
    long count = Data._M_finish - Data._M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                        // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->Data.end() || type < it->first) ? map->Data.end() : it)->second;
    }
    template <class T> __forceinline T* GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->Data.end() || type < it->first) ? map->Data.end() : it)->second;
    }
};
class CThingCreatureBase : public CThing { };

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;              // 0x006A1440
};
class CTCWeapon {
public:
    EWeaponType GetWeaponType() const;                  // 0x005DA7D0
};
#pragma pack(push, 1)
class CTCInventoryWeapons {                             // interface 0x13, partial view (retail offsets)
public:
    unsigned char               _pad000[0x141];
    CIntelligentPointer<CThing> PActiveRangedWeapon;    // +0x141
};
#pragma pack(pop)

class CTCSpecialAbilities {
public:
    enum EWEAPON_STATE {                                // enumerator names descriptive (from the landed anim-name switches)
        WEAPON_STATE_UNARMED = 0,
        WEAPON_STATE_ARMED   = 1,
        WEAPON_STATE_BOW     = 2
    };
    static EWEAPON_STATE __fastcall CalculateWeaponState(CThingCreatureBase& creature);
};

CTCSpecialAbilities::EWEAPON_STATE __fastcall CTCSpecialAbilities::CalculateWeaponState(CThingCreatureBase& creature)
{
    if (creature.HasTC((ETCInterfaceType)0x46)) {
        CThing* weapon = creature.PeekTC<CTCCarrying>((ETCInterfaceType)0x46)->GetThingInPrimarySlot();
        if (weapon && !weapon->Dead) {
            const CTCInventoryWeapons* inventory;
            if (creature.HasTC((ETCInterfaceType)0x13))
                inventory = creature.PeekTC<CTCInventoryWeapons>((ETCInterfaceType)0x13);
            if (weapon == inventory->PActiveRangedWeapon.GetPItem()) {
                if (weapon->HasTC((ETCInterfaceType)0x48))
                    return weapon->GetTC<CTCWeapon>((ETCInterfaceType)0x48)->GetWeaponType() == (EWeaponType)4
                               ? WEAPON_STATE_ARMED : WEAPON_STATE_BOW;
                return WEAPON_STATE_BOW;
            }
            return WEAPON_STATE_ARMED;
        }
    }
    return WEAPON_STATE_UNARMED;
}