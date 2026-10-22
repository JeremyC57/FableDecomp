// Behaviour test for C3DAnimationStats::InitialiseStats (0x00A4EBB0): model and body verbatim, the chunk accessors,
// CCharString copy/dtor and CEventMap::insert stubbed; exercises no-helpers, movement-vector and timing-event paths
// and checks the counted-pointer release balance.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define _snprintf snprintf
#endif

static int g_strCopies = 0, g_strDtors = 0, g_blocksFreed = 0, g_objDestroyed = 0;

void operator delete(void* p) { if (p) g_blocksFreed++; free(p); }
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer {
public:
    T* Object;
    SCountBlock* Info;
    CCountedPointer() : Object(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Info(o.Info) { if (Info) ++Info->m_references; }   // test only: real copies share the block
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Object = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* c = other.Info;
        T* p = other.Object;
        if (Info != c) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Object = p;
            Info = c;
            if (c)
                ++c->m_references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
class CCharString {
public:
    void* Data;
    CCharString() : Data(0) {}                 // test only: static event table
    CCharString(const CCharString& other);
    ~CCharString();
};
CCharString::CCharString(const CCharString& other) : Data(other.Data) { g_strCopies++; }
CCharString::~CCharString() { g_strDtors++; Data = 0; }
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C3DAnimFileTimingEventChunk {
public:
    char _pad0[0x10];
    CCharString Name;   // +0x10
    float Time;         // +0x14
};
class C3DAnimFileMovementVectorChunk {
public:
    char _pad0[0x2c];
    C3DVector Vector;   // +0x2c
};
class C3DAnimFileHelpersChunk {
public:
    bool HasMovementVector() const;
    CCountedPointer<C3DAnimFileMovementVectorChunk> GetMovementVector();
    CCountedPointer<C3DAnimFileTimingEventChunk> FindFirstEvent();
    CCountedPointer<C3DAnimFileTimingEventChunk> FindNextEvent(const CCountedPointer<C3DAnimFileTimingEventChunk>& previous);
};
class C3DAnimFileRootChunk {
public:
    char _pad0[0x2c];
    bool LoopFlag;      // +0x2c
    float Duration;     // +0x30
    bool HasHelpers() const;
    CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();
};
class C3DAnimFile {
public:
    CCountedPointer<C3DAnimFileRootChunk> GetRootChunk() const;
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
    template <class C, class D> pair(const pair<C, D>& p) : first(p.first), second(p.second) {}
};
template <class A, class B> inline pair<A, B> make_pair(const A& a, const B& b) { return pair<A, B>(a, b); }
}
class CEventMap {
public:
    typedef std::pair<CCharString, float> value_type;
    struct iterator { void* _M_node; };
    void* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    iterator insert(const value_type& value);
};
enum EAnimationType { ANIMATION_TYPE_NONE, ANIMATION_TYPE_WALK = 7 };
#pragma pack(push, 2)
class C3DAnimationStats {
public:
    void* _vptr;
    long _pad4;
    EAnimationType AnimType;     // +0x08
    float Duration;              // +0x0c
    bool LoopFlag;               // +0x10
    bool MovementFlag;           // +0x11
    C3DVector MovementVector;    // +0x12
    short _pad1e;
    CEventMap TimingEvents;      // +0x20
    void InitialiseStats(const C3DAnimFile& file, EAnimationType type);
    void AddTimingEvent(CCharString name, float time) { TimingEvents.insert(std::pair<CCharString, float>(name, time)); }
};
#pragma pack(pop)

// ---- body verbatim (final.cpp) ----
void C3DAnimationStats::InitialiseStats(const C3DAnimFile& file, EAnimationType type)
{
    C3DAnimFileRootChunk* root = file.GetRootChunk().Object;
    C3DAnimationStats* const& self = this;
    AnimType = type;
    LoopFlag = root->LoopFlag;
    Duration = root->Duration;
    self->MovementFlag = false;
    MovementVector.Z = 0.0f;
    MovementVector.Y = 0.0f;
    MovementVector.X = 0.0f;
    if (root->HasHelpers()) {
        CCountedPointer<C3DAnimFileHelpersChunk> helpers = root->GetHelpers();
        if (helpers->HasMovementVector()) {
            MovementFlag = true;
            CCountedPointer<C3DAnimFileMovementVectorChunk> movement = helpers->GetMovementVector();
            MovementVector = C3DVector(movement->Vector);
        }
        CCountedPointer<C3DAnimFileTimingEventChunk> event = helpers->FindFirstEvent();
        while (event.Object) {
            float time = event->Time;
            AddTimingEvent(event->Name, time);
            CCharString name = event->Name;
            event = helpers->FindNextEvent(event);
        }
    }
}
// ---- stubs ----
static C3DAnimFileRootChunk g_root;
static C3DAnimFileHelpersChunk g_helpers;
static C3DAnimFileMovementVectorChunk g_movement;
static C3DAnimFileTimingEventChunk g_events[3];
static int g_eventCount = 0, g_hasHelpers = 0, g_hasMovement = 0;
static void __fastcall DestroyNothing(void*) { g_objDestroyed++; }
template <class T> static CCountedPointer<T> Share(T* object)
{
    CCountedPointer<T> p;
    p.Object = object;
    p.Info = (SCountBlock*)malloc(sizeof(SCountBlock));
    p.Info->m_references = 1;
    p.Info->m_pfnDestroy = DestroyNothing;
    p.Info->m_pObject = object;
    return p;
}
CCountedPointer<C3DAnimFileRootChunk> C3DAnimFile::GetRootChunk() const { return Share(&g_root); }
bool C3DAnimFileRootChunk::HasHelpers() const { return g_hasHelpers != 0; }
CCountedPointer<C3DAnimFileHelpersChunk> C3DAnimFileRootChunk::GetHelpers() { return Share(&g_helpers); }
bool C3DAnimFileHelpersChunk::HasMovementVector() const { return g_hasMovement != 0; }
CCountedPointer<C3DAnimFileMovementVectorChunk> C3DAnimFileHelpersChunk::GetMovementVector() { return Share(&g_movement); }
CCountedPointer<C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindFirstEvent()
{
    if (g_eventCount == 0) return CCountedPointer<C3DAnimFileTimingEventChunk>();
    return Share(&g_events[0]);
}
CCountedPointer<C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindNextEvent(const CCountedPointer<C3DAnimFileTimingEventChunk>& previous)
{
    int index = (int)(previous.Object - g_events) + 1;
    if (index >= g_eventCount) return CCountedPointer<C3DAnimFileTimingEventChunk>();
    return Share(&g_events[index]);
}
static void* g_insertedName[8]; static float g_insertedTime[8]; static int g_inserted = 0;
CEventMap::iterator CEventMap::insert(const value_type& value)
{
    g_insertedName[g_inserted] = value.first.Data;
    g_insertedTime[g_inserted] = value.second;
    g_inserted++;
    _M_node_count++;
    iterator it; it._M_node = 0; return it;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void Reset(C3DAnimationStats& stats)
{
    memset(&stats, 0x55, sizeof(stats));
    stats.TimingEvents._M_node_count = 0;
    g_inserted = 0; g_strCopies = 0; g_strDtors = 0; g_blocksFreed = 0; g_objDestroyed = 0;
}

int main()
{
    C3DAnimationStats stats;
    g_root.LoopFlag = true; g_root.Duration = 2.5f;
    g_movement.Vector = C3DVector(1.0f, -2.0f, 3.0f);
    static char nameA[] = "StepLeft", nameB[] = "StepRight";
    g_events[0].Name.Data = nameA; g_events[0].Time = 0.25f;
    g_events[1].Name.Data = nameB; g_events[1].Time = 0.75f;

    // 1. no helpers: scalar fields only, zero vector, no events, root block released
    Reset(stats); g_hasHelpers = 0; g_hasMovement = 1; g_eventCount = 2;
    stats.InitialiseStats(C3DAnimFile(), ANIMATION_TYPE_WALK);
    CHECK(stats.AnimType == ANIMATION_TYPE_WALK);
    CHECK(stats.LoopFlag == true && stats.Duration == 2.5f && stats.MovementFlag == false);
    CHECK(stats.MovementVector.X == 0.0f && stats.MovementVector.Y == 0.0f && stats.MovementVector.Z == 0.0f);
    CHECK(g_inserted == 0 && stats.TimingEvents._M_node_count == 0);
    CHECK(g_blocksFreed == 1 && g_objDestroyed == 1 && g_strCopies == 0 && g_strDtors == 0);

    // 2. helpers with a movement vector and two timing events
    Reset(stats); g_hasHelpers = 1; g_hasMovement = 1; g_eventCount = 2; g_root.LoopFlag = false;
    stats.InitialiseStats(C3DAnimFile(), ANIMATION_TYPE_NONE);
    CHECK(stats.AnimType == ANIMATION_TYPE_NONE && stats.LoopFlag == false);
    CHECK(stats.MovementFlag == true);
    CHECK(stats.MovementVector.X == 1.0f && stats.MovementVector.Y == -2.0f && stats.MovementVector.Z == 3.0f);
    CHECK(g_inserted == 2 && stats.TimingEvents._M_node_count == 2);
    CHECK(g_insertedName[0] == nameA && g_insertedTime[0] == 0.25f);
    CHECK(g_insertedName[1] == nameB && g_insertedTime[1] == 0.75f);
    CHECK(g_strCopies == 6 && g_strDtors == 6);            // per event: the by-value name, the pair handed to insert, the `name` local
    CHECK(g_blocksFreed == 5 && g_objDestroyed == 5);      // root, helpers, movement, two events

    // 3. helpers without a movement vector and no events
    Reset(stats); g_hasHelpers = 1; g_hasMovement = 0; g_eventCount = 0;
    stats.InitialiseStats(C3DAnimFile(), ANIMATION_TYPE_WALK);
    CHECK(stats.MovementFlag == false);
    CHECK(stats.MovementVector.X == 0.0f && stats.MovementVector.Y == 0.0f && stats.MovementVector.Z == 0.0f);
    CHECK(g_inserted == 0 && g_strCopies == 0 && g_strDtors == 0);
    CHECK(g_blocksFreed == 2 && g_objDestroyed == 2);

    char line[64];
    _snprintf(line, sizeof(line), "00a4ebb0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}