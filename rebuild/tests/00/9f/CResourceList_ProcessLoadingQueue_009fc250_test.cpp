#include <stdio.h>
#include <string.h>
// CResourceList::ProcessLoadingQueue(bool limitTime, double endTime) -- walk the loading queue: abort the loads that may be aborted (when aborting is enabled), complete the ones that finished, restarting from the head after each removal; stop early once the time budget is spent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC250, length 260 bytes (`ret 0Ch`).
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double on the FPU); CResource virtual slots 4 IsResourceLoaded (+0x10),
// 5 OnResourceLoaded (+0x14), 6 CanAbortResourceLoad(long unusedFrames) const (+0x18, Ego_r.pdb ?CanAbortResourceLoad@CResource@@UBE_NJ@Z), 7 OnResourceLoadAborted (+0x1c)
// (vtable 0x012354A4: 0x0042D950/60/70/80, labels from the manifest). Global: the abort-loads enable byte 0x01396C5C
// (descriptive name).
// Layout: engine/CResourceList.h (Head CResource +0x04, ResourceCount +0x20, AllocatedMemory +0x24, CurrentFrame +0x2c);
// CResource as the landed copy ctor 0x0046649A (ResourceList +8, PrevResource +0xc, NextResource +0x10, ResourceSize +0x14,
// LastUsedFrame +0x18).
// Shape: the abort argument is `GetCurrentFrame() - resource->GetLastUsedFrame()` through two inline const accessors
// (the plain member subtraction loads CurrentFrame first and rotates every later scratch register); the removal is an inline member (`ResourceList = 0`, the inline CResource::ListRemove of 0x009FBFB0 -- its two zero
// stores share the hoisted ebx zero -- then the count/memory bookkeeping), expanded once per arm; the successor is read
// into ebp before the tests; `GFGetTime() > endTime` gives `fcomp qword ; test ah,41h ; je exit`.
#include "engine/CResourceList.h"

double __fastcall GFGetTime();                          // 0x009E1BC0
extern bool g_bAbortResourceLoads;                      // 0x01396C5C

class CResource { public:                                      // vtable 0x012354A4, 0x1C bytes
    virtual ~CResource();
    virtual void v01();
    virtual void UnloadResource();                      // slot 2
    virtual unsigned long GetUnloadableMemory();        // slot 3
    virtual bool IsResourceLoaded();                    // slot 4 (+0x10)
    virtual void OnResourceLoaded();                    // slot 5 (+0x14)
    virtual bool CanAbortResourceLoad(long unusedFrames) const;   // slot 6 (+0x18)
    virtual void OnResourceLoadAborted();               // slot 7 (+0x1c)
    long           IVRefCount;                          // +0x04 (CIVCountedPointeeBase)
    CResourceList* ResourceList;                        // +0x08
    CResource*     PrevResource;                        // +0x0C
    CResource*     NextResource;                        // +0x10
    unsigned long  ResourceSize;                        // +0x14
    unsigned long  LastUsedFrame;                       // +0x18
    unsigned long GetLastUsedFrame() const { return LastUsedFrame; }
    void ListRemove()                                   // 0x009FBFB0 out of line elsewhere
    {
        if (NextResource) {
            if (PrevResource->NextResource == this && NextResource->PrevResource == this) {
                PrevResource->NextResource = NextResource;
                NextResource->PrevResource = PrevResource;
            }
            PrevResource = 0;
            NextResource = 0;
        }
    }
};

struct CResourceList_Queue : CResourceList {
    CResource* GetHead() { return reinterpret_cast<CResource*>(Head); }
    unsigned long GetCurrentFrame() const { return CurrentFrame; }
    void RemoveResource(CResource* resource)
    {
        resource->ResourceList = 0;
        resource->ListRemove();
        --ResourceCount;
        AllocatedMemory -= resource->ResourceSize;
    }
    void ProcessLoadingQueue(bool limitTime, double endTime);
};

void CResourceList_Queue::ProcessLoadingQueue(bool limitTime, double endTime)
{
    CResource* resource = GetHead()->NextResource;
    while (resource != GetHead()) {
        if (!resource)
            break;
        if (limitTime && GFGetTime() > endTime)
            break;
        CResource* next = resource->NextResource;
        if (g_bAbortResourceLoads && resource->CanAbortResourceLoad(GetCurrentFrame() - resource->GetLastUsedFrame())) {
            RemoveResource(resource);
            resource->OnResourceLoadAborted();
            resource = GetHead()->NextResource;
        }
        else if (resource->IsResourceLoaded()) {
            RemoveResource(resource);
            resource->OnResourceLoaded();
            resource = GetHead()->NextResource;
        }
        else
            resource = next;
    }
}

// ---- test stubs ----
static double g_time, g_timeStep;
double __fastcall GFGetTime() { double t = g_time; g_time += g_timeStep; return t; }
bool g_bAbortResourceLoads;
CResource::~CResource() { }
void CResource::v01() { }
void CResource::UnloadResource() { }
unsigned long CResource::GetUnloadableMemory() { return 0; }
bool CResource::IsResourceLoaded() { return false; }
void CResource::OnResourceLoaded() { }
bool CResource::CanAbortResourceLoad(long) const { return false; }
void CResource::OnResourceLoadAborted() { }
static char g_log[256];
static void logc(char c, int id) { size_t n = strlen(g_log); g_log[n] = c; g_log[n + 1] = (char)('0' + id); g_log[n + 2] = 0; }
struct TR : CResource {
    int id, loaded, abortMin; long lastArg;
    bool IsResourceLoaded() { logc('?', id); return loaded != 0; }
    void OnResourceLoaded() { logc('L', id); }
    bool CanAbortResourceLoad(long f) const { ((TR*)this)->lastArg = f; logc('a', id); return abortMin > 0 && f >= abortMin; }
    void OnResourceLoadAborted() { logc('A', id); }
};
static CResourceList_Queue q;
static TR r[4];
static int fails;
static void check(int ok, const char* what) { if (!ok) { fails++; printf("FAIL %s log=%s\n", what, g_log); } }
static void build(int n)
{
    memset(&q, 0, sizeof q);
    CResource* head = q.GetHead();
    CResource* prev = head;
    for (int i = 0; i < n; i++) {
        r[i].id = i + 1; r[i].loaded = 0; r[i].abortMin = 0; r[i].lastArg = -1;
        r[i].ResourceList = &q; r[i].ResourceSize = 10 * (i + 1); r[i].LastUsedFrame = 0;
        r[i].PrevResource = prev; prev->NextResource = &r[i]; prev = &r[i];
    }
    prev->NextResource = head; head->PrevResource = prev;
    q.ResourceCount = n; q.AllocatedMemory = 1000; q.CurrentFrame = 100;
    g_time = 0; g_timeStep = 0; g_bAbortResourceLoads = false; g_log[0] = 0;
}
int main()
{
    // 1: empty queue
    build(0); q.ProcessLoadingQueue(false, 0.0); check(g_log[0] == 0 && q.ResourceCount == 0, "empty");
    // 2: loaded / not loaded / loaded, aborting off: restart from the head after each removal
    build(3); r[0].loaded = 1; r[2].loaded = 1;
    q.ProcessLoadingQueue(false, 0.0);
    check(strcmp(g_log, "?1L1?2?3L3?2") == 0, "order");
    check(q.ResourceCount == 1 && q.AllocatedMemory == 1000 - 10 - 30, "counts");
    check(r[0].ResourceList == 0 && r[0].NextResource == 0 && r[0].PrevResource == 0, "unlinked");
    check(q.GetHead()->NextResource == &r[1] && r[1].NextResource == q.GetHead() && q.GetHead()->PrevResource == &r[1], "relinked");
    // 3: aborting on: frames unused = CurrentFrame - LastUsedFrame
    build(2); g_bAbortResourceLoads = true; r[0].LastUsedFrame = 85; r[0].abortMin = 10; r[1].LastUsedFrame = 99; r[1].abortMin = 10; r[1].loaded = 1;
    q.ProcessLoadingQueue(false, 0.0);
    check(strcmp(g_log, "a1A1a2?2L2") == 0, "abort");
    check(r[0].lastArg == 15 && r[1].lastArg == 1 && q.ResourceCount == 0 && q.AllocatedMemory == 1000 - 30, "abortargs");
    // 4: time budget already spent
    build(2); r[0].loaded = 1; g_time = 5.0; q.ProcessLoadingQueue(true, 4.0); check(g_log[0] == 0, "timeout");
    // 4b: time limit ignored when limitTime is false
    build(1); r[0].loaded = 1; g_time = 5.0; q.ProcessLoadingQueue(false, 4.0); check(strcmp(g_log, "?1L1") == 0, "nolimit");
    // 5: time runs out after two checks (equal time is not over)
    build(3); g_time = 3.0; g_timeStep = 1.0; q.ProcessLoadingQueue(true, 4.0); check(strcmp(g_log, "?1?2") == 0, "budget");
    // 6: a null successor ends the walk
    build(2); r[0].NextResource = 0; q.ProcessLoadingQueue(false, 0.0); check(strcmp(g_log, "?1") == 0, "nullnext");
    if (!fails) printf("009fc250_TEST PASS\n");
    return fails;
}