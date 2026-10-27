// CPlayer::InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input) -- entering the map hero-information screen mode: open the player character's CTCInventoryMap component (OpenInventory, slot 30), take the main input process off the interface, attach the hero-information-screens and map-screen input processes, then the alive check
// Leaf: ?InitModeNavigateInventoryMapScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634F60, 156 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B0D4), which pairs
//   every EPlayerMode's Init and Uninit member pointers; this is the Init of mode 8 (partner 0x00635000, which closes the same
//   component). Modes 4..8 are the five inventory screens in FableWin.pdb's order (NavigateInventory, Clothing, Weapons,
//   AbilitiesScreen, MapScreen), and the body opens interface 0x1B (CTCInventoryMap, as the landed
//   CInputProcessInventoryMapScreen::Init) and attaches CPlayer::InputProcessInventoryMapScreen (+0x120); both PDBs hold
//   ?InitModeNavigateInventoryMapScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z.
// Model: the landed family J104 row CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 (same GetTC, same alive
//   tail), plus CAInputProcess::RemoveFromInterface() 0x00A0D2D0 (Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ) on
//   InputProcessMain (+0x100) before the two AddToInterface calls.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (four calls), CVectorMap LowerBound 0x0040F020,
//   CTCInventoryMap::OpenInventory (virtual slot 30), RemoveFromInterface 0x00A0D2D0, AddToInterface (virtual slot 1).
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
namespace NInventory {
class CTCInventoryBase : public CTCBase {                   // partial view: 86 vtable slots (CTCInventoryMagic adds none)
public:
    virtual ~CTCInventoryBase();
    virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04(); virtual void Slot05();
    virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09(); virtual void Slot10();
    virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14(); virtual void Slot15();
    virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19(); virtual void Slot20();
    virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24(); virtual void Slot25();
    virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void OpenInventory();                             // slot 30 (+0x78)
    virtual void CloseInventory();                            // slot 31 (+0x7C)
    virtual void Slot32(); virtual void Slot33(); virtual void Slot34(); virtual void Slot35(); virtual void Slot36();
    virtual void Slot37(); virtual void Slot38(); virtual void Slot39(); virtual void Slot40(); virtual void Slot41();
    virtual void Slot42(); virtual void Slot43(); virtual void Slot44(); virtual void Slot45(); virtual void Slot46();
    virtual void Slot47(); virtual void Slot48(); virtual void Slot49(); virtual void Slot50(); virtual void Slot51();
    virtual void Slot52(); virtual void Slot53(); virtual void Slot54(); virtual void Slot55(); virtual void Slot56();
    virtual void Slot57(); virtual void Slot58(); virtual void Slot59(); virtual void Slot60(); virtual void Slot61();
    virtual void Slot62(); virtual void Slot63(); virtual void Slot64(); virtual void Slot65(); virtual void Slot66();
    virtual void Slot67(); virtual void Slot68(); virtual void Slot69(); virtual void Slot70(); virtual void Slot71();
    virtual void Slot72(); virtual void Slot73(); virtual void Slot74(); virtual void Slot75(); virtual void Slot76();
    virtual void Slot77(); virtual void Slot78(); virtual void Slot79(); virtual void Slot80(); virtual void Slot81();
    virtual void Slot82(); virtual void Slot83(); virtual void Slot84(); virtual void Slot85();
};
}
class CTCInventoryMap : public NInventory::CTCInventoryBase {   // partial view
public:
    enum { INTERFACE_TYPE = 0x1b };                       // GetInterfaceType
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
class CInputProcessInventoryMapScreen : public CInputProcessInventoryBase { };
class CInputProcessMain : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
private:
    void InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventoryMap* inventory = 0;
    character->GetTC(&inventory);
    inventory->OpenInventory();
    InputProcessMain_ptr->RemoveFromInterface();
    InputProcessHeroInformationScreens_ptr->AddToInterface(*PlayerInterface);
    InputProcessInventoryMapScreen_ptr->AddToInterface(*PlayerInterface);
    if (character.GetPItem() && !character->IsDead())
        character.GetPItem();
}