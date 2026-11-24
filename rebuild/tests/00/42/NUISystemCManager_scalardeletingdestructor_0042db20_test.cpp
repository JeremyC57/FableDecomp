// Behaviour test for 0x0042DB20 NUISystem::CManager::`scalar deleting destructor' (reached through the
// delete in the inline Clear, its only caller) on engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER_CLEAR_INLINE +
// FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE, as the row): Clear deletes the singleton when one is stored -- the scalar deleting
// destructor runs the protected non-virtual ~CManager once on it and frees it -- and zeroes GPUniqueInstance; with none stored nothing
// is destroyed or freed and the pointer stays 0. The constructor, ~CManager and the 45 vtable slots are stubs; operator new / delete
// are counted; a derived test class reaches the protected static.
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_CMANAGER_CLEAR_INLINE
#define FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                  // the by-value CDefPointer returns of two stubbed slots

static int g_dtors = 0, g_frees = 0; static void* g_lastDtor = 0; static void* g_lastFree = 0;
void* operator new(size_t size) { return malloc(size); }
void operator delete(void* p) { if (p) { ++g_frees; g_lastFree = p; } free(p); }
NUISystem::CManager* NUISystem::CManager::GPUniqueInstance;
NUISystem::CManager::CManager() { }
NUISystem::CManager::~CManager() { ++g_dtors; g_lastDtor = this; }
CCharString::~CCharString() { }
void NUISystem::CObservable::SetEvent(NUISystem::EEvent) { }
void NUISystem::CObservable::SetOwnedEvent(NUISystem::CObserver *, NUISystem::EEvent) { }
void NUISystem::CObservable::AddObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::AddExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::ClearObservers() { }
NUISystem::CObserver * NUISystem::CObservable::GetExclusiveObserver() { return 0; }
bool NUISystem::CObservable::IsExclusivelyObserved() { return false; }
void NUISystem::CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { }
bool NUISystem::CManager::GetMouseDisabled() { return false; }
void NUISystem::CManager::SetKeyPressed(unsigned short) { }
unsigned short NUISystem::CManager::GetKeyPressed() { return 0; }
void NUISystem::CManager::Action(CCountedPointer<NUISystem::CAction> &) { }
void NUISystem::CManager::SetWeaponsInventory(CTCInventoryWeapons *) { }
void NUISystem::CManager::SetMapInventory(CTCInventoryMap *) { }
void NUISystem::CManager::SetQuestInventory(CTCInventoryQuests *) { }
void NUISystem::CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { }
void NUISystem::CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { }
void NUISystem::CManager::SetExperienceInventory(CTCInventoryExperience *) { }
void NUISystem::CManager::SetInventory(CTCInventory *) { }
void NUISystem::CManager::SetInventoryClothing(CTCInventoryClothing *) { }
void NUISystem::CManager::SetInventoryTrade(CTCInventoryTrade *) { }
void NUISystem::CManager::AddThingActionParam(long) { }
void NUISystem::CManager::RemoveThingActionParam(long) { }
void NUISystem::CManager::SetPlayerThing(CThing *) { }
void NUISystem::CManager::AddUIActionParam(NUISystem::CActionParam *) { }
void NUISystem::CManager::RemoveUIActionParam(NUISystem::CActionParam *) { }
void NUISystem::CManager::ClearActionParams() { }
void NUISystem::CManager::SetNumberAugmentations(unsigned long) { }
unsigned long NUISystem::CManager::GetNumberAugmentations() const { return 0; }
void NUISystem::CManager::SetChange(NUISystem::CManager::EChange) { }
NUISystem::CManager::EChange NUISystem::CManager::GetChange() { return (NUISystem::CManager::EChange)0; }
void NUISystem::CManager::SetMetaLayer(long) { }
long NUISystem::CManager::GetMetaLayer() { return 0; }
EEngine2DLayer NUISystem::CManager::GetLayer(long) { return (EEngine2DLayer)0; }
void NUISystem::CManager::ClearBastardChild() { }
void NUISystem::CManager::SetRightStickForce(C2DVector &) { }
C2DVector & NUISystem::CManager::GetRightStickForce() { static C2DVector s; return s; }
void NUISystem::CManager::PlaySoundW(CCharString &, long) { }
unsigned long NUISystem::CManager::PlaySample(CCharString &, long) { return 0; }
CDefPointer<NUISystem::CUIMiscThingsDef> NUISystem::CManager::GetMiscThingsDef() { return CDefPointer<NUISystem::CUIMiscThingsDef>(); }
CDefPointer<NUISystem::CUIIconsDef> NUISystem::CManager::GetIconsDef() { return CDefPointer<NUISystem::CUIIconsDef>(); }
void NUISystem::CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *, NUISystem::CComponent *, NUISystem::CComponent *, NUISystem::EEvent) { }

class TestManager : public NUISystem::CManager {
public:
    static NUISystem::CManager* Stored() { return GPUniqueInstance; }
    static void Store(NUISystem::CManager* m) { GPUniqueInstance = m; }
};

int main()
{
    NUISystem::CManager* m = new TestManager;
    TestManager::Store(m);
    NUISystem::CManager::Clear();                                       // case: stored -> destroyed once, freed, zeroed
    bool ok = g_dtors == 1 && g_lastDtor == m && g_frees == 1 && g_lastFree == m && TestManager::Stored() == 0;
    NUISystem::CManager::Clear();                                       // case: none stored -> nothing destroyed or freed
    ok = ok && g_dtors == 1 && g_frees == 1 && TestManager::Stored() == 0;
    // the catalog PassPattern is PASS_0042db20 (TABLE_CHANGES: 0042db20_TEST PASS); both are printed
    if (ok) { printf("0042db20_TEST PASS\n"); printf("PASS_0042db20\n"); return 0; }
    printf("0042db20_TEST FAIL\n");
    return 1;
}
