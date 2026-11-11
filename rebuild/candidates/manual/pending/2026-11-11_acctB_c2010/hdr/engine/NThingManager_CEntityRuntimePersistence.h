#ifndef FABLE_ENGINE_NTHINGMANAGER_CENTITYRUNTIMEPERSISTENCE_H
#define FABLE_ENGINE_NTHINGMANAGER_CENTITYRUNTIMEPERSISTENCE_H
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
class CPersistContext;
class CThingManager;
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
class CStringParser;
#endif
namespace NThingManager {
// Original TPI0x194b2/194be/194c4; native00521620 proves all accessed
// field offsets,24byte script records and16byte dead records. Full outer40.
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {
    public:
        CCharString Script;
        CArray<unsigned char> SaveData;
        long UncompressedSize;
        bool Valid;
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
        // Opt-in (chunk 2010, row 0x005248B0): Ego_r.pdb ??0CEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@QAE@XZ, inline in
        // retail: the resize(n) temporary is Script() then SaveData zeroed, UncompressedSize = 0, Valid = false (0x0052C6A0, 0x00524AA5..0x00524ADA).
        CEntityScriptInfo() : UncompressedSize(0), Valid(false) {}
#endif
    };
    class CPersistedDeadEntity {
    public:
        unsigned __int64 UID;
        bool GamePersistent;
        unsigned long SectionNameCRC;
    };
    void SaveToGameStateBinary(CPersistContext&);
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_CLEAR
    // Opt-in (chunk 765, row 0x00520C70 CThingManager::ClearPersistenceDataForSection): Ego_r.pdb / FableWin.pdb
    // ?ClearPersistenceDataForSection@CEntityRuntimePersistence@NThingManager@@QAEXABVCCharString@@@Z, out of line at 0x00520390.
    void ClearPersistenceDataForSection(const CCharString& section);   // 0x00520390
#endif
    void LoadFromGameStateBinary(CPersistContext&);
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_DEAD
    // Retail opt-in (chunk 846, row 0x0051FD80 CThingManager::LoadSingleThing): Ego_r.pdb / FableWin.pdb
    // ?IsEntityPersistedAsDead@CEntityRuntimePersistence@NThingManager@@QBE_N_K@Z, inline in retail (`lea esi,[ebx+0B8h] ; call
    // 00527D20h` = the landed map _M_find, then `cmp eax,[esi]` against the header node at 0x005200F9). Body only: no layout change.
    bool IsEntityPersistedAsDead(unsigned __int64 uid) const { return PersistedDeadEntities.find(uid)!=PersistedDeadEntities.end(); }
#endif
    bool GetGamePersistentSaveData(long,CArray<unsigned char>&) const; // 0x00522530 (Ego_r 0x005297A0, FableWin 0x01D437D0)
    void SetGamePersistentSaveData(long,const void*,unsigned long);   // PDB: QAEXJPBXK
    const CCharString* GetLevelPersistentScript(long level) const {   // PDB QBEPBVCCharString@@J (Ego_r + FableWin); inline in 0x00522720
        if((unsigned long)level<LevelPersistentEntityScripts.size() && LevelPersistentEntityScripts[level].Valid)
            return &LevelPersistentEntityScripts[level].Script;
        return 0;
    }
    const CCharString* GetGamePersistentScript(long level) const {
        if((unsigned long)level<GamePersistentEntityScripts.size() && GamePersistentEntityScripts[level].Valid)
            return &GamePersistentEntityScripts[level].Script;
        return 0;
    }
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
    // Opt-in (chunk 2010, row 0x005248B0): Ego_r.pdb / FableWin.pdb ?Clear@CEntityRuntimePersistence@NThingManager@@QAEXXZ (retail 0x005236F0,
    // landed), ?LoadFromGameStateScript@CEntityRuntimePersistence@NThingManager@@QAEXAAVCStringParser@@@Z (0x005248B0),
    // ?AddPersistedDeadEntity@..@QAEX_K_NK@Z (0x00520330, landed) and ?SetGamePersistentScript@..@QAEXJABVCCharString@@@Z (0x005237F0,
    // landed). Declarations only: the row defines the two small same-TU bodies it expands.
    void Clear();                                                                       // 0x005236F0
    void LoadFromGameStateScript(CStringParser& parser);                                // 0x005248B0
    void AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC);   // 0x00520330
    void SetGamePersistentScript(long index, const CCharString& script);               // 0x005237F0
#endif
#ifdef FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_CLEARINSCRIPT
    // Opt-in (chunk 2003, row 0x0051F480): Ego_r.pdb ?ClearPersistenceDataForSectionInEntityScript@CEntityRuntimePersistence@NThingManager@@
    // AAEXABVCCharString@@AAVCEntityScriptInfo@12@@Z (private), called only by ClearPersistenceDataForSection 0x00520390 for every
    // valid record of both script arrays (0x005203C6 / 0x00520426). Declaration only.
private:
    void ClearPersistenceDataForSectionInEntityScript(const CCharString& section, CEntityScriptInfo& info);   // 0x0051F480
#endif
private:
    // CThingManager reads SaveFormatVersion in place (ctor 0x00523540, RuntimeLoadEntities 0x00522720); the PDB layout
    // records no access, so the friendship is a modelling assumption that changes no bytes or names.
    friend class ::CThingManager;
    CArray<CEntityScriptInfo> LevelPersistentEntityScripts;
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;
    std::map<unsigned __int64,CPersistedDeadEntity> PersistedDeadEntities;
    long SaveFormatVersion;
};
FABLE_STATIC_ASSERT(sizeof(CEntityRuntimePersistence::CEntityScriptInfo)==24);
FABLE_STATIC_ASSERT(sizeof(CEntityRuntimePersistence::CPersistedDeadEntity)==16);
FABLE_STATIC_ASSERT(sizeof(CEntityRuntimePersistence)==40);
}
#endif
