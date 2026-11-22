// Behaviour test for 0x0041E5F2 NUISystem::CManager::GetInstance on engine/NUISystem_CManager.h: the first call allocates and
// constructs the singleton (0xD0 bytes, the protected constructor) into the protected static GPUniqueInstance and returns it; later
// calls return the stored instance without constructing; an instance already stored is returned as is. The body is the row's; the
// constructor and the 45 vtable slots are stubs; a derived test class reads and sets the protected static.
#include <stdio.h>
#define FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                  // the by-value CDefPointer returns of two stubbed slots

namespace NUISystem {
CManager* CManager::GetInstance()
{
    if (GPUniqueInstance == 0)
        GPUniqueInstance = new CManager;
    return GPUniqueInstance;
}
}

static int g_ctors = 0;
NUISystem::CManager* NUISystem::CManager::GPUniqueInstance;
NUISystem::CManager::CManager() { ++g_ctors; }
NUISystem::CManager::~CManager() { }
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
    bool ok = sizeof(NUISystem::CManager) == 0xd0 && TestManager::Stored() == 0;
    NUISystem::CManager* a = NUISystem::CManager::GetInstance();       // case: none yet -> created
    ok = ok && a != 0 && g_ctors == 1 && TestManager::Stored() == a;
    NUISystem::CManager* b = NUISystem::CManager::GetInstance();       // case: stored -> returned, not constructed again
    ok = ok && b == a && g_ctors == 1;
    TestManager other;                                                  // (its construction runs the stub constructor once)
    TestManager::Store(&other);                                         // case: another instance stored -> that one returned
    ok = ok && NUISystem::CManager::GetInstance() == &other && g_ctors == 2;
    TestManager::Store(0);
    printf(ok ? "0041e5f2_TEST PASS\n" : "0041e5f2_TEST FAIL\n");
    return ok ? 0 : 1;
}
