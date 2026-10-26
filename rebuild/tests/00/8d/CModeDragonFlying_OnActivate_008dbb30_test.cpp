// Behaviour test for NCreatureMode::CModeDragonFlying::OnActivate (0x008DBB30): model and body verbatim from final.cpp
// with the out-of-line callees stubbed; checks the suspend call for a player creature only, the flag clear, the
// un-willable call, the leg length, the speed/acceleration priming, the animation priming and the clamped playback rate.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#define __forceinline inline
#define __fastcall
#endif

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CModeBase;
static int g_suspend = 0; static bool g_suspendFlag = false;
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetSuspendAnimation(bool suspend, CModeBase* mode) { g_suspend++; g_suspendFlag = suspend; }
};
struct CTCFlagged {
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;
};
class CAnimateThing { public:
    unsigned char _pad00[0x1c];
    float         PlaybackSpeed;
    float         TargetPlaybackSpeed;
};
static CAnimateThing g_anim; static float g_duration = 2.0f; static int g_indexCalls = 0; static long g_indexSeen = -1;
class CTCAnimationComplex { public:
    CAnimateThing* GetAnimateThing() { return &g_anim; }
    long GetAnimIndex() { g_indexCalls++; return 7; }
    float GetAnimDuration(long animIndex) { g_indexSeen = animIndex; return g_duration; }
};
static int g_willable = 0; static bool g_willableFlag = false; static CThing* g_willableThing = 0;
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable) { g_willable++; g_willableFlag = notWillable; g_willableThing = thing; }

long g_GameFramesPerSecond = 25;

class C3DVector { public:
    float X, Y, Z;
};

static int g_baseActivate = 0; static int g_animUpdate = 0; static CAnimateThing* g_animUpdated = 0;
struct CModeBase {
    virtual void v00() {}
    virtual void v01() {}
    virtual void AnimUpdate(CAnimateThing& anim) { g_animUpdate++; g_animUpdated = &anim; }
    CThing*       ParentThing;
    unsigned char _pad08[0x10];
    bool          Active;
    void OnActivate() { g_baseActivate++; Active = true; }
};

namespace NCreatureMode {

struct CModeDragonFlying : CModeBase {
    unsigned char _pad1c[0x18];
    long          FlapCounter;
    unsigned char _pad38[0x0c];
    C3DVector     Destination;
    C3DVector     StartPos;
    unsigned char _pad5c[0x0c];
    float         DistanceToTravel;
    float         Speed;
    float         StartSpeed;
    float         MovementDuringAnimationLoop;
    float         Acceleration;
    float         AnimLength;
    unsigned char _pad80[0x10];
    bool          FirstFrame;
    void OnActivate();
};

__forceinline float Distance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return (float)sqrt(dx * dx + dy * dy + dz * dz);
}
void CModeDragonFlying::OnActivate()
{
    CModeBase::OnActivate();
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, this);
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a))
        flagged->m_flag = 0;
    SetThingAsNotBloodyWillableAtAll(ParentThing, true);
    DistanceToTravel = Distance(Destination, StartPos);
    Acceleration = 0.0f;
    StartSpeed = Speed;
    CTCAnimationComplex* animation = 0;
    ParentThing->GetInterface(&animation, 0x5a);
    CAnimateThing* anim = animation->GetAnimateThing();
    AnimUpdate(*anim);
    AnimLength = animation->GetAnimDuration(animation->GetAnimIndex());
    MovementDuringAnimationLoop = AnimLength * Speed;
    float frames = (float)g_GameFramesPerSecond;
    float rate = 1.0f / (AnimLength * frames);
    if (rate < 0.0f)
        rate = 0.0f;
    else if (rate > 1.0f)
        rate = 1.0f;
    anim->PlaybackSpeed = rate;
    anim->TargetPlaybackSpeed = rate;
    FirstFrame = true;
    FlapCounter = 0;
}

}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    static CThing thing; static CTCFlagged flagged; static CTCAnimationComplex animation;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x3a; entries[0].component = (CTCBase*)&flagged;
    entries[1].type = 0x5a; entries[1].component = (CTCBase*)&animation;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    thing.m_presentBits[0x3a >> 5] |= 1UL << (0x3a & 31);
    thing.m_presentBits[0x5a >> 5] |= 1UL << (0x5a & 31);
    thing.m_type = 1;
    flagged.m_flag = 1;

    NCreatureMode::CModeDragonFlying mode;
    mode.ParentThing = &thing; mode.Active = false; mode.FlapCounter = 9; mode.FirstFrame = false;
    mode.Destination.X = 4.0f; mode.Destination.Y = 7.0f; mode.Destination.Z = 12.0f;
    mode.StartPos.X = 1.0f; mode.StartPos.Y = 3.0f; mode.StartPos.Z = 0.0f;
    mode.Speed = 2.5f; mode.Acceleration = 9.0f; mode.StartSpeed = -1.0f;
    g_anim.PlaybackSpeed = g_anim.TargetPlaybackSpeed = -5.0f;

    mode.OnActivate();
    CHECK(g_baseActivate == 1 && mode.Active);
    CHECK(g_suspend == 1 && g_suspendFlag);
    CHECK(flagged.m_flag == 0);
    CHECK(g_willable == 1 && g_willableFlag && g_willableThing == &thing);
    CHECK(Near(mode.DistanceToTravel, 13.0f));          // sqrt(9 + 16 + 144)
    CHECK(mode.Acceleration == 0.0f && mode.StartSpeed == 2.5f);
    CHECK(g_animUpdate == 1 && g_animUpdated == &g_anim);
    CHECK(g_indexCalls == 1 && g_indexSeen == 7 && mode.AnimLength == 2.0f);
    CHECK(Near(mode.MovementDuringAnimationLoop, 5.0f));
    CHECK(Near(g_anim.PlaybackSpeed, 1.0f / 50.0f) && Near(g_anim.TargetPlaybackSpeed, 1.0f / 50.0f));
    CHECK(mode.FirstFrame && mode.FlapCounter == 0);

    // a non-player creature is not suspended; a tiny animation length clamps the rate to 1
    thing.m_type = 0; g_duration = 0.01f;
    mode.OnActivate();
    CHECK(g_suspend == 1 && g_baseActivate == 2);
    CHECK(g_anim.PlaybackSpeed == 1.0f && g_anim.TargetPlaybackSpeed == 1.0f);
    // a negative length clamps to 0
    g_duration = -1.0f;
    mode.OnActivate();
    CHECK(g_anim.PlaybackSpeed == 0.0f && g_anim.TargetPlaybackSpeed == 0.0f);

    char line[64];
    _snprintf(line, sizeof(line), "008dbb30_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}