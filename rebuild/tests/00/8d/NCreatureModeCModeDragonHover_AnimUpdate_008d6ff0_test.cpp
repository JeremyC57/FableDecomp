#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeDragonHover::AnimUpdate (retail 0x008D6FF0): an idle hovering dragon whose
// animation list is empty or replaceable gets a random loaded "ST_IDLE_SUBTLE" animation as its main animation
// (type 6, mode 1, looping flag, the entry's flags, 100%) with the entry's blend-in and transition times.
class CThing { public: int id; };
class CAnimInit { public:
    long Mode; long AnimType; bool Flag05; long AnimIndex; unsigned long Flags; long Arg24; long Percent;
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
        : Mode(mode), AnimType(animType), Flag05(flag05), AnimIndex(animIndex), Flags(flags), Arg24(arg24), Percent(percent) { }
};
class CAnimationEntry { public:
    long AnimIndex;
    unsigned long GetAnimateThingFlags() { return 0x40; }
    long GetAnimationTransitionInTime() { return 7; }
    long GetTimeToBlendIn(CThing* thing) { return thing ? 5 : -1; }
};
class CAnimateThing { public:
    int AnimsBegin;
    int AnimsEnd;
    unsigned char CurrentAnimFlags;
    int sets;
    long setIndex, setBlend, setTransition, setType, setMode, setPercent;
    unsigned long setFlags;
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition)
    {
        ++sets; setIndex = init.AnimIndex; setBlend = blendIn; setTransition = transition; setType = init.AnimType;
        setMode = init.Mode; setPercent = init.Percent; setFlags = init.Flags;
    }
};
static CAnimationEntry* g_idle;
static bool g_busy;
class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() { return g_busy; }
    CAnimationEntry* GetRandomLoadedAnimFromName(const char* name, bool loadedOnly)
    {
        return (std::strcmp(name, "ST_IDLE_SUBTLE") == 0 && !loadedOnly) ? g_idle : 0;
    }
};
struct CModeDragonHover {
    CThingCreatureBase* ParentThing;
    void AnimUpdate(CAnimateThing& animate)
    {
        CThingCreatureBase* creature = ParentThing;
        if (creature->IsPerformingAnyAction())
            return;
        if (animate.AnimsBegin != animate.AnimsEnd && !(animate.CurrentAnimFlags & 2))
            return;
        CAnimationEntry* entry = creature->GetRandomLoadedAnimFromName("ST_IDLE_SUBTLE", false);
        if (entry) {
            CThing* thing = ParentThing;
            animate.SetMainAnim(CAnimInit(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100),
                entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
        }
    }
};

int main()
{
    CAnimationEntry idle;
    idle.AnimIndex = 77;
    g_idle = &idle;
    g_busy = false;
    CThingCreatureBase dragon;
    CModeDragonHover m;
    m.ParentThing = &dragon;
    CAnimateThing a;
    a.AnimsBegin = 0; a.AnimsEnd = 0; a.CurrentAnimFlags = 0; a.sets = 0;
    m.AnimUpdate(a);
    int pass = a.sets == 1 && a.setIndex == 77 && a.setBlend == 5 && a.setTransition == 7 && a.setType == 6
        && a.setMode == 1 && a.setPercent == 100 && a.setFlags == 0x40;
    a.AnimsEnd = 1;
    m.AnimUpdate(a);
    pass = pass && a.sets == 1;
    a.CurrentAnimFlags = 2;
    m.AnimUpdate(a);
    pass = pass && a.sets == 2;
    g_busy = true;
    m.AnimUpdate(a);
    pass = pass && a.sets == 2;
    g_busy = false; g_idle = 0;
    m.AnimUpdate(a);
    pass = pass && a.sets == 2;
    if (!pass) { std::printf("008d6ff0_TEST FAIL\n"); return 1; }
    std::printf("008d6ff0_TEST PASS\n");
    return 0;
}