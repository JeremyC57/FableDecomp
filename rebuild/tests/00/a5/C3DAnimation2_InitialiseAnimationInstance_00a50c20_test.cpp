// Behaviour test for 0x00A50C20 C3DAnimation2::InitialiseAnimationInstance: the row's defines, includes and body verbatim, over zeroed
// object images (retail offsets: AnimType +8, Duration +0xC, LoopFlag +0x10, MovementFlag +0x11, MovementVector +0x12, TimingEvents +0x20,
// CMovableResource vptr +0x2C, Allocator +0x30, MemoryPool +0x38, MemoryHandle +0x3C, MovementTrack +0x40, SamplesPerSecond +0x50,
// FrameCount +0x54, SequenceCount +0x58, LockedFromMemMemoryRelocation +0x74, InstanceList +0x78) with doubles for the out-of-line
// callees. Checked: the stats / frame count / rate / movement track copied from the shared animation (the old track released), the
// instance registered in the shared animation's list, the call order of both resource arms (movable pool: SetMemoryPool on the scratch
// buffer, InitialiseAnimationSequences, CalculateMemoryAllocatedByClient, AllocateResource, GetPointerFromHandle, memcpy, RelocateData,
// SetMemoryPool on the block; allocator: Reset, SetCapacity, Allocate, memcpy, RelocateData with handle 0), the scratch buffer
// unlocked on every exit, the relocation lock set during the build and cleared on both returns, a failed AllocateResource returning
// false without touching out_match, and out_match = matches / max(SequenceCount, 1) (and skipped when null).
#include <stdio.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
#define FABLE_RETAIL_C3DANIMATIONSTATS_TIMINGEVENTS
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_C3DANIMATION2_INSTANCELIST
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_C3DANIMATION2_INITSEQUENCES
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_RESET
#include "fable_compiler.h"
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DAnimationStats.h"
#include "engine/C3DAnimation2.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CScopedLockFlagBool.h"
class CScratchBuffer {
public:
    virtual ~CScratchBuffer();
    bool Lock(void** buffer);                                // 0x009F4980
    void Unlock();                                           // 0x009F49A0
    long GetByteSize() const { return NoBytes; }
private:
    long           _pad04;
    unsigned char* Buffer;                                   // +0x08
    bool           Locked;                                   // +0x0c
    long           NoBytes;                                  // +0x10
};
extern CScratchBuffer g_ScratchBuffer;                       // 0x013CB498
bool C3DAnimation2::InitialiseAnimationInstance(C3DAnimation2& shared_anim, const C3DMesh2& mesh, bool fit_to_mesh, float* out_match)
{
    CScopedLockFlagBool lock(LockedFromMemMemoryRelocation);
    FrameCount = shared_anim.FrameCount;
    SamplesPerSecond = shared_anim.SamplesPerSecond;
    LoopFlag = shared_anim.LoopFlag;
    Duration = shared_anim.Duration;
    MovementFlag = shared_anim.MovementFlag;
    MovementVector = shared_anim.MovementVector;
    TimingEvents = shared_anim.TimingEvents;
    AnimType = shared_anim.AnimType;
    shared_anim.InstanceList.push_back(CIntelligentPointer<C3DAnimation2>(this));
    {
        void* temp_buffer = 0;
        g_ScratchBuffer.Lock(&temp_buffer);
        unsigned long buffer_size = g_ScratchBuffer.GetByteSize();
        Allocator->SetMemoryPool(temp_buffer, buffer_size);
        unsigned long matches = 0;
        InitialiseAnimationSequences(shared_anim, mesh, fit_to_mesh, matches);
        MovementTrack = shared_anim.MovementTrack;
        unsigned long temp_size = Allocator->CalculateMemoryAllocatedByClient();
        if (MemoryPool) {
            if (!MemoryPool->AllocateResource(&MemoryHandle, this, temp_size, 0)) {
                g_ScratchBuffer.Unlock();
                return false;
            }
            void* dest_buffer = MemoryPool->GetPointerFromHandle(MemoryHandle);
            memcpy(dest_buffer, temp_buffer, temp_size);
            RelocateData(MemoryHandle, temp_buffer, dest_buffer, temp_size);
            Allocator->SetMemoryPool(dest_buffer, temp_size);
        } else {
            Allocator->Reset();
            Allocator->SetCapacity(temp_size);
            void* dest_buffer = Allocator->Allocate(temp_size);
            memcpy(dest_buffer, temp_buffer, temp_size);
            RelocateData(0, temp_buffer, dest_buffer, temp_size);
        }
        g_ScratchBuffer.Unlock();
        if (out_match)
            *out_match = (float)matches / (float)std::max(SequenceCount, 1UL);
        return true;
    }
}

#include <new>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
template<class T> static T& F(void* object, int offset) { return *(T*)((char*)object + offset); }
static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
static void ResetLog() { g_logn = 0; g_log[0] = 0; }

// CScratchBuffer doubles: Lock hands out g_scratch (and sets the byte size GetByteSize reads inline).
static unsigned char g_scratch[64]; static int g_locked = 0;
CScratchBuffer::~CScratchBuffer() { }
bool CScratchBuffer::Lock(void** buffer) { Log('L'); Buffer = g_scratch; NoBytes = sizeof(g_scratch); Locked = true; g_locked = 1; *buffer = Buffer; return true; }
void CScratchBuffer::Unlock() { Log('U'); Locked = false; g_locked = 0; }
CScratchBuffer g_ScratchBuffer;

// CSimpleMemoryPool doubles.
static void* g_setMem[2]; static unsigned long g_setSize[2]; static int g_setN = 0;
static unsigned long g_clientSize = 0, g_capacity = 0; static unsigned char g_allocDest[64];
void CSimpleMemoryPool::SetMemoryPool(void* memory, unsigned long size) { Log('S'); if (g_setN < 2) { g_setMem[g_setN] = memory; g_setSize[g_setN] = size; } ++g_setN; }
unsigned long CSimpleMemoryPool::CalculateMemoryAllocatedByClient() const { Log('C'); return g_clientSize; }
void CSimpleMemoryPool::Reset() { Log('X'); }
void CSimpleMemoryPool::SetCapacity(unsigned long capacity) { Log('K'); g_capacity = capacity; }
void* CSimpleMemoryPool::Allocate(unsigned long size) { Log('M'); check(size == g_clientSize, "Allocate size"); return g_allocDest; }

// CMovableResourceMemoryPool / CHandleBasedMemoryPool doubles.
static bool g_allocOk = true; static CMovableResource* g_allocRes = 0; static unsigned long g_allocSize = 0; static unsigned char g_poolDest[64];
bool CMovableResourceMemoryPool::AllocateResource(long* handle, CMovableResource* resource, unsigned long size, unsigned long align)
{ Log('A'); g_allocRes = resource; g_allocSize = size; check(align == 0, "AllocateResource alignment 0"); if (!g_allocOk) return false; *handle = 77; return true; }
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle) { Log('P'); check(handle == 77, "GetPointerFromHandle handle"); return g_poolDest; }

// CBaseIntelligentPointer doubles: PData holds the item itself.
static int g_ipLive = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++g_ipLive; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_ipLive; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = (CBaseObjectPointer*)item; }

// InitialiseAnimationSequences double: checks the copy before it and the lock, fills the scratch buffer, reports g_matches.
static unsigned long g_matches = 0; static const void* g_meshSeen = 0; static bool g_fitSeen = false; static int g_lockedDuringBuild = 0;
static C3DAnimation2* g_sharedSeen = 0;
bool C3DAnimation2::InitialiseAnimationSequences(C3DAnimation2& shared_animation, const C3DMesh2& mesh, bool create_rigid_bones, unsigned long& build_result)
{
    Log('I');
    g_sharedSeen = &shared_animation; g_meshSeen = &mesh; g_fitSeen = create_rigid_bones;
    g_lockedDuringBuild = LockedFromMemMemoryRelocation && g_locked;
    check(build_result == 0, "matches starts at 0");
    check(FrameCount == shared_animation.FrameCount && SamplesPerSecond == shared_animation.SamplesPerSecond, "frames copied before the build");
    for (int i = 0; i < 16; ++i) g_scratch[i] = (unsigned char)(0xA0 + i);
    build_result = g_matches;
    return true;
}

// RelocateData: the CMovableResource subobject's slot 1, reached through a stand-in vtable.
static long g_relHandle = -1; static void* g_relSrc = 0; static void* g_relDst = 0; static long g_relSize = 0; static void* g_relThis = 0;
struct FakeMovable { virtual ~FakeMovable() { } virtual void RelocateData(long handle, void* src, void* dst, long size)
    { Log('R'); g_relThis = this; g_relHandle = handle; g_relSrc = src; g_relDst = dst; g_relSize = size; } };

static int g_deleted = 0; static void* g_deletedData = 0;
static void FABLE_FASTCALL RecordDelete(void* data) { ++g_deleted; g_deletedData = data; }

// The TimingEvents copy: retail calls the tree's operator= out of line (0x00A54A90); the double records the pair it was given.
typedef std::pair<const CCharString, float> TTimingEvent;
typedef std::_Rb_tree<CCharString, TTimingEvent, std::_Select1st<TTimingEvent>, std::less<CCharString>, std::allocator<TTimingEvent> > TTimingTree;
static const void* g_treeTo = 0; static const void* g_treeFrom = 0; static int g_treeCopies = 0;
template<> TTimingTree& TTimingTree::operator=(const TTimingTree& other) { g_treeTo = this; g_treeFrom = &other; ++g_treeCopies; Log('T');
    check(F<bool>((void*)this, 0x74 - 0x20), "relocation lock taken before the copies"); return *this; }

typedef std::list<CIntelligentPointer<C3DAnimation2> > TInstanceList;
typedef std::multimap<CCharString, float> TTimingEvents;
static double g_animImg[16], g_sharedImg[16];
static unsigned char g_allocatorImg[0x1c], g_poolImg[0x464], g_meshImg[16];

// The lane STLport shim exposes few accessors here: read the retail layouts (list: sentinel node pointer, nodes next / prev / value;
// tree: node count at +4).
static int ListSize(void* list) { char* head = F<char*>(list, 0); int n = 0; for (char* p = F<char*>(head, 0); p != head; p = F<char*>(p, 0)) ++n; return n; }
static C3DAnimation2* ListFront(void* list) { char* head = F<char*>(list, 0); return ((CIntelligentPointer<C3DAnimation2>*)(F<char*>(head, 0) + 8))->Get(); }
static unsigned long TreeSize(void* tree) { return F<unsigned long>(tree, 4); }

int main()
{
    void* anim = g_animImg; void* shared = g_sharedImg;
    check(sizeof(C3DAnimation2) == 0x7c, "C3DAnimation2 size");
    C3DAnimation2* a = (C3DAnimation2*)anim; C3DAnimation2* s = (C3DAnimation2*)shared;
    check((char*)static_cast<CMovableResource*>(a) - (char*)a == 0x2c, "CMovableResource at +0x2C");
    new ((char*)shared + 0x78) TInstanceList();
    new ((char*)shared + 0x20) TTimingEvents();
    new ((char*)anim + 0x20) TTimingEvents();
    FakeMovable fake; memcpy((char*)anim + 0x2c, &fake, 4);
    TInstanceList& instances = F<TInstanceList>(shared, 0x78);
    TTimingEvents& sharedEvents = F<TTimingEvents>(shared, 0x20);
    TTimingEvents& animEvents = F<TTimingEvents>(anim, 0x20);
    check(TreeSize(&sharedEvents) == 0 && ListSize(&instances) == 0, "fresh containers");

    // shared animation's data
    F<long>(shared, 8) = 3; F<float>(shared, 0xc) = 2.25f; F<bool>(shared, 0x10) = true; F<bool>(shared, 0x11) = true;
    F<float>(shared, 0x12) = 1.0f; F<float>(shared, 0x16) = -2.0f; F<float>(shared, 0x1a) = 4.5f;
    F<float>(shared, 0x50) = 30.0f; F<unsigned long>(shared, 0x54) = 120;
    CCPPointerInfo* sharedTrackInfo = new CCPPointerInfo(1, RecordDelete, (void*)0x1234);
    F<void*>(shared, 0x40) = (void*)0x1234; F<CCPPointerInfo*>(shared, 0x44) = sharedTrackInfo;
    // the instance: an old movement track to release, the allocator, sequence count
    F<void*>(anim, 0x40) = (void*)0x99; F<CCPPointerInfo*>(anim, 0x44) = new CCPPointerInfo(1, RecordDelete, (void*)0x99);
    F<void*>(anim, 0x30) = g_allocatorImg;
    F<unsigned long>(anim, 0x58) = 4;

    // 1) movable pool arm
    F<void*>(anim, 0x38) = g_poolImg;
    g_matches = 3; g_clientSize = 20; g_allocOk = true; ResetLog(); g_setN = 0;
    float match = -1.0f;
    bool ok = a->InitialiseAnimationInstance(*s, *(const C3DMesh2*)g_meshImg, true, &match);
    check(ok, "pool arm returns true");
    check(strcmp(g_log, "TLSICAPRSU") == 0, "pool arm call order");
    if (strcmp(g_log, "TLSICAPRSU") != 0) printf("  log %s\n", g_log);
    check(a->GetFrameCount() == 120 && a->GetSamplesPerSecond() == 30.0f, "frame count / rate copied");
    check(F<long>(anim, 8) == 3 && F<float>(anim, 0xc) == 2.25f && F<bool>(anim, 0x10) && F<bool>(anim, 0x11), "stats copied");
    C3DVector mv = a->GetMovementVector();
    check(mv.X == 1.0f && mv.Y == -2.0f && mv.Z == 4.5f, "movement vector copied");
    check(g_treeCopies == 1 && g_treeTo == &animEvents && g_treeFrom == &sharedEvents, "timing events assigned from the shared ones");
    check(F<void*>(anim, 0x40) == (void*)0x1234 && F<CCPPointerInfo*>(anim, 0x44) == sharedTrackInfo && sharedTrackInfo->RefCount == 2, "movement track shared");
    check(g_deleted == 1 && g_deletedData == (void*)0x99, "old movement track released");
    check(ListSize(&instances) == 1 && ListFront(&instances) == a, "instance registered in the shared list");
    check(g_ipLive == 1, "temporary intelligent pointer destroyed, node kept");
    check(g_sharedSeen == s && g_meshSeen == g_meshImg && g_fitSeen == true && g_lockedDuringBuild, "build arguments, lock held");
    check(g_setMem[0] == g_scratch && g_setSize[0] == sizeof(g_scratch), "allocator pointed at the scratch buffer");
    check(g_allocRes == static_cast<CMovableResource*>(a) && g_allocSize == 20, "AllocateResource resource / size");
    check(F<long>(anim, 0x3c) == 77, "memory handle stored");
    check(memcmp(g_poolDest, g_scratch, 16) == 0, "sequences copied into the block");
    check(g_relThis == (char*)anim + 0x2c && g_relHandle == 77 && g_relSrc == g_scratch && g_relDst == g_poolDest && g_relSize == 20, "RelocateData(handle, temp, dest, size)");
    check(g_setMem[1] == g_poolDest && g_setSize[1] == 20, "allocator moved onto the block");
    check(match == 0.75f, "out_match = 3 / 4");
    check(!F<bool>(anim, 0x74) && !g_locked, "lock cleared, scratch unlocked");

    // 2) allocator arm, SequenceCount 0 -> divide by 1
    F<void*>(anim, 0x38) = 0;
    F<unsigned long>(anim, 0x58) = 0;
    g_matches = 5; g_clientSize = 12; ResetLog(); g_setN = 0; memset(g_allocDest, 0, sizeof(g_allocDest));
    ok = a->InitialiseAnimationInstance(*s, *(const C3DMesh2*)g_meshImg, false, &match);
    check(ok, "allocator arm returns true");
    check(strcmp(g_log, "TLSICXKMRU") == 0, "allocator arm call order");
    if (strcmp(g_log, "TLSICXKMRU") != 0) printf("  log %s\n", g_log);
    check(g_capacity == 12 && g_setN == 1, "capacity set to the client size, no second SetMemoryPool");
    check(memcmp(g_allocDest, g_scratch, 12) == 0 && g_allocDest[12] == 0, "client bytes copied into the allocation");
    check(g_relHandle == 0 && g_relSrc == g_scratch && g_relDst == g_allocDest && g_relSize == 12, "RelocateData(0, temp, dest, size)");
    check(g_fitSeen == false, "fit_to_mesh passed");
    check(match == 5.0f, "out_match = 5 / max(0, 1)");
    check(ListSize(&instances) == 2 && sharedTrackInfo->RefCount == 2 && g_deleted == 1, "second instance; track already shared");
    check(!F<bool>(anim, 0x74) && !g_locked, "lock cleared, scratch unlocked (allocator arm)");

    // 3) failed resource allocation: false, out_match untouched, scratch unlocked, lock cleared
    F<void*>(anim, 0x38) = g_poolImg;
    g_allocOk = false; ResetLog(); g_setN = 0; match = -1.0f;
    ok = a->InitialiseAnimationInstance(*s, *(const C3DMesh2*)g_meshImg, true, &match);
    check(!ok, "failed allocation returns false");
    check(strcmp(g_log, "TLSICAU") == 0, "failed allocation call order");
    check(match == -1.0f, "out_match untouched on failure");
    check(!F<bool>(anim, 0x74) && !g_locked, "lock cleared, scratch unlocked (failure)");

    // 4) null out_match
    g_allocOk = true; ResetLog();
    ok = a->InitialiseAnimationInstance(*s, *(const C3DMesh2*)g_meshImg, true, 0);
    check(ok && strcmp(g_log, "TLSICAPRSU") == 0, "null out_match");
    check(ListSize(&instances) == 4, "every call registers an instance");

    if (!g_fail) printf("00a50c20_TEST PASS\n");
    return g_fail;
}