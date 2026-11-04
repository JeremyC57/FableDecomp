// CThingManager::ClearPersistenceDataForSection(const CCharString& section) -- forward to the entity runtime persistence member
// Leaf: ?ClearPersistenceDataForSection@CThingManager@@QAEXABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00520C70, 11 bytes: `add ecx,0A0h ; jmp 0x00520390` (tail call, the argument stays in place).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ClearPersistenceDataForSection@CThingManager@@QAEXABVCCharString@@@Z and its
//   member twin ?ClearPersistenceDataForSection@CEntityRuntimePersistence@NThingManager@@QAEXABVCCharString@@@Z, the landed callee
//   0x00520390 on EntityRuntimePersistence (+0xA0, engine/CThingManager.h); the sole caller CQuestManager::ClearQuestPersistScript
//   0x004B3A0D passes the thing manager. The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?ClearPersistenceDataForSection@CEntityRuntimePersistence@NThingManager@@QAEXABVCCharString@@@Z 0x00520390.
// Header: engine/CThingManager.h (FABLE_RETAIL_CTHINGMANAGER_ACCESSORS), engine/NThingManager_CEntityRuntimePersistence.h
//   (FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_CLEAR).
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_CLEAR
#include "engine/CThingManager.h"
void CThingManager::ClearPersistenceDataForSection(const CCharString& section)
{
    EntityRuntimePersistence.ClearPersistenceDataForSection(section);
}