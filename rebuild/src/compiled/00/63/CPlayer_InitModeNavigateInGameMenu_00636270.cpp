// CPlayer::InitModeNavigateInGameMenu(EPlayerMode mode, CProcessedInput* input) -- entering the in-game menu mode: open the player character's in-game menu component, then attach the in-game-menu input process
// Leaf: ?InitModeNavigateInGameMenu@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe @ 0x00636270, 92 bytes, thiscall `ret 8` (a CPlayer::PlayerModeInfo handler).
// Identity (probable, donor order + body): Ego_r.pdb orders ..., InitModeChargeUpWillSpell, UninitModeChargeUpWillSpell,
// InitModeNavigateInGameMenu, UninitModeNavigateInGameMenu, InitModeTavernGame, ...; this body attaches Ego_r.pdb
// CPlayer::InputProcessInGameMenu (+0x194) and its neighbour 0x006362D0 detaches the same process (after closing the menu
// component), followed by InitModeLightning 0x00636480 (landed). The component's interface is 0xC6 (CTCInGameMenu by its callee).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
// CTCInGameMenu::OpenInGameMenu 0x004830A1 (no arguments, plain `ret`: Ego_r.pdb ?OpenInGameMenu@CTCInGameMenu@@QAEXXZ; its only
// caller is this body), CAInputProcess::AddToInterface (slot 1).
// Shape: the landed screen-mode GetTC head (`T* tc = 0; character->GetTC(&tc); tc->...`), the component bit test is the byte
// test `test byte [eax+38h],40h`.
// Layout: engine/CPlayer.h through the landed screen-mode donor include (InitModeNavigateInventoryExperienceScreen 0x00635080):
// PlayerInterface +0x0C, World +0x1C, PlayerCharacter +0x34, the scoped_ptr<CInputProcess*> members; CThing partial view with
// retail UsingComponents +0x20, Components +0x44, Dead (bit 0 of +0x91).
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
    void RemoveFromInterface();                             // 0x00A0D2D0
};

class CInputProcessGameBase : public CAInputProcess { };   // partial views (bases not modelled)
class CInputProcessInGameMenu : public CInputProcessGameBase { };

class CGameCameraManager {
public:
    void PushSpecialCameraMode();                           // 0x00698F50
    void SetAutomaticCameraControl();                       // 0x00699090
};
class CCameraManagerSource {                                // the world's +0x34 object (partial view)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld {                                              // partial view
public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                     // +0x34
};

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CTCInGameMenu : public CTCBase {                       // partial view
public:
    enum { INTERFACE_TYPE = 0xC6 };
    void OpenInGameMenu();                                  // 0x004830A1
};
class CPlayer : public CPlayerPdb {
private:
    void InitModeNavigateInGameMenu(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeNavigateInGameMenu(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInGameMenu* menu = 0;
    character->GetTC(&menu);
    menu->OpenInGameMenu();
    InputProcessInGameMenu_ptr->AddToInterface(*PlayerInterface);
}