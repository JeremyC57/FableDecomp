#ifndef FABLE_ENGINE_CTHINGMANAGER_H
#define FABLE_ENGINE_CTHINGMANAGER_H
// Retail class (Codex pass 11 model; consolidation 2 merged the retry11a rows 0x00520670 / 0x00521620 and cons11b row
// 0x00606300): CClassFactory base (0x1c), Component+1c, persistence+a0, SerialiseVersionNumber+d8. Needs FABLE_RETAIL_CCLASSFACTORY.
// Caller0049ebf1 allocates0xe8 before constructor00523540; original PDB
// member order with retail STLport sizes and natural eight-byte alignment.
#include "engine/CClassFactory.h"
#include "fable_string.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "engine/CThingTypeInfo.h"
#include "engine/CThingTypeInit.h"
#include "engine/ThingLList.h"
class CMainGameComponent; class CPersistContext; class CThing; class CGameDefinitionManager;
class CWorld; class CWorldMap; class CPlayerManager; class CNavigatorManager; class CEnvironment;
class CMemoryDataInputStream; class CASuspendableProcess;
#ifdef FABLE_RETAIL_CTHINGMANAGER_RUNTIMESAVE
class CMemoryDataOutputStream;                                  // row 0x00523CF0: SaveEntitiesBinary's parameter
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
class CThingSearchTools;                                        // chunk 765
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER
class CEnginePrimitivePrepareInfo;                              // chunk 734: PreparePrimitivesForEngine's parameter
#endif
class CThingManager : public CClassFactory {
#if defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT) || defined(FABLE_RETAIL_CTHINGMANAGER_ISDRAWCOMBATCOLLISIONDEBUG)
// Evidence: both original PDB type records and donor OnHit/helper disassembly.
// (wf7, row 0x007FD650: FABLE_RETAIL_CTHINGMANAGER_ISDRAWCOMBATCOLLISIONDEBUG = this accessor alone.)
public:
bool IsDrawCombatCollisionDebug()const{return DrawCombatCollisionDebug;}
#endif
#ifdef FABLE_RETAIL_STEALTH_DRAW
private:static bool DrawPerceptionDebug;
public:bool IsDrawingPerceptionDebug()const{return DrawPerceptionDebug;}
#endif
public:
    CThingManager(const CMainGameComponent&,const CGameDefinitionManager&,CWorld&,CPlayerManager&,CNavigatorManager&,unsigned long*,unsigned long,unsigned long,unsigned long);
    virtual ~CThingManager();
#ifdef FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
    // Retail opt-in (Codex pass18): Ego_r0052d760/FableWin01d45200; public original member.
    void SetNameForThing(unsigned __int64 uid,const CWideString& new_name);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_RUNTIMESAVE
    // Opt-in (row 0x00523CF0): Ego_r.pdb ?SaveEntitiesToScript@CThingManager@@QAEXAAVCCharString@@JABV?$vector@PAVCThing@@...AAV?$vector@VCCharString@@...
    // (public, 0x00521EB0) and ?SaveEntitiesBinary@CThingManager@@QAEXAAVCMemoryDataOutputStream@@J... (public, 0x00522160). Declarations only.
    void SaveEntitiesToScript(CCharString&,long,const std::vector<CThing*>&,std::vector<CCharString>&);          // 0x00521EB0
    void SaveEntitiesBinary(CMemoryDataOutputStream&,long,const std::vector<CThing*>&,std::vector<CCharString>&); // 0x00522160
private:
    // Ego_r.pdb ?RuntimeSaveGamePersistentEntitiesInLoadedMaps@CThingManager@@AAEX_N@Z (private, 0x00523CF0).
    void RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool);           // 0x00523CF0
public:
#endif
    void InitThingTypes();                                              // 0x00522A20 (Ego_r.pdb ?InitThingTypes@CThingManager@@QAEXXZ)
#ifdef FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
    // Opt-in (agentNF14, row 0x006C8780 CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn): FableWin.pdb
    // ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ (0x01CF6420; ?GetWorld@CThingManager@@QAEAAVCWorld@@XZ 0x01CF63E0 is its twin), inline
    // in retail (the World reference +0x24 read in place, 0x006C881D). Member function only: no layout change.
    const CWorld& PeekWorld() const { return World; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_GETWORLD
    // Opt-in (agentNF31 name-fix pass, row 0x0073A680 CTCCreatureModeManager::CModeBase::GetWorld): FableWin.pdb
    // ?GetWorld@CThingManager@@QAEAAVCWorld@@XZ (0x01CF63E0; _fablewin_api 0x017CB8C0), the non-const twin of PeekWorld, inline in
    // retail (the World reference +0x24 read in place). Member function only: no layout change.
    CWorld& GetWorld() { return World; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
    // Retail opt-in (chunk 846, row 0x0051FD80 LoadSingleThing): the same Ego_r.pdb member with its body. Retail keeps an out-of-line
    // copy at 0x0051ED80 and expands it in LoadSingleThing (`lea esi,[ebx+58h] ; call 00527D80h` = the landed UIDToThingMap _M_find,
    // `cmp eax,[esi] ; je ; mov ebp,[eax+18h]` with ebp = 0 before the call, 0x0051FFC6..0x0051FFE9).
    CThing* GetPThingWithUID(unsigned __int64 uid) {
        std::map<unsigned __int64,CThing*>::iterator it=UIDToThingMap.find(uid);
        CThing* pthing=0;
        if(it!=UIDToThingMap.end()) pthing=it->second;
        return pthing;
    }
    // Ego_r.pdb / FableWin.pdb ?CheckAndFixUID@CThingManager@@QAEXAA_K@Z, out of line at 0x0051F340 (landed). Declaration only.
    void CheckAndFixUID(unsigned __int64&);                             // 0x0051F340
    // Ego_r.pdb ?PeekDefinitionManager@CThingManager@@QBEABVCGameDefinitionManager@@XZ (reads +0x20 in place, 0x0051FF3A) and
    // ?GetLevelBeingLoaded@CThingManager@@QBEJXZ (+0xDC in place at 0x0051FFB0; retail also keeps an out-of-line copy at 0x0051E2C0).
    const CGameDefinitionManager& PeekDefinitionManager() const { return DefinitionManager; }
    long GetLevelBeingLoaded() const { return LevelBeingLoaded; }
#else
    CThing* GetPThingWithUID(unsigned __int64);                         // Ego_r.pdb ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z (0x00821010)
#endif
    void LoadHeroStateBinary(CPersistContext&);
    // Ego_r + FableWin signatures (2026-11-03, row 0x00522720): the name set and thing vector are references.
    void LoadEntitiesFromScript(const CCharString&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&); // 0x00520D00
    void LoadEntitiesBinary(CMemoryDataInputStream&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&); // 0x00521240
    void RuntimeLoadEntities(long,const std::set<CCharString>&,std::vector<CThing*>&); // 0x00522720
    unsigned __int64 RemapUIDForSerialisation(bool,bool,unsigned long,unsigned __int64) const;
    // Retail (chunk 718, row 0x0089E190; the FABLE_RETAIL_CTHINGMANAGER_PEEK opt-in folded into the retail-only class at landing): Ego_r.pdb / FableWin.pdb ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z,
    // the landed body 0x0051ED60 (the only spelling bound to it). A declaration only: no layout change.
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;        // 0x0051ED60
    // Codex pass 13 (row 0x004FB990): FableWin ?GetCurrentThingUID@CThingManager@@QBE_KXZ, inline in retail (reads CurrentUniqueID).
    unsigned __int64 GetCurrentThingUID() const { return CurrentUniqueID; }
#ifdef FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS
    // Retail opt-in (chunk 2008, row 0x004CA390 CThing::ConstructFromBinarySave): Ego_r.pdb 0x00528260 ?ThingTypeToTypeFlags@CThingManager@@QBEKJ@Z,
    // out of line in retail at 0x0051E580 (`mov ecx,[ecx+3Ch] ; lea eax,[eax+eax*2] ; mov eax,[ecx+eax*4+4]`: ThingTypeInfo[type].ThingTypeFlags;
    // landed callers bind it as GetThingTypeFlags@QAEKJ), and FableWin.pdb ?PeekComponent@CThingManager@@QBEABVCMainGameComponent@@XZ
    // (0x01CF6360, inline in retail: the Component reference +0x1C read in place, 0x004CA57C). No layout change.
    unsigned long ThingTypeToTypeFlags(long type) const;                // 0x0051E580
    const CMainGameComponent& PeekComponent() const { return Component; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb / FableWin.pdb ?WorldUpdateThings@CThingManager@@QAEXXZ (0x0051FCC0)
    // and ?UpdateDeaths@CThingManager@@QAEXXZ (0x0051F070, landed), and the FableWin-only inline
    // ?IsNonScriptedEntitesPauseMode@CThingManager@@QBE_NXZ (UpdateFrame reads +0x54 in place at 0x004A5D38). No layout change.
    void WorldUpdateThings();                                           // 0x0051FCC0
    void UpdateDeaths();                                                // 0x0051F070
    bool IsNonScriptedEntitesPauseMode() const { return NonScriptedEntitesPauseMode; }
#ifdef FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
    // Nested opt-in (chunk 758, row 0x00725B70 CTCPhysicsControlled::UpdateAdditionalImpulse): FableWin.pdb
    // ?IsAllEntitesPauseMode@CThingManager@@QBE_NXZ, inline in retail (the row reads +0x55 in place). No layout change.
    bool IsAllEntitesPauseMode() const { return AllEntitesPauseMode; }
#endif
    // Chunk 734 (row 0x004A1CB0): Ego_r.pdb / FableWin.pdb ?PreparePrimitivesForEngine@CThingManager@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
    // (retail 0x0051EAB0, landed).
    void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);   // 0x0051EAB0
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_PEEKPLAYERMANAGER
    // Opt-in (wf14, row 0x004C9920 CThing::IsAffectedByBulletTime): FableWin.pdb ?PeekPlayerManager@CThingManager@@QBEABVCPlayerManager@@XZ
    // (0x01CF6220) / ?GetPlayerManager@CThingManager@@QAEAAVCPlayerManager@@XZ (0x01CF6260), inline in retail (`mov ecx,[ecx+30h]`
    // at 0x004C9956). No layout change.
    const CPlayerManager& PeekPlayerManager() const { return PlayerManager; }
    CPlayerManager& GetPlayerManager() { return PlayerManager; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
    // Opt-in (chunk 765, rows 0x004C7A50 / 0x004C7A60 / 0x004C7BC0 / 0x00686A90 / 0x00520C70): Ego_r.pdb / FableWin.pdb public
    // members, out of line in retail in Ego_r's donor order: ?GetSerialiseVersion@CThingManager@@QBEJXZ 0x0051E2D0 (+0xD8),
    // ?IsSerialisingGameState@CThingManager@@QBE_NXZ 0x0051E2E0 (+0x9A; after GetLevelBeingLoaded 0x0051E2C0 +0xDC, as Ego_r
    // 0x005281F0 / 0x00528200 / 0x00528210), ?PeekThingSearchTools@CThingManager@@QBEABVCThingSearchTools@@XZ 0x0051E480,
    // ?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ 0x0051E490 and
    // ?ClearPersistenceDataForSection@CThingManager@@QAEXABVCCharString@@@Z 0x00520C70. Declarations only: no layout change.
    long GetSerialiseVersion() const;                                   // 0x0051E2D0
    bool IsSerialisingGameState() const;                                // 0x0051E2E0
    const CThingSearchTools& PeekThingSearchTools() const;              // 0x0051E480
    CThingSearchTools& GetThingSearchTools();                           // 0x0051E490
    void ClearPersistenceDataForSection(const CCharString& section);   // 0x00520C70
#endif
protected:
    virtual void OnCreateClass(CClass&);
    virtual void OnDeleteClass(CClass&);
private:
    // Ego_r.pdb ?AddNewThingType@CThingManager@@AAEXABVCThingTypeInit@@@Z (out of line at 0x005282A0 in Ego_r); retail expands it in
    // InitThingTypes 0x00522A20 (the AddNewClassType virtual call, then the two ThingTypeInfo stores).
    void AddNewThingType(const CThingTypeInit& init) {
        CClassFactory::AddNewClassType(init);
        ThingTypeInfo[init.TypeID].ThingTypeFlags=init.ThingTypeFlags;
        ThingTypeInfo[init.TypeID].DefClassType=init.DefClassType;
    }
    CThing* LoadSingleThing(CPersistContext&,long);
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
    // Ego_r.pdb / FableWin.pdb ?ActivateAfterLoading@CThingManager@@AAEXJ@Z (private, out of line at 0x0051E5A0, landed; the tail call
    // of LoadEntitiesFromScript 0x00521204). Declaration only.
    void ActivateAfterLoading(long);                                    // 0x0051E5A0
#endif
    const CMainGameComponent& Component;
    const CGameDefinitionManager& DefinitionManager;
    CWorld& World;
    CWorldMap& WorldMap;
    unsigned long* WorldSeed;
    CPlayerManager& PlayerManager;
    CNavigatorManager& NavigationManager;
    const CEnvironment* PEnvironment;
    std::vector<CThingTypeInfo> ThingTypeInfo;                          // +3c
    unsigned long DeadThingList[3]; // std::vector<CThing*> +48
    bool NonScriptedEntitesPauseMode;
    bool AllEntitesPauseMode;
    bool FirstRenderOfFrame;
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
    std::map<unsigned __int64,CThing*> UIDToThingMap;                   // +58 (chunk 846: typed; STLport 12 bytes, same layout)
#else
    unsigned long UIDToThingMap[3]; // std::map<unsigned __int64,CThing*> +58
#endif
    unsigned __int64 CurrentUniqueID;
    unsigned __int64 CurrentLocalThingUniqueID;
    bool DrawCombatCollisionDebug;
    bool DrawAttitudeDebug;
    long MaxThingDrawDist;
    unsigned long GlobalThingLoadingBehaviour; // CThingManager::EGlobalThingLoadingBehaviour +80
    unsigned long RenderList[2]; // ThingLList +84
    std::vector<ThingLList> UpdateLists;                                // +8c
    bool UseMapSpecificUIDs;
    bool UpdateAndRenderAllFlag;
    bool SerialisingGameState;
    bool CurrentlyLoading;
    bool LoadingEntitiesFromSaveGame;
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence; // +a0
    bool ErrorFlag;
#ifdef FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
    // Original Ego_r type196dd private map; native00521a91 accesses+cc. Same three-word size.
    std::map<unsigned __int64,std::pair<CCharString,CWideString> > NPCNameMap;
#else
    unsigned long NPCNameMap[3]; // std::map<unsigned __int64,std::pair<CCharString,CWideString> > +cc
#endif
    long SerialiseVersionNumber;
    long LevelBeingLoaded;
    CCharString CurrentSectionName;
};
FABLE_STATIC_ASSERT(sizeof(CThingManager)==0xe8);
#endif
