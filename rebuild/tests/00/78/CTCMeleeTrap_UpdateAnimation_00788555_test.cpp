// Behaviour test for CTCMeleeTrap::UpdateAnimation (0x00788555): the animation always steps by the thing's update time; an open
// opener starts continuous play (type 2) or resets; a closed, inactive opener ends the trigger and kills a type-4 trap.
#include <cstdio>
class CTCBase;
class CWorld { public: int id; };
static CWorld g_world;
struct CAnimFrameTime {
    float Time; float Offset; float Blend;
    CAnimFrameTime(float time) : Offset(0.0f), Blend(0.0f) { Time = time; }
};
static int g_frames = 0; static float g_lastTime = -1;
class CAnimateThing { public: void UpdateFrameLogic(const CAnimFrameTime& time) { ++g_frames; g_lastTime = time.Time + time.Offset + time.Blend; } };
static CAnimateThing g_anim;
struct CTCAnimation { CAnimateThing& GetAnimation() { return g_anim; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CComponentBitset {
    unsigned long m_words[9];
    bool test(long bit) { return (m_words[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
static int g_kills = 0;
class CThing {
public:
    CComponentBitset UsingComponents;
    CInterfaceMap    m_interfaces;
    template <class T> T* PeekTC(long type)
    {
        T* tc = 0;
        if (UsingComponents.test(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            tc = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
        return tc;
    }
    void Kill(bool immediately) { if (immediately) ++g_kills; }
};
float GetUpdateTimeForThing(CThing* thing, CWorld* world) { return world == &g_world ? 0.25f : -1.0f; }
static bool g_isOpen = false;
class CFadeInFadeOutBase { public: bool Active; bool IsOpen() { return g_isOpen; } };
static int g_continuous = 0, g_resets = 0;
class CTCTrapBase { public:
    CThing*       ParentThing;
    CFadeInFadeOutBase TrapOpener;
    bool          Triggered;
    long          TrapType;
    CWorld* GetWorld() { return &g_world; }
    void Reset() { ++g_resets; }
};
struct CTCMeleeTrap : CTCTrapBase {
    bool PlayingContinuous;
    void InitialiseContinuousPlay() { ++g_continuous; PlayingContinuous = true; }
    void UpdateAnimation();
};

void CTCMeleeTrap::UpdateAnimation()
{
    CThing* thing = ParentThing;
    CAnimFrameTime time(GetUpdateTimeForThing(thing, GetWorld()));
    CAnimateThing& animation = ParentThing->PeekTC<CTCAnimation>(0x5a)->GetAnimation();
    if (!PlayingContinuous && Triggered) {
        if (TrapOpener.IsOpen()) {
            if (TrapType == 2)
                InitialiseContinuousPlay();
            else
                Reset();
        } else if (!TrapOpener.Active) {
            if (TrapType == 4)
                ParentThing->Kill(true);
            Triggered = false;
        }
    }
    animation.UpdateFrameLogic(time);
}

int main()
{
    int bad = 0;
    CTCAnimation animTC;
    CInterfaceMap::SEntry entries[1] = { { 0x5a, (CTCBase*)&animTC } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.UsingComponents.m_words[i] = 0;
    thing.UsingComponents.m_words[2] = 1UL << (0x5a & 31); thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCMeleeTrap t; t.ParentThing = &thing; t.TrapOpener.Active = false; t.Triggered = false; t.TrapType = 2; t.PlayingContinuous = false;
    t.UpdateAnimation();
    if (g_frames != 1 || g_lastTime != 0.25f || g_continuous || g_resets) bad |= 1;
    t.Triggered = true; g_isOpen = true;
    t.UpdateAnimation();                                // open, type 2: continuous play
    if (g_continuous != 1 || !t.PlayingContinuous || g_frames != 2) bad |= 2;
    t.PlayingContinuous = false; t.TrapType = 3;
    t.UpdateAnimation();                                // open, other type: reset
    if (g_resets != 1) bad |= 4;
    g_isOpen = false; t.TrapOpener.Active = true;
    t.UpdateAnimation();                                // still fading: nothing
    if (!t.Triggered || g_kills) bad |= 8;
    t.TrapOpener.Active = false; t.TrapType = 4;
    t.UpdateAnimation();                                // closed: trigger over, type 4 kills
    if (t.Triggered || g_kills != 1 || g_frames != 5) bad |= 16;
    if (bad) { std::printf("00788555_TEST FAIL\n"); return 1; }
    std::printf("00788555_TEST PASS\n");
    return 0;
}