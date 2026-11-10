#ifndef FABLE_ENGINE_CQ_DRAGONBOSSFIGHTSCRIPT_H
#define FABLE_ENGINE_CQ_DRAGONBOSSFIGHTSCRIPT_H
// Retail opt-in (chunk 790): NScript::CQ_DragonBossFightScript, new header (no includer before chunk 790).
// Layout: Ego_r.pdb CQ_DragonBossFightScript (struct_layouts_egor.tsv, 184 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after each of the two STLport vectors (0xC, Dinkumware 0x10): DragonState +0x48,
// MinionSpawnPoints +0x4c, SummonerSpawnPoints +0x58, NextSummonerSpawnPoint +0x64 .. SummonerSpawningEnabled +0x81, cs_flags
// map<CCharString,bool> +0x84, MinionSpawnDelay / SummonerSpawnDelay CTimer +0x90 / +0x94, sizeof 0x98 (the Alloc 0x00D27240
// news 0x98). Checked against retail: DoMission 0x00D26190 waits on DragonState +0x48 == 4 and looks up
// cs_flags["PutMaskOn"] at `lea ecx,[esi+84h]` (the landed PlayCutscene 0x00D27050 passes &cs_flags = this+0x84 too).
// Retail methods: Ego_r.pdb name set ?<Name>@CQ_DragonBossFightScript@NScript@@; RegisterMain 0x00D25520, Main 0x00D255E0, Init
// 0x00D254B0 (landed); DoMission 0x00D26190 spawns RunEnemySpawning 0x00D26BA0 and JackTaunts 0x00D26A50 under their thread-name
// literals (CSpawnedFunc<CQ_DragonBossFightScript> vtable 0x012CB720); PlayCutscene(CCharString) 0x00D27050 (landed).
// Nested entity scripts (Ego_r.pdb ?Main@CDragon@CQ_DragonBossFightScript@NScript@@ ...), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (DoMission writes MasterScriptData BodyGuardsInLimbo +0xe4, HeroWoreMask +0x133).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_DragonBossFightScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_DragonBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    #ifdef FABLE_RETAIL_DRAGON_ORIGINAL_TYPES
    enum EDragonState { DS_HEALTH_HIGH, DS_HEALTH_MEDIUM, DS_HEALTH_LOW, DS_HEALTH_VERY_LOW, DS_DEAD };
#else
    enum EDragonState { };
#endif                                               // Ego_r.pdb enum (DoMission waits for the value 4)
#ifdef FABLE_RETAIL_DRAGON_ORIGINAL_TYPES
    typedef CQ_DragonBossFightScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CQ_DragonBossFightScript(const CQ_DragonBossFightScript&);
    CQ_DragonBossFightScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL * FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    void SpawnEnemies();
    virtual ~CQ_DragonBossFightScript();
    CQ_DragonBossFightScript& operator=(const CQ_DragonBossFightScript&);
#endif
    virtual void RegisterMain();                                         // 0x00D25520 (slot 1)
    virtual void Main();                                                 // 0x00D255E0 (slot 2)
    virtual void Init();                                                 // 0x00D254B0 (slot 3)
    void DoMission();                                                    // 0x00D26190
    void JackTaunts();                                                   // 0x00D26A50
    void RunEnemySpawning();                                             // 0x00D26BA0
    void SpawnMinions();                                                 // 0x00D26CC0
    void SpawnSummoners();                                               // 0x00D26E60
    void PlayCutscene(CCharString);                                      // 0x00D27050
#ifdef FABLE_RETAIL_DRAGON_STATE_BODY
    // Complete SetDragonState 00d258c0; original state names from both raw donors.
    void SetDragonState(EDragonState dragonState);
#endif
    EDragonState DragonState;                                            // +0x48
    std::vector<CScriptThing> MinionSpawnPoints;                         // +0x4c
    std::vector<CScriptThing> SummonerSpawnPoints;                       // +0x58
    long NextSummonerSpawnPoint;                                         // +0x64
    long TargetNumMinions;                                               // +0x68
    long TargetNumSummoners;                                             // +0x6c
    long NumMinions;                                                     // +0x70
    long NumSummoners;                                                   // +0x74
    long NumFlyBysBetweenSummonerSpawns;                                 // +0x78
    long NumFlyBysSinceLastSummonerSpawn;                                // +0x7c
    bool MinionSpawningEnabled;                                          // +0x80
    bool SummonerSpawningEnabled;                                        // +0x81
    std::map<CCharString,bool> cs_flags;                                 // +0x84
    CTimer MinionSpawnDelay;                                             // +0x90
    CTimer SummonerSpawnDelay;                                           // +0x94
    class CDragon;
    class CDBMinion;
    class CDBSummoner;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_DragonBossFightScript) == 0x98);
#ifdef FABLE_RETAIL_DRAGON_ORIGINAL_TYPES
namespace NScript {
class CQ_DragonBossFightScript::CDragon : public CEntityScriptBase {
    typedef CQ_DragonBossFightScript TParentClass;
protected:
    CQ_DragonBossFightScript& ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
public:
    CDragon(CQ_DragonBossFightScript&,CScriptDataBase*,const CScriptThing&);
    CDragon(const CDragon&);
    virtual ~CDragon();
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_DragonBossFightScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const { return &ParentClass; }
    CQ_SunnyvaleMasterData* GetMasterData();
private:
    float MediumHealth,LowHealth,VeryLowHealth;
    virtual void Init();
    virtual void OnPredicateFail();
    virtual void Main();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_DragonBossFightScript::CDragon)==0x28);
#endif


#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_DRAGONBOSSFIGHTSCRIPT_H
