// CPlayer::UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input) -- leaving the live-GUI mode: close the player character's PC inventory, end the live controlled creature's blocking, take the hero-information-screens and inventory input processes off the interface and give the main input process back
// Leaf: ?UninitModeLiveGUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006369B0, 194 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B82C): the Uninit of
//   mode 43, between the OracleMinigame (41: Uninit 0x00633BB0, landed) / FireheartMinigame (42) pairs and the Console pair
//   (44: Uninit 0x00634510, landed); FableWin.pdb's source order is OracleMinigame, FireheartMinigame, LiveGUI, Console,
//   and retail places this body right after the mode-43 Init 0x006368D0 (which opens the PC inventory and attaches the same
//   two processes) as both PDBs place UninitModeLiveGUI after InitModeLiveGUI.
// Model: the landed J104 CPlayer rows (0x00635080: layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport
//   bitset reference proxy, `T* tc = 0`).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap LowerBound 0x0040F020 (interfaces 0x11 =
//   CTCInventory and 0x30 = CTCCombat), CTCInventory::ClosePCInventory(bool) 0x00587847, CPlayer::GetPControlledCreature()
//   const 0x00487DC0, CTCCombat::UnsetBlocking() 0x007338A0, CAInputProcess::RemoveFromInterface() 0x00A0D2D0 (twice),
//   AddToInterface (virtual slot 1).
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        size_t _M_bpos;
        reference(bitset<_Nb>& __b, size_t __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](size_t __pos) { return reference(*this, __pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    iterator LowerBound(const K& key);                        // 0x0040F020 for the TC map
};

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass { };

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)
class CTCInventory : public CTCBase {                       // partial view
public:
    enum { INTERFACE_TYPE = 0x11 };                       // GetInterfaceType
    void ClosePCInventory(bool immediately);              // 0x00587847
};
class CTCCombat : public CTCBase {                          // partial view
public:
    enum { INTERFACE_TYPE = 0x30 };                       // GetInterfaceType
    void UnsetBlocking();                                 // 0x007338A0
};

class CThing {                                              // partial view, retail offsets
public:
    unsigned char      _pad00[0x20];                        // CClass / CBaseObject / CBaseClass, UniqueID (+0x18)
    std::bitset<274>   UsingComponents;                     // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];                 // UpdateComponentsList (list, 4) ... PlayerNumber (+0x90)
    unsigned char      Dead : 1;                            // +0x91 bit 0
    unsigned char      CreatedByOwnerThingCreation : 1;
    unsigned char      IncludeInThingScans : 1;
    unsigned char      Activated : 1;
    unsigned char      GloballyLoaded : 1;
    unsigned char      InLimbo : 1;
    unsigned char      KillLocked : 1;
    unsigned char      CreatedBySerialisation : 1;
    bool IsDead() const { return Dead; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };                   // RTTI CThingPlayerCreature <- CThingCreatureBase <- CThingGameObject
class CThingGameObject : public CThingPhysical { };         //   <- CThingPhysical <- CThing
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CInputProcessManager : public CBaseClassNonCopyable { };   // RTTI CGamePlayerInterface <- CInputProcessManager
class CGamePlayerInterface : public CInputProcessManager { };
class CAInputProcess : public CBaseClassNonCopyable {
public:
    virtual bool AddToInterface(CInputProcessManager& manager);   // slot 1 (+0x04)
    void RemoveFromInterface();                                   // 0x00A0D2D0
};
class CInputProcessGameBase : public CAInputProcess { };
class CInputProcessHeroInformationScreens : public CInputProcessGameBase { };
class CInputProcessInventoryBase : public CInputProcessGameBase { };
class CInputProcessInventory : public CInputProcessInventoryBase { };
class CInputProcessMain : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventory* inventory = 0;
    character->GetTC(&inventory);
    inventory->ClosePCInventory(true);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCombat* combat = 0;
        creature->GetTC(&combat);
        combat->UnsetBlocking();
    }
    InputProcessHeroInformationScreens_ptr->RemoveFromInterface();
    InputProcessInventory_ptr->RemoveFromInterface();
    InputProcessMain_ptr->AddToInterface(*PlayerInterface);
}