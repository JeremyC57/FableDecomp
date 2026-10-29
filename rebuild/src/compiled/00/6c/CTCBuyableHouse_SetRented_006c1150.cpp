// CTCBuyableHouse::SetRented(bool rented) -- mark the house as rented out (or not): keep the living hero's rented-building count in step, evict the tenant when an owned house stops being rented, rebuild the main sign post and refresh the player GUI's mini-map markers
// Leaf: ?SetRented@CTCBuyableHouse@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C1150, 243 bytes, `ret` through the tail jump to CPlayerGui::UpdateMiniMapMarkers
//   with the argument slot reused (`mov [esp+4],0`).
// Identity: Ego_r.pdb has `?SetRented@CTCBuyableHouse@@QAEX_N@Z`; the catalog label is right. The sibling
//   CTCBuyableHouse::SetOwnedByPlayer 0x006C17B0 (landed) ends with the same sign-post and GUI tail.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (Ego_r `QAEAAVCPlayer@@XZ`), CPlayer::GetPControlledCreature 0x00487DC0
//   (settled), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (settled, inside the inline
//   CThing::GetTC<CTCHeroStats>), CTCBuyableHouse::Evict 0x006C0FD0, NeedsMainSignPost 0x006BFB90 and CreateMainSignPost
//   0x006C0F00 (all private in Ego_r: `AAEXXZ` / `AAE_NXZ` / `AAEXXZ`), CBaseIntelligentPointer::GetPItem 0x00A01B50 (inside
//   CIntelligentPointer<CThing>::Get), CThing::Kill 0x004C9B80 (`?Kill@CThing@@QAEX_N@Z`), CPlayerGui::UpdateMiniMapMarkers
//   0x004378C0 (Ego_r `?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z`: the body forwards to the mini-map drawer +0x160, whose
//   0x00643E9A forwards to its CMiniMapDisplay +0x28 at 0x0082B7C0 -- CDrawMiniMap::UpdateMarkers(bool) and
//   CMiniMapDisplay::UpdateMarkers(bool) in the PDB; the landed spelling `Refresh(long)` is not a PDB name).
// Globals: g_pMainGameComponent 0x013B86A0 and g_pPlayerGui 0x013B8790 (bound spellings; no PDB data symbols).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, Rented +0x12, MainSignPost +0x24), engine/CTCHeroStats.h
//   (NumberOfBuildingsRented +0xE8), engine/CPlayer.h, engine/CPlayerManager.h and engine/CPlayerGui.h through the rename trick (callee classes,
//   methods only); CMainGameComponent::PPlayerManager +0x1C (Ego_r, scoped_ptr); CThing: UsingComponents +0x20, Components
//   +0x44, Dead +0x91 bit 0 (Ego_r minus 0x10 past Components). CTCHeroStats is interface type 4 (bit 4 of the bitset byte +0x20).
// Shape: the hero stats pointer starts null (`xor ecx,ecx`) and GetTC leaves it so when the bitset bit is clear; the key of
//   the inlined LowerBound is homed in the dead `rented` argument slot (bl keeps the argument). GetTC must take the map's
//   address into a local (`lea esi,[eax+0x44]`, end read as [esi+4]) as agent561's 0x004A2F10 model; reading
//   Components.Data.end() straight off the hero pointer keeps the hero in esi instead (DIFFER 246v243).
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
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator end() { return this->_M_finish; }
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
};

enum ETCInterfaceType { };                                  // values not needed here
class CTCBase;

#define CTCHeroStats CTCHeroStatsLayout
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCHeroStats : public CTCHeroStatsLayout { public:
    enum { INTERFACE_TYPE = 4 };                            // key 4 (`cmp [eax],4`); enumerator name not recovered
};

class CThing { public:                                      // partial view, retail offsets
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                        // CClass / CBaseObject / CBaseClass part
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                            // +0x91 bit 0
    void Kill(bool b);                                      // 0x004C9B80
    template <class T> __forceinline bool GetTC(T** out)
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
class CThingPlayerCreature : public CThing { };

#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
class CPlayer : public CPlayerLayout { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager : public CPlayerManagerLayout { public:
    CPlayer& GetMainPlayer();                               // 0x00449970
};
template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent { public:                          // Ego_r.pdb offsets up to +0x28 hold in retail
    unsigned char              _pad00[0x1C];
    scoped_ptr<CPlayerManager> PPlayerManager;              // +0x1C
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CPlayerGui : public CPlayerGuiLayout { public:
    void UpdateMiniMapMarkers(bool force);                  // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

class CBaseObject;
class CBaseIntelligentPointer { public:                     // vptr + PData, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

#define CTCBuyableHouse CTCBuyableHouseLayout
#include "engine/CTCBuyableHouse.h"
#undef CTCBuyableHouse
class CTCBuyableHouse : public CTCBuyableHouseLayout { public:
    void SetRented(bool rented);
private:
    bool NeedsMainSignPost();                               // 0x006BFB90
    void CreateMainSignPost();                              // 0x006C0F00
    void Evict();                                           // 0x006C0FD0
};

void CTCBuyableHouse::SetRented(bool rented)
{
    CThingPlayerCreature* hero = g_pMainGameComponent->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCHeroStats* stats = 0;
        hero->GetTC(&stats);
        if (rented == true) {
            if (!Rented)
                stats->NumberOfBuildingsRented++;
        } else if (rented == false) {
            if (Rented == true)
                stats->NumberOfBuildingsRented--;
        }
    }
    Rented = rented;
    if (!rented && OwnedByPlayer)
        Evict();
    if (reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()
        && !reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()->Dead)
        reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()->Kill(true);
    if (NeedsMainSignPost())
        CreateMainSignPost();
    g_pPlayerGui->UpdateMiniMapMarkers(false);
}