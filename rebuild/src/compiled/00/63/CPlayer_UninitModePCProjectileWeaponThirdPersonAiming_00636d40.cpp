// CPlayer::UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input) -- leaving PC third-person projectile aiming: detach fifteen input processes from the player's interface, clear PCProjectileWeaponThirdPersonAimingMode and take the controlled creature's mode manager out of creature mode 0x19
// Leaf: ?UninitModePCProjectileWeaponThirdPersonAiming@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00636D40, 264 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (abs store at 0x0121B868, the
//   Uninit beside 0x00636B70's Init store at 0x0121B85E). Retail order 0x00636B00 UninitModeTakePhotojournalScreenshot
//   (landed), 0x00636B70, 0x00636D40, 0x00636E50 InitModes (landed) matches both PDBs' source order
//   UninitModeTakePhotojournalScreenshot, Init/UninitModePCProjectileWeaponThirdPersonAiming, InitModes; the body clears
//   CPlayer::PCProjectileWeaponThirdPersonAimingMode (+0x21A) and detaches (RemoveFromInterface).
// Shape: the controlled creature is used without a null test and the manager pointer is NOT pre-zeroed (`T* tc;`): with
//   no interface bit retail passes the uninitialised local (its frame word, shared with the lookup key) to RemoveMode,
//   and VC duplicates the RemoveMode tail into both branches.
// Model: the landed sibling 0x006360A0 (layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport bitset
//   reference proxy).
// Callees: CAInputProcess::RemoveFromInterface() 0x00A0D2D0 fifteen times, CPlayer::GetPControlledCreature() const
//   0x00487DC0, CVectorMap LowerBound 0x0040F020 (interface 0x31 = CTCCreatureModeManager),
//   CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0.
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
class CTCCreatureModeManager : public CTCBase {             // partial view
public:
    enum { INTERFACE_TYPE = 0x31 };                       // GetInterfaceType
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const;            // slot 1 (+0x04)
    };
    void RemoveMode(unsigned long mode);                  // 0x0073ABA0
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
    void RemoveFromInterface();                             // 0x00A0D2D0
};
class CInputProcessGameBase : public CAInputProcess { };
class CInputProcessControlCreature : public CInputProcessGameBase { };
class CInputProcessControlCreatureActivateZTarget : public CInputProcessGameBase { };
class CInputProcessCameraLookAround : public CInputProcessGameBase { };
class CInputProcessControlCreatureRightStick : public CInputProcessGameBase { };
class CInputProcessJumpingAndRolling : public CInputProcessGameBase { };
class CInputProcessBlock : public CInputProcessGameBase { };
class CInputProcessCreatureMovement : public CInputProcessGameBase { };
class CInputProcessUseRangedWeapon : public CInputProcessGameBase { };
class CInputProcessSetRangedWeaponMode : public CInputProcessGameBase { };
class CInputProcessWatchForRangedWeaponThirdPersonModeTermination : public CInputProcessGameBase { };
class CInputProcessCreatureMovementWatchForControlAngleChange : public CInputProcessGameBase { };
class CInputProcessStrafe : public CInputProcessGameBase { };
class CInputProcessDebugControls : public CInputProcessGameBase { };
class CInputProcessQuickAccessItems : public CInputProcessGameBase { };
class CInputProcessCombat : public CInputProcessGameBase { };
class CInputProcessSpecialAbilities : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature_ptr->RemoveFromInterface();
    InputProcessControlCreatureActivateZTarget_ptr->RemoveFromInterface();
    InputProcessControlCreatureRightStick_ptr->RemoveFromInterface();
    InputProcessRightStickLookAround_ptr->RemoveFromInterface();
    InputProcessStrafe_ptr->RemoveFromInterface();
    InputProcessBlock_ptr->RemoveFromInterface();
    InputProcessDebugControls_ptr->RemoveFromInterface();
    InputProcessQuickAccessItems_ptr->RemoveFromInterface();
    InputProcessCombat_ptr->RemoveFromInterface();
    InputProcessUseRangedWeapon_ptr->RemoveFromInterface();
    InputProcessSpecialAbilities_ptr->RemoveFromInterface();
    InputProcessJumpingAndRolling_ptr->RemoveFromInterface();
    InputProcessSetRangedWeaponMode_ptr->RemoveFromInterface();
    InputProcessWatchForControlAngleChange_ptr->RemoveFromInterface();
    InputProcessWatchForRangedWeaponThirdPersonModeTermination_ptr->RemoveFromInterface();
    PCProjectileWeaponThirdPersonAimingMode = false;
    CThingPlayerCreature* creature = GetPControlledCreature();
    CTCCreatureModeManager* modes;
    creature->GetTC(&modes);
    modes->RemoveMode(0x19);
}