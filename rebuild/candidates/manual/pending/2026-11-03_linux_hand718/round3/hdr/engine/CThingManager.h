#ifndef FABLE_ENGINE_CTHINGMANAGER_H
#define FABLE_ENGINE_CTHINGMANAGER_H
// Retail class (Codex pass 11 model; consolidation 2 merged the retry11a rows 0x00520670 / 0x00521620 and cons11b row
// 0x00606300): CClassFactory base (0x1c), Component+1c, persistence+a0, SerialiseVersionNumber+d8. Needs FABLE_RETAIL_CCLASSFACTORY.
// Caller0049ebf1 allocates0xe8 before constructor00523540; original PDB
// member order with retail STLport sizes and natural eight-byte alignment.
#include "engine/CClassFactory.h"
#include "fable_string.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"
class CMainGameComponent; class CPersistContext; class CThing; class CGameDefinitionManager;
class CWorld; class CWorldMap; class CPlayerManager; class CNavigatorManager; class CEnvironment;
class CMemoryDataInputStream; class CASuspendableProcess;
class CThingManager : public CClassFactory {
public:
    CThingManager(const CMainGameComponent&,const CGameDefinitionManager&,CWorld&,CPlayerManager&,CNavigatorManager&,unsigned long*,unsigned long,unsigned long,unsigned long);
    virtual ~CThingManager();
    void LoadHeroStateBinary(CPersistContext&);
    // Ego_r + FableWin signatures (2026-11-03, row 0x00522720): the name set and thing vector are references.
    void LoadEntitiesFromScript(const CCharString&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&); // 0x00520D00
    void LoadEntitiesBinary(CMemoryDataInputStream&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&); // 0x00521240
    void RuntimeLoadEntities(long,const std::set<CCharString>&,std::vector<CThing*>&); // 0x00522720
    unsigned __int64 RemapUIDForSerialisation(bool,bool,unsigned long,unsigned __int64) const;
#ifdef FABLE_RETAIL_CTHINGMANAGER_PEEK
    // Retail opt-in (chunk 718, row 0x0089E190): Ego_r.pdb / FableWin.pdb ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z,
    // the landed body 0x0051ED60 (the only spelling bound to it). A declaration only: no layout change.
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;        // 0x0051ED60
#endif
protected:
    virtual void OnCreateClass(CClass&);
    virtual void OnDeleteClass(CClass&);
private:
    CThing* LoadSingleThing(CPersistContext&,long);
    const CMainGameComponent& Component;
    const CGameDefinitionManager& DefinitionManager;
    CWorld& World;
    CWorldMap& WorldMap;
    unsigned long* WorldSeed;
    CPlayerManager& PlayerManager;
    CNavigatorManager& NavigationManager;
    const CEnvironment* PEnvironment;
    unsigned long ThingTypeInfo[3]; // std::vector<CThingTypeInfo> +3c
    unsigned long DeadThingList[3]; // std::vector<CThing*> +48
    bool NonScriptedEntitesPauseMode;
    bool AllEntitesPauseMode;
    bool FirstRenderOfFrame;
    unsigned long UIDToThingMap[3]; // std::map<unsigned __int64,CThing*> +58
    unsigned __int64 CurrentUniqueID;
    unsigned __int64 CurrentLocalThingUniqueID;
    bool DrawCombatCollisionDebug;
    bool DrawAttitudeDebug;
    long MaxThingDrawDist;
    unsigned long GlobalThingLoadingBehaviour; // CThingManager::EGlobalThingLoadingBehaviour +80
    unsigned long RenderList[2]; // ThingLList +84
    unsigned long UpdateLists[3]; // std::vector<ThingLList> +8c
    bool UseMapSpecificUIDs;
    bool UpdateAndRenderAllFlag;
    bool SerialisingGameState;
    bool CurrentlyLoading;
    bool LoadingEntitiesFromSaveGame;
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence; // +a0
    bool ErrorFlag;
    unsigned long NPCNameMap[3]; // std::map<unsigned __int64,std::pair<CCharString,CWideString> > +cc
    long SerialiseVersionNumber;
    long LevelBeingLoaded;
    CCharString CurrentSectionName;
};
FABLE_STATIC_ASSERT(sizeof(CThingManager)==0xe8);
#endif
