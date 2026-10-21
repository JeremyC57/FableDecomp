// Behaviour test for 0093e8c0: src/0093e8c0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_VillageTaskTorch::IsRunnable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_VillageTaskTorch::IsRunnable() -- clear the task, ask the creature's village for a torch task; keep it only when the task's torch exists and its graphic appearance has the torch dummy object (else clear it again); runnable when a task remains
// Leaf: ?IsRunnable@CAIStateGroup_VillageTaskTorch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E8C0, length 318 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (PTask
//          assignments through CIntelligentPointer<CVillageTaskTorch> temporaries, vtable 0x0128B088), GFGetPVillageInterface
//          0x007EE540, CTCVillage torch-task query 0x006590C4 (forwards to 0x00660034 on the village's +0x4c member),
//          CTCGraphicAppearance::GetDummyObject 0x004C08C0 (name = the global CCharString 0x013BACA0).
// Layout: Ego_r.pdb CAIStateGroup_VillageTaskTorch: PTCVillageMember +0x14, PTask +0x18; the task's torch pointer +0x08;
//          the thing's graphic appearance component cached at +0x64.
// Shape: a named appearance local, an uninitialised CRightHandedSet and a GetCreature() accessor; the task is read into a
// named local before reaching its torch pointer (`lea ecx,[eax+8]`), and the pointer assignment reads the source item into
// a local before SetPItem (so `&PTask` is formed after the GetPItem call, as retail).
struct CCharString { void* PStringData; };
extern CCharString g_TorchDummyName;            // 0x013BACA0 (dynamically initialised global name)
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
};
struct CRightHandedSet { C3DVector First; C3DVector Second; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char _pad[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {
    CThing* GetThing() const { return (CThing*)GetPItem(); }
};
struct CVillageTaskTorch {
    unsigned char _pad[8];
    CBaseIntelligentPointer PTorch;             // +0x08 CIntelligentPointer<CThing>
};
struct CTaskPointer : CBaseIntelligentPointer { // CIntelligentPointer<CVillageTaskTorch>, vtable 0x0128B088
    virtual void v00();
    CTaskPointer(CVillageTaskTorch* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CTaskPointer& operator=(const CTaskPointer& other) { void* item = other.GetPItem(); SetPItem(item); return *this; }
    CVillageTaskTorch* GetTask() const { return (CVillageTaskTorch*)GetPItem(); }
};
struct CThingAICreature;
struct CTCVillageMember;
struct CTCVillage {
    CVillageTaskTorch* GetTorchTask(CThingAICreature* creature);   // 0x006590C4
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillageMember* member);   // 0x007EE540
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};
struct CAIStateGroup_VillageTaskTorch : CAIStateGroupBase {
    CTCVillageMember* PTCVillageMember;         // +0x14
    CTaskPointer      PTask;                    // +0x18
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    virtual bool IsRunnable();
};

bool CAIStateGroup_VillageTaskTorch::IsRunnable()
{
    PTask = 0;
    CTCVillage* village = GFGetPVillageInterface(PTCVillageMember);
    if (village) {
        PTask = village->GetTorchTask(GetCreature());
        if (PTask.GetTask()) {
            CVillageTaskTorch* task = PTask.GetTask();
            CThing* torch = (CThing*)task->PTorch.GetPItem();
            if (torch) {
                CTCGraphicAppearance* appearance = torch->PTCGraphicAppearance;
                C3DVector position(0, 0, 0);
                CRightHandedSet orientation;
                if (!appearance->GetDummyObject(g_TorchDummyName, 0, 0, &position, &orientation, 1.0f))
                    PTask = 0;
            }
        }
    }
    return PTask.GetTask() != 0;
}

// ---- stubs and driver ----
#include <cstdlib>
#include <new>
static int g_dummyCalls = 0, g_taskQueries = 0;
static bool g_hasDummy = false;
static CTCVillage g_village;
static bool g_member = true;
static CVillageTaskTorch* g_task = 0;
static CThingAICreature* g_lastCreature = 0;
CCharString g_TorchDummyName;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
void* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(void* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() {}
void CTaskPointer::v00() {}
CAIStateGroupBase::~CAIStateGroupBase() {}
CVillageTaskTorch* CTCVillage::GetTorchTask(CThingAICreature* creature) { ++g_taskQueries; g_lastCreature = creature; return g_task; }
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillageMember* member) { return member ? &g_village : 0; }
bool CTCGraphicAppearance::GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale)
{
    ++g_dummyCalls;
    if (&name != &g_TorchDummyName || zero1 != 0 || zero2 != 0 || scale != 1.0f) return false;
    if (position->x != 0.0f || position->y != 0.0f || position->z != 0.0f || !orientation) return false;
    return g_hasDummy;
}
int main()
{
    CAIStateGroup_VillageTaskTorch* group = (CAIStateGroup_VillageTaskTorch*)calloc(1, sizeof(CAIStateGroup_VillageTaskTorch));
    new (&group->PTask) CTaskPointer(0);
    CAIBrain brain; memset(&brain, 0, sizeof(brain)); brain.PCreature = (CThingAICreature*)0x5150;
    group->PBrain = &brain;
    CVillageTaskTorch* task = (CVillageTaskTorch*)calloc(1, sizeof(CVillageTaskTorch));
    new (&task->PTorch) CBaseIntelligentPointer();
    CThing* torch = (CThing*)calloc(1, sizeof(CThing));
    CTCGraphicAppearance appearance; torch->PTCGraphicAppearance = &appearance;
    // no village member: task cleared, not runnable
    group->PTask.SetPItem(task); group->PTCVillageMember = 0;
    CHECK(!group->CAIStateGroup_VillageTaskTorch::IsRunnable() && group->PTask.GetPItem() == 0 && g_taskQueries == 0);
    group->PTCVillageMember = (CTCVillageMember*)0x10;
    // village gives no task
    g_task = 0;
    CHECK(!group->CAIStateGroup_VillageTaskTorch::IsRunnable() && g_taskQueries == 1 && g_lastCreature == (CThingAICreature*)0x5150);
    // task without a torch: kept, runnable, no dummy query
    g_task = task;
    CHECK(group->CAIStateGroup_VillageTaskTorch::IsRunnable() && group->PTask.GetPItem() == task && g_dummyCalls == 0);
    // torch without the dummy object: cleared
    task->PTorch.SetPItem(torch); g_hasDummy = false;
    CHECK(!group->CAIStateGroup_VillageTaskTorch::IsRunnable() && group->PTask.GetPItem() == 0 && g_dummyCalls == 1);
    // torch with the dummy object: kept
    g_hasDummy = true;
    CHECK(group->CAIStateGroup_VillageTaskTorch::IsRunnable() && group->PTask.GetPItem() == task && g_dummyCalls == 2);
    if (fails == 0) printf("0093e8c0_TEST PASS\n");
    return fails;
}
