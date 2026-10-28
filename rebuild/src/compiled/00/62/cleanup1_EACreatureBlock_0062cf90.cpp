// CWorld::EACreatureBlock(const CGameEvent* event) -- game-event action: start the event player's controlled creature blocking (a CCreatureAction_StartBlocking, then CTCCombat::SetBlocking) unless the player is in mode 0x2F, the thing manager is paused, the player is in spell mode or not the main player, the creature is dead or already blocking, its primary-slot weapon is unusable or its berserk state forbids blocking
// Leaf: ?EACreatureBlock@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062CF90, 506 bytes, thiscall, `ret 4`; 0x120-byte frame (the 0x114-byte action
//   temporary, the conditional-destruction flag word, the IS_THING_ALIVE reference slot, the canBlock byte).
// Identity (proven): registered by the static initialiser at 0x0121BEFB in the game-event dispatch table (no direct
//   caller); retail keeps the Ego_r.pdb donor order of the CWorld EA run here: EALoadGameFromInGameMenu 0x0062CE60
//   (landed), EACreatureBlock (this row), EAControlledCreatureJump 0x0062D380 (landed, whose header already names this
//   address); the body builds a CCreatureAction_StartBlocking (Ego_r.pdb ??0CCreatureAction_StartBlocking@@QAE@AAV
//   CThingCreatureBase@@@Z) and calls CTCCombat::IsBlocking / SetBlocking.
// Callees: CWorld::GetPlayer(unsigned long) 0x0049D850 (x2), CPlayer::HasMode(EPlayerMode) const 0x00634160,
//   CPlayer::GetPControlledCreature() const 0x00487DC0, CWorld::GetThingManager (virtual slot 3, +0x0C) x2,
//   CPlayer::IsMainPlayer() const 0x00487520, IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0 (__fastcall, by
//   reference), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (non-const: CTCCombat 0x30) and
//   0x004365B0 (const: CTCCarrying 0x46, CTCWeapon 0x48, CTCBerserk 0x84), CTCCombat::IsBlocking() const 0x007331E0 (x2),
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440, CTCWeapon::IsUsableBy(const CThing&) const 0x005DAB10,
//   CTCBerserk::CanBerserkBlock() const 0x0078BEF0, CCreatureAction_StartBlocking(CThingCreatureBase&) 0x00855BE0,
//   CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, ~CCombatActionBase 0x0062E900,
//   CTCCombat::SetBlocking() 0x007337E0.
// Interface types (GetInterfaceType, vtable slot 21): CTCCombat 0x30 (0x00739CE0), CTCWeapon 0x48 (0x005DADE0),
//   CTCBerserk 0x84 (0x0078DAC0); CTCCarrying 0x46 (the primary-slot owner).
// Layout: retail CThingManager NonScriptedEntitesPauseMode +0x54 / AllEntitesPauseMode +0x55 (Ego_r.pdb +0x68/+0x69 less
//   the Dinkumware container bytes); engine/CPlayer.h SpellMode +0x218; retail CThing UsingComponents +0x20, Components
//   +0x44; CCombatActionBase 0x114 bytes (landed 0x0062D380 model).
// Shape: IS_THING_ALIVE gets a same-TU noinline body (null test, dead bit +0x91) so VC7.1 knows the creature slot it is
//   handed by reference is only read: the creature stays in ebx and the slot is reused for the TC keys (0x120 frame);
//   the first GetPlayer result is a block-scoped local (retail calls GetPlayer before pushing the mode).
//   The const lookups test the bit through the bitset reference proxy (retail `test byte ptr`), not _M_test.
// Shape: GetTC as the landed 0x00635080 (bitset reference proxy, LowerBound, end / greater test); the carrying, weapon
//   and berserk lookups go through const things (the const LowerBound copy); the start-blocking action is a temporary
//   inside the `&&`, so its destructor is guarded by the flag word VC7.1 clears at entry.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
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
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    bool _M_test(size_t __pos) const { return (_M_w[_S_whichword(__pos)] & _S_maskbit(__pos)) != 0; }
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
    bool operator[](size_t __pos) const { return this->_M_test(__pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    iterator LowerBound(const K& key);                        // 0x0040F020 for the TC map
    const_iterator LowerBound(const K& key) const;            // 0x004365B0 for the TC map
};

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;
class CThing;
class CTCCombat { public:
    enum { INTERFACE_TYPE = 0x30 };
    bool IsBlocking() const;                                  // 0x007331E0
    void SetBlocking();                                       // 0x007337E0
};
class CTCCarrying { public:
    enum { INTERFACE_TYPE = 0x46 };
    CThing* GetThingInPrimarySlot() const;                    // 0x006A1440
};
class CTCWeapon { public:
    enum { INTERFACE_TYPE = 0x48 };
    bool IsUsableBy(const CThing& thing) const;               // 0x005DAB10
};
class CTCBerserk { public:
    enum { INTERFACE_TYPE = 0x84 };
    bool CanBerserkBlock() const;                             // 0x0078BEF0
};

class CThing {                                              // partial view, retail offsets
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                     // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      DeadFlags;                           // +0x91 (bit 0: dead)
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
    template <class T> __forceinline bool GetTC(const T** out) const
    {
        if (const_cast<CThing*>(this)->UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureActionBase;
class CThingCreatureBase : public CThingGameObject {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase { };

template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0
{
    return thing && !(thing->DeadFlags & 1);
}

class CCreatureActionBase {                                // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                        // 0x00693EF0
    unsigned char _body[0xa8 - 4];
};
class __declspec(novtable) CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114
public:
    unsigned char _combat[0x114 - 0xa8];
    virtual ~CCombatActionBase();                          // 0x0062E900
};
class CCreatureAction_StartBlocking : public CCombatActionBase {
public:
    CCreatureAction_StartBlocking(CThingCreatureBase& creature);   // 0x00855BE0
};

class CGameEvent {                                         // 0x28 bytes (Ego_r.pdb = retail)
public:
    long          Type;                                    // +0x00
    signed char   Player;                                  // +0x04
    unsigned char Data[32];                                // +0x05
    unsigned char EndPos;                                  // +0x25
    bool          Valid;                                   // +0x26
    bool          Replacement;                             // +0x27
};

enum EPlayerMode { PLAYER_MODE_NULL = 0 };
class CPlayer : public CPlayerPdb {
public:
    bool HasMode(EPlayerMode mode) const;                  // 0x00634160
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
    bool IsMainPlayer() const;                             // 0x00487520
};

class CThingManager {                                      // partial view, retail offsets
public:
    unsigned char _pad00[0x54];
    bool NonScriptedEntitesPauseMode;                      // +0x54
    bool AllEntitesPauseMode;                              // +0x55
};

class CWorld : public CWorldDonor {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager& GetThingManager();              // slot 3 (+0x0C, 0x0049E1B0)
protected:
    CPlayer* GetPlayer(unsigned long number);              // 0x0049D850
public:
    void EACreatureBlock(const CGameEvent* event);
};

void CWorld::EACreatureBlock(const CGameEvent* event)
{
    {
        CPlayer* modePlayer = GetPlayer(event->Player);
        if (modePlayer->HasMode((EPlayerMode)0x2f))
            return;
    }
    CPlayer* player = GetPlayer(event->Player);
    CThingPlayerCreature* creature = player->GetPControlledCreature();
    if (GetThingManager().NonScriptedEntitesPauseMode)
        return;
    if (GetThingManager().AllEntitesPauseMode)
        return;
    if (player->SpellMode)
        return;
    if (!player->IsMainPlayer())
        return;
    if (!IS_THING_ALIVE(creature))
        return;
    CTCCombat* combat;
    if (!creature->GetTC(&combat))
        return;
    if (combat->IsBlocking())
        return;
    const CThingPlayerCreature* constCreature = creature;
    bool canBlock = true;
    const CTCCarrying* carrying;
    if (constCreature->GetTC(&carrying)) {
        const CThing* weapon = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weaponTC;
        if (weapon && weapon->GetTC(&weaponTC)) {
            if (!weaponTC->IsUsableBy(*creature))
                canBlock = false;
        }
        const CTCBerserk* berserk;
        if (constCreature->GetTC(&berserk))
            canBlock = berserk->CanBerserkBlock();
        if (!canBlock)
            return;
    }
    if (!combat->IsBlocking() && creature->SetCurrentAction(CCreatureAction_StartBlocking(*creature)))
        combat->SetBlocking();
}