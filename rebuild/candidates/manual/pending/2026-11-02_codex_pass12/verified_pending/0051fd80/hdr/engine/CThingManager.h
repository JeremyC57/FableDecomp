#ifndef FABLE_ENGINE_CTHINGMANAGER_H
#define FABLE_ENGINE_CTHINGMANAGER_H
// Retail native Component+1c, persistence+a0, SerialiseVersionNumber+d8.
// Caller0049ebf1 allocates0xe8 before constructor00523540; original PDB
// member order with retail STLport sizes and natural eight-byte alignment.
#ifdef FABLE_RETAIL_CTHINGMANAGER_TEXT
#include "stlport/fable_set.h"
class CASuspendableProcess;
#endif
#include "engine/CClassFactory.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCharString.h"
#ifdef FABLE_RETAIL_INIT_THING_TYPES
#include "engine/CThingTypeInfo.h"
#include "engine/CThingTypeInit.h"
#include "engine/ThingLList.h"
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "engine/CWideString.h"
#endif
class CPersistContext; class CThing; class CGameDefinitionManager;
class CWorld; class CWorldMap; class CPlayerManager; class CNavigatorManager; class CEnvironment;
class CThingManager : public CClassFactory {
public:
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    void InitThingTypes();
#endif
    CThingManager(const CMainGameComponent&,const CGameDefinitionManager&,CWorld&,CPlayerManager&,CNavigatorManager&,unsigned long*,unsigned long,unsigned long,unsigned long);
    virtual ~CThingManager();
    void LoadHeroStateBinary(CPersistContext&);
#ifdef FABLE_RETAIL_CTHINGMANAGER_TEXT
    void LoadEntitiesFromScript(const CCharString&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&);
#endif
#ifdef FABLE_RETAIL_LOAD_SINGLE_THING
    const CGameDefinitionManager& PeekDefinitionManager()const{return DefinitionManager;}
    CWorld& GetWorld(){return World;}
#ifdef FABLE_RETAIL_LOAD_SINGLE_LEVEL_ACCESSOR
    long GetLevelBeingLoaded()const{return LevelBeingLoaded;}
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOAD
    void CheckAndFixUID(unsigned __int64&);
    CThing* GetPThingWithUID(unsigned __int64 uid) {
        std::map<unsigned __int64,CThing*>::iterator it=UIDToThingMap.find(uid);
#ifdef FABLE_RETAIL_LOAD_SINGLE_THING
        CThing* pthing=0;
        if(it!=UIDToThingMap.end())pthing=it->second;
        return pthing;
#else
        return it==UIDToThingMap.end()?0:it->second;
#endif
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 RemapUIDForSerialisation(bool,bool,unsigned long,unsigned __int64) const;
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    void SaveGameState(CPersistContext&);
#ifdef FABLE_RETAIL_LOAD_GAME_STATE
    void LoadGameState(CPersistContext&);
    void LoadHeroStateScript(CStringParser&);
    void SetNameForThing(unsigned __int64,const CWideString&);
#endif
#endif
protected:
    virtual void OnCreateClass(CClass&);
    virtual void OnDeleteClass(CClass&);
private:
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    void AddNewThingType(const CThingTypeInit& init) {
        CClassFactory::AddNewClassType(init);
        ThingTypeInfo[init.TypeID].ThingTypeFlags=init.ThingTypeFlags;
        ThingTypeInfo[init.TypeID].DefClassType=init.DefClassType;
    }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_TEXT
    void ActivateAfterLoading(long);
#endif
    CThing* LoadSingleThing(CPersistContext&,long);
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    void RuntimeSaveGameHero(bool);
    void RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool);
#endif
    const CMainGameComponent& Component;
    const CGameDefinitionManager& DefinitionManager;
    CWorld& World;
    CWorldMap& WorldMap;
    unsigned long* WorldSeed;
    CPlayerManager& PlayerManager;
    CNavigatorManager& NavigationManager;
    const CEnvironment* PEnvironment;
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    std::vector<CThingTypeInfo> ThingTypeInfo;
#else
    unsigned char ThingTypeInfo[12]; // std::vector<CThingTypeInfo> +3c
#endif
    unsigned char DeadThingList[12]; // std::vector<CThing*> +48
    bool NonScriptedEntitesPauseMode;
    bool AllEntitesPauseMode;
    bool FirstRenderOfFrame;
    unsigned char _pad_0x57;
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOAD
    std::map<unsigned __int64,CThing*> UIDToThingMap;
#else
    unsigned char UIDToThingMap[12]; // std::map<unsigned __int64,CThing*> +58
#endif
    unsigned __int64 CurrentUniqueID;
    unsigned __int64 CurrentLocalThingUniqueID;
    bool DrawCombatCollisionDebug;
    bool DrawAttitudeDebug;
    long MaxThingDrawDist;
    unsigned char GlobalThingLoadingBehaviour[4]; // CThingManager::EGlobalThingLoadingBehaviour +80
    unsigned char RenderList[8]; // ThingLList +84
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    std::vector<ThingLList> UpdateLists;
#else
    unsigned char UpdateLists[12]; // std::vector<ThingLList> +8c
#endif
    bool UseMapSpecificUIDs;
    bool UpdateAndRenderAllFlag;
    bool SerialisingGameState;
    bool CurrentlyLoading;
    bool LoadingEntitiesFromSaveGame;
    unsigned char _pad_0x9d[3];
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence;
#else
    unsigned char EntityRuntimePersistence[40]; // NThingManager::CEntityRuntimePersistence +a0
#endif
    bool ErrorFlag;
    unsigned char _pad_0xc9[3];
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    std::map<unsigned __int64,std::pair<CCharString,CWideString> > NPCNameMap;
#else
    unsigned char NPCNameMap[12]; // std::map<unsigned __int64,std::pair<CCharString,CWideString> > +cc
#endif
    long SerialiseVersionNumber;
    long LevelBeingLoaded;
    CCharString CurrentSectionName;
};
FABLE_STATIC_ASSERT(sizeof(CThingManager)==0xe8);
#endif
