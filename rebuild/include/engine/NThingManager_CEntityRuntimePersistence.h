#ifndef FABLE_ENGINE_NTHINGMANAGER_CENTITYRUNTIMEPERSISTENCE_H
#define FABLE_ENGINE_NTHINGMANAGER_CENTITYRUNTIMEPERSISTENCE_H
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
class CPersistContext;
class CThingManager;
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
    };
    class CPersistedDeadEntity {
    public:
        unsigned __int64 UID;
        bool GamePersistent;
        unsigned long SectionNameCRC;
    };
    void SaveToGameStateBinary(CPersistContext&);
    void LoadFromGameStateBinary(CPersistContext&);
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
