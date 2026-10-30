// NScript::CQ_ArenaScript::CArenaEnemy::OnPredicateFail() -- an arena enemy left the fight: one fewer of its creature type alive; if the hero killed it, add the current round/wave creature def's DeathScore to the crowd points; drop its quest-info health bar if it had one
// Leaf: ?OnPredicateFail@CArenaEnemy@CQ_ArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F1A8F0, 147 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): the row is referenced once, from slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F9C50
//   (0x012F9C64), which the landed NScript::CQ_ArenaScript::CArenaEnemy::Alloc 0x00F1A810 stores (slot 1 = the landed
//   CArenaEnemy Main 0x00F1A990; slots 4 / 6 the CEntityScriptBase defaults 0x00CDEBC0 / 0x00CDEBE0). Ego_r.pdb gives
//   CArenaEnemy exactly OnPredicateFail, Alloc and Main: ?OnPredicateFail@CArenaEnemy@CQ_ArenaScript@NScript@@EAEXXZ
//   (private virtual). Entity slot order (landed CTyler::Init 0x00E836F0): 0 dtor, 1 Main, 2 Init, 3 GetParentScript,
//   4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Catalog label sub_00f1a8f0 (gap scan).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CScriptThing slot 18 MsgIsKilledBy (+0x48);
//   CGameScriptInterface slot 338 RemoveQuestInfoElement (+0x548, ghidra_out/cgsi_slots_aligned.tsv).
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CQ_ArenaScript::CArenaEnemy (40 bytes = the Alloc's new size): ParentClass +0x14, PMasterData +0x18,
//   HealthBarIndex +0x1c, BigCreature +0x20, MyCreatureType +0x24. CQ_ArenaScript at Ego_r - 0x14 (retail CScriptBase 0x40),
//   less 4 after the Dinkumware Rounds vector: Rounds +0x98, ArenaRound +0xa8, ArenaRoundWave +0xac, TotalCreatures +0xd0,
//   NewCrowdPoints +0xf4. Def elements are the retail polymorphic chain (FABLE_RETAIL_DEF_CLASS_BASE): CArenaRoundDef 0x38
//   (Waves +0x2c), CArenaWaveDef 0x3c (Creatures +0x2c), CArenaCreatureDef 0x38 (DeathScore +0x34) -- the strides retail uses.
// Shape: STLport `operator[]` as `*(begin() + n)`; the bool result kept in bl across the temporary's destructor.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    iterator begin() { return this->_M_start; }
    reference operator[](unsigned int __n) { return *(begin() + __n); }
};
}
class CArenaCreatureDef : public CDefClassBase {       // vtable 0x012FB358, 0x38 bytes
public:
    virtual ~CArenaCreatureDef();
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase* other);      // slot 19
    virtual unsigned long GetSizeofClass();             // slot 20
    CCharString CreatureType;                           // +0x28
    long        NumCreatures;                           // +0x2c
    CCharString HUDType;                                // +0x30
    long        DeathScore;                             // +0x34
};
class CArenaWaveDef : public CDefClassBase {           // vtable 0x012FB2F0, 0x3c bytes
public:
    virtual ~CArenaWaveDef();
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F261F0
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                           NumWaveCreatures;    // +0x28
    std::vector<CArenaCreatureDef> Creatures;           // +0x2c
    bool                           ShortWave;           // +0x38
};
class CArenaRoundDef : public CDefClassBase {          // 0x38 bytes
public:
    virtual ~CArenaRoundDef();
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                        NumWaves;               // +0x28
    std::vector<CArenaWaveDef>  Waves;                  // +0x2c
};

#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // vtable 0x01238C8C (79 slots), ~CScriptThing 0x004AA840
#include "engine/CGameScriptInterfaceBase.h"           // vtable slots: CGameScriptInterface 0x01260F0C
#include "engine/CEntityScriptBase.h"                  // vtable 0x012C3224
#include "engine/CLevelScriptBase.h"                   // CScriptBase (0x40, vtable 0x012C1648) + Game +0x40, MasterScriptData +0x44

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCharString                  CrowdLoopTags[4][5];   // +0x48
    std::vector<CArenaRoundDef>  Rounds;                // +0x98
    long                         ArenaState;            // +0xa4
    long                         ArenaRound;            // +0xa8
    long                         ArenaRoundWave;        // +0xac
    long                         GoldMultiplier;        // +0xb0
    bool                         ChamLeaving;           // +0xb4
    long                         GoldTotal;             // +0xb8
    bool                         InHitCutsceneAlready;  // +0xbc
    bool                         NeedBertForSpeech;     // +0xbd
    CScriptThing                 CellsVillage;          // +0xc0
    bool                         PlayerLeaving;         // +0xcc
    bool                         PlayerWon;             // +0xcd
    long                         TotalCreatures[3];     // +0xd0
    long                         ExtraCreatures;        // +0xdc
    bool                         MissionSucceeded;      // +0xe0
    bool                         MissionFailed;         // +0xe1
    bool                         ArenaSpawnNeeded[16];  // +0xe2
    long                         NewCrowdPoints;        // +0xf4
    class CArenaEnemy;
};
class CQ_ArenaScript::CArenaEnemy : public CEntityScriptBase {
public:
    CQ_ArenaScript*          ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    long                     HealthBarIndex;            // +0x1c
    bool                     BigCreature;               // +0x20
    long                     MyCreatureType;            // +0x24
private:
    virtual void OnPredicateFail();
};

void CQ_ArenaScript::CArenaEnemy::OnPredicateFail()
{
    --ParentClass->TotalCreatures[MyCreatureType];
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
        ParentClass->NewCrowdPoints += ParentClass->Rounds[ParentClass->ArenaRound].Waves[ParentClass->ArenaRoundWave].Creatures[MyCreatureType].DeathScore;
    if (BigCreature)
        Game->RemoveQuestInfoElement(HealthBarIndex);
}
}   // namespace NScript