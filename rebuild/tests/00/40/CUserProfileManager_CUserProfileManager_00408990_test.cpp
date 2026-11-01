// Behaviour test for CUserProfileManager::~CUserProfileManager (0x00408990): the user body unregisters the CDeviceResetCallback sub-object from the display manager, then ScoreBoardProxyIp / Password / UserName, SaveGameNames (elements forward, then storage), the pending-tutorial queue, GivenTutorialCategories, DisplayModes, the two input vectors, ProfileName, then ~CBaseClassNonCopyable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct DeviceResetCallbackModel { virtual void OnPreDeviceReset() = 0; };
static const DeviceResetCallbackModel* g_removed;
struct DisplayManagerModel { void RemoveDeviceResetCallback(const DeviceResetCallbackModel* cb) { g_removed = cb; Log('R'); } };
static DisplayManagerModel g_display;
struct SystemManagerModel { DisplayManagerModel* PDisplayManager; DisplayManagerModel* GetDisplayManager() { return PDisplayManager; } };
static SystemManagerModel g_system = { &g_display };
static SystemManagerModel* GFGetSystemManager() { return &g_system; }
struct WideStringModel { char Tag; WideStringModel() : Tag('?') { } ~WideStringModel() { Log(Tag); } };
struct VectorModel { char Tag; void* Start; VectorModel() : Start(0) { } ~VectorModel() { if (Start) { Log(Tag); std::free(Start); } } };
struct StringVectorModel {
    WideStringModel* Start; WideStringModel* Finish;
    ~StringVectorModel() { for (WideStringModel* p = Start; p != Finish; ++p) p->~WideStringModel(); if (Start) { Log('V'); std::free(Start); } }
};
struct QueueModel { ~QueueModel() { Log('Q'); } };
struct BaseClassNonCopyableModel { virtual ~BaseClassNonCopyableModel() { Log('N'); } };
struct UserProfileManagerModel : BaseClassNonCopyableModel, DeviceResetCallbackModel {
    virtual void OnPreDeviceReset() { }
    ~UserProfileManagerModel() { GFGetSystemManager()->GetDisplayManager()->RemoveDeviceResetCallback(this); }
    WideStringModel   ProfileName;
    VectorModel       AssignedInputsPassive;
    VectorModel       AssignedInputsAggressive;
    VectorModel       DisplayModes;
    VectorModel       GivenTutorialCategories;
    QueueModel        PendingTutorialCategories;
    StringVectorModel SaveGameNames;
    WideStringModel   ScoreBoardUserName;
    WideStringModel   ScoreBoardPassword;
    WideStringModel   ScoreBoardProxyIp;
};
int main()
{
    UserProfileManagerModel* m = new UserProfileManagerModel;
    const DeviceResetCallbackModel* callback = m;
    m->ProfileName.Tag = 'n';
    m->AssignedInputsPassive.Tag = '1'; m->AssignedInputsPassive.Start = std::malloc(4);
    m->AssignedInputsAggressive.Tag = '2'; m->AssignedInputsAggressive.Start = std::malloc(4);
    m->DisplayModes.Tag = 'D'; m->DisplayModes.Start = std::malloc(4);
    m->GivenTutorialCategories.Tag = 'G'; m->GivenTutorialCategories.Start = std::malloc(4);
    m->SaveGameNames.Start = (WideStringModel*)std::malloc(2 * sizeof(WideStringModel));
    m->SaveGameNames.Start[0].Tag = 'a'; m->SaveGameNames.Start[1].Tag = 'b'; m->SaveGameNames.Finish = m->SaveGameNames.Start + 2;
    m->ScoreBoardUserName.Tag = 'u'; m->ScoreBoardPassword.Tag = 'p'; m->ScoreBoardProxyIp.Tag = 'i';
    BaseClassNonCopyableModel* p = m;
    delete p;
    if (std::strcmp(g_log, "RipuabVQGD21nN") != 0 || g_removed != callback) { std::printf("00408990_TEST FAIL\n"); return 1; }
    std::printf("00408990_TEST PASS\n");
    return 0;
}