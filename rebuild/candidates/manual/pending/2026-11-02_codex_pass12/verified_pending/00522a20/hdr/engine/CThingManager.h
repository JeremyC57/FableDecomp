#ifndef FABLE_ENGINE_CTHINGMANAGER_H
#define FABLE_ENGINE_CTHINGMANAGER_H
// Retail native Component+1c, persistence+a0, SerialiseVersionNumber+d8.
// Caller0049ebf1 allocates0xe8 before constructor00523540; original PDB
// member order with retail STLport sizes and natural eight-byte alignment.
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
#ifdef FABLE_RETAIL_MAP_BRUSHES
    CThing* GetPThingWithUID(unsigned __int64);
#endif
    void LoadHeroStateBinary(CPersistContext&);
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 RemapUIDForSerialisation(bool,bool,unsigned long,unsigned __int64) const;
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    void SaveGameState(CPersistContext&);
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
    unsigned char UIDToThingMap[12]; // std::map<unsigned __int64,CThing*> +58
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
