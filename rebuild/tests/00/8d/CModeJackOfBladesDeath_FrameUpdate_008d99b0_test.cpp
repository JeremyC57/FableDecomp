// 008d99b0 behaviour test: NCreatureMode::CModeJackOfBladesDeath::FrameUpdate (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>
#include <math.h>

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

static int g_destroys;
struct CJackOfBladesDeathDef {
    long  m_refCount;
    float DeathDuration;
    float RiseHeight;
    void Destroy() { ++g_destroys; }
};
static CJackOfBladesDeathDef g_def;
struct CDefClassBase {
    bool GetSubDef(CJackOfBladesDeathDef** out) { ++g_def.m_refCount; *out = &g_def; return true; }
};

static int g_enableCalls; static bool g_enabled = true;
static int g_setPosCalls; static C3DVector g_pos;
struct CPhysicsObject {
    void SetPosition(const C3DVector& position) { ++g_setPosCalls; g_pos = position; }
    void SetEnabled(bool enabled) { ++g_enableCalls; g_enabled = enabled; }
};

static int g_removes; static unsigned long g_removedMode;
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode) { ++g_removes; g_removedMode = mode; }
};

struct CThing {
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CPhysicsObject* PPhysics;
    CDefClassBase*  PDef;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

static long g_GameFramesPerSecond;

struct CModeStandBase {
    CThing* ParentThing;
};

struct CModeJackOfBladesDeath : CModeStandBase {
    C3DVector StartPos;
    long      Timer;
    void FrameUpdate();
};

void CModeJackOfBladesDeath::FrameUpdate()
{
    CJackOfBladesDeathDef* subDef = 0;
    ParentThing->PDef->GetSubDef(&subDef);
    CJackOfBladesDeathDef* def = subDef;
    ParentThing->PPhysics->SetEnabled(false);
    float t = (float)Timer / (long)(g_GameFramesPerSecond * def->DeathDuration);
    C3DVector target = C3DVector(0.0f, 0.0f, def->RiseHeight) + StartPos;
    C3DVector position = (target - StartPos) * t + StartPos;
    ParentThing->PPhysics->SetPosition(position);
    if (++Timer > g_GameFramesPerSecond * def->DeathDuration) {
        CTCCreatureModeManager* modes = 0;
        ParentThing->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x35);
        if (--def->m_refCount == 0)
            def->Destroy();
        return;
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    CTCCreatureModeManager modes;
    CInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0x31, (CTCBase*)&modes }, { 0x40, 0 } };
    CPhysicsObject physics;
    CDefClassBase defBase;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    thing.PPhysics = &physics; thing.PDef = &defBase;
    CModeJackOfBladesDeath mode;
    mode.ParentThing = &thing;
    mode.StartPos = C3DVector(1.0f, 2.0f, 3.0f);
    g_GameFramesPerSecond = 10;
    g_def.DeathDuration = 2.0f;   // 20 frames
    g_def.RiseHeight = 4.0f;

#define RESET() (g_destroys = 0, g_enableCalls = 0, g_enabled = true, g_setPosCalls = 0, g_removes = 0, g_removedMode = 0)
    // mid-rise: Timer 5 of 20 -> a quarter of the way up; the def ref taken by GetSubDef is dropped (1 -> 0 destroys)
    RESET(); g_def.m_refCount = 0; mode.Timer = 5; mode.FrameUpdate();
    Check("mid enable", g_enableCalls == 1 && !g_enabled);
    Check("mid position", g_setPosCalls == 1 && Near(g_pos.X, 1.0f) && Near(g_pos.Y, 2.0f) && Near(g_pos.Z, 4.0f));
    Check("mid timer", mode.Timer == 6 && g_removes == 0);
    Check("mid release", g_def.m_refCount == 0 && g_destroys == 1);
    // def still referenced elsewhere: no destroy
    RESET(); g_def.m_refCount = 2; mode.Timer = 10; mode.FrameUpdate();
    Check("half position", Near(g_pos.Z, 5.0f) && mode.Timer == 11 && g_removes == 0);
    Check("shared release", g_def.m_refCount == 2 && g_destroys == 0);
    // Timer 20 -> 21 > 20: death mode removed through the 0x31 component
    RESET(); g_def.m_refCount = 0; mode.Timer = 20; mode.FrameUpdate();
    Check("end position", Near(g_pos.Z, 7.0f) && mode.Timer == 21);
    Check("end remove", g_removes == 1 && g_removedMode == 0x35 && g_destroys == 1 && g_def.m_refCount == 0);
    // exactly at the boundary (Timer 19 -> 20 is not > 20)
    RESET(); g_def.m_refCount = 0; mode.Timer = 19; mode.FrameUpdate();
    Check("boundary", g_removes == 0 && mode.Timer == 20 && g_destroys == 1);
    if (g_fail) return 1;
    printf("008d99b0_TEST PASS\n");
    return 0;
}