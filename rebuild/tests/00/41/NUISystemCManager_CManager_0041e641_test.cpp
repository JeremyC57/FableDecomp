// Behaviour test for 0x0041E641 NUISystem::CManager::~CManager (protected, non-virtual) on engine/NUISystem_CManager.h: the error
// message is cleared only while one is showing; the singleton gets an empty mesh bank, then an empty graphics bank (each argument built
// before GetInstance is called); then the members are destroyed (+0x88 down to +0x10: the counted banks and BastardChild release their
// count blocks) and the CObservable base last. The destructor body is the row's; a derived test class reaches the protected members;
// the out-of-line members it links against are stubs below (the 45 vtable slots, ClearErrorMessage, GetInstance, SetMeshBank,
// SetGraphicsBank, the constructor, ~CCharString).
#include <stdio.h>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CMANAGER_ERRORMESSAGE
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                  // the by-value CDefPointer returns of two stubbed slots

namespace NUISystem {
CManager::~CManager()
{
    if (m_bHasErrorMessage)
        ClearErrorMessage();
    GetInstance()->SetMeshBank(CCountedPointer<CMeshDataBank>());
    GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());
}
}

static char g_log[32]; static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }
static NUISystem::CManager* g_instance = 0;
static NUISystem::CManager* g_meshThis = 0; static NUISystem::CManager* g_graphicsThis = 0;
static bool g_meshEmpty = false, g_graphicsEmpty = false;

// stubs: the out-of-line members this TU links against
NUISystem::CManager::CManager() { }
void NUISystem::CManager::ClearErrorMessage() { Log('C'); }
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { Log('I'); return g_instance; }
void NUISystem::CManager::SetMeshBank(CCountedPointer<CMeshDataBank> mesh_bank)
{ Log('M'); g_meshThis = this; g_meshEmpty = mesh_bank.Data == 0 && mesh_bank.Info == 0; }
void NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank)
{ Log('G'); g_graphicsThis = this; g_graphicsEmpty = graphics_bank.Data == 0 && graphics_bank.Info == 0; }
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

static void __fastcall DeleteMesh(void*) { Log('m'); }
static void __fastcall DeleteGraphics(void*) { Log('g'); }
static void __fastcall DeleteChild(void*) { Log('c'); }

class TestManager : public NUISystem::CManager {
public:
    void SetShowing(bool showing) { m_bHasErrorMessage = showing; }
    void HoldBanks(CCPPointerInfo* mesh, CCPPointerInfo* graphics, CCPPointerInfo* child)
    {
        static int object;
        PMeshBank.Data = reinterpret_cast<CMeshDataBank*>(&object); PMeshBank.Info = mesh;
        PGraphicsBank.Data = reinterpret_cast<CGraphicDataBank*>(&object); PGraphicsBank.Info = graphics;
        BastardChild.Data = reinterpret_cast<NUISystem::CComponent*>(&object); BastardChild.Info = child;
    }
};

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("0041e641 FAIL %s (log %s)\n", what, g_log); g_fail = 1; } }
int main()
{
    TestManager singleton;
    g_instance = &singleton;
    // case 1: an error message is showing -> cleared first; both banks handed over empty; members' count blocks released
    {
        TestManager* m = new TestManager;
        m->SetShowing(true);
        CCPPointerInfo* mesh = new CCPPointerInfo(1, DeleteMesh, 0);
        CCPPointerInfo* graphics = new CCPPointerInfo(1, DeleteGraphics, 0);
        CCPPointerInfo child(2, DeleteChild, 0);       // still referenced elsewhere: not deleted
        m->HoldBanks(mesh, graphics, &child);
        g_n = 0;
        delete m;
        Check(g_n == 7 && g_log[0] == 'C' && g_log[1] == 'I' && g_log[2] == 'M' && g_log[3] == 'I' && g_log[4] == 'G', "showing: order");
        Check(g_log[5] == 'm' && g_log[6] == 'g', "showing: mesh bank (+0x18) released before graphics bank (+0x10)");
        Check(g_meshThis == &singleton && g_graphicsThis == &singleton && g_meshEmpty && g_graphicsEmpty, "showing: empty banks to the singleton");
        Check(child.RefCount == 1, "showing: BastardChild reference dropped");
    }
    // case 2: no error message -> not cleared; empty members release nothing
    {
        TestManager* m = new TestManager;
        m->SetShowing(false);
        g_n = 0; g_meshEmpty = g_graphicsEmpty = false;
        delete m;
        Check(g_n == 4 && g_log[0] == 'I' && g_log[1] == 'M' && g_log[2] == 'I' && g_log[3] == 'G', "not showing: order");
        Check(g_meshEmpty && g_graphicsEmpty, "not showing: empty banks");
    }
    g_instance = 0;
    if (g_fail) return 1;
    printf("0041e641_TEST PASS\n");
    return 0;
}
