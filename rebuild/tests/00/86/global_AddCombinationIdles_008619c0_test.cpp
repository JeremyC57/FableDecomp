// Behaviour test for 0x008619C0 `anonymous namespace'::AddCombinationIdles (chunk 402): nothing happens when the animate
// thing already runs a combination animation of set type 25 or the creature has no "ST_IDLE_COMB" animation (in any
// group) or no loaded one; otherwise one combination animation is added: type 25, the entry's bank index, update
// mode 1, not looping, the entry's animate-thing flags, priority 100, the frame zeroed, the entry's blend-in time for
// this creature and its delay time.
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int gStrings = 0;
class CCharString {
public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; ++gStrings; }
    ~CCharString() { --gStrings; }
};
class CAnimFilterBase {
public:
    virtual bool Passes(long group) const = 0;
    virtual ~CAnimFilterBase() { }
};
class CAnimFilterAnyGroup : public CAnimFilterBase {
public:
    virtual bool Passes(long group) const { (void)group; return true; }
};
class CThing { };
class CAnimationEntry {
public:
    long          BankIndex;
    long          Group;
    unsigned long AnimateThingFlags;
    long          DelayTime;
    long          BlendIn;
    long GetDelayTime() const { return DelayTime; }
    long GetTimeToBlendIn(const CThing& thing) const { (void)thing; return BlendIn; }
    unsigned long GetAnimateThingFlags() const { return AnimateThingFlags; }
};
class CThingCreatureBase : public CThing {
public:
    const char*      AnimName;
    CAnimationEntry* Loaded;
    CAnimationEntry  Entry;
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const
    {
        return AnimName != 0 && strcmp(AnimName, name.Text) == 0 && filter.Passes(Entry.Group);
    }
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool exclude) const
    {
        (void)exclude;
        return (AnimName != 0 && strcmp(AnimName, name.Text) == 0) ? Loaded : 0;
    }
};
enum EAnimUpdateMode { ANIM_UPDATE_MODE_0, ANIM_UPDATE_MODE_1 };
class CAnimFrameInfo {
public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
};
class CAnimInit {
public:
    EAnimUpdateMode UpdateMode;
    unsigned char   Type;
    bool            Looping;
    long            Anim;
    float           Speed;
    float           Alpha;
    float           LastAlpha;
    CAnimFrameInfo  Frame;
    unsigned long   Flags;
    unsigned long   TypeFlags;
    long            Priority;
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping,
              unsigned long flags, long priority)
    {
        Type = type; TypeFlags = typeFlags; Anim = anim; UpdateMode = updateMode; Looping = looping; Flags = flags;
        Priority = priority; Speed = Alpha = LastAlpha = 1.0f;
    }
};
class CAnimSet { };
class CAnimateThing {
public:
    CAnimSet* Existing;
    int       Added;
    CAnimInit LastInit;
    long      LastBlendIn, LastDelay;
    bool      LastA, LastB;
    CAnimateThing() : Existing(0), Added(0), LastBlendIn(0), LastDelay(0), LastA(true), LastB(true) { }
    CAnimSet* GetFirstCombinationAnimFromType(long type) { return type == 0x19 ? Existing : 0; }
    void AddCombinationAnim(const CAnimInit& init, long blendInTime, long delayTime, bool a, bool b)
    {
        ++Added; LastInit = init; LastBlendIn = blendInTime; LastDelay = delayTime; LastA = a; LastB = b;
    }
};

namespace {

void __fastcall AddCombinationIdles(const CThingCreatureBase& creature, CAnimateThing& animate)
{
    if (animate.GetFirstCombinationAnimFromType(0x19))
        return;
    CCharString name("ST_IDLE_COMB");
    if (creature.HasAnim(name, CAnimFilterAnyGroup())) {
        const CAnimationEntry* anim = creature.GetRandomLoadedAnimFromName(name, false);
        if (anim) {
            unsigned long flags = anim->GetAnimateThingFlags();
            CAnimInit init;
            init.Init(0x19, 0, anim->BankIndex, ANIM_UPDATE_MODE_1, false, flags, 100);
            animate.AddCombinationAnim(init, anim->GetTimeToBlendIn(creature), anim->GetDelayTime(), false, false);
        }
    }
}

}

int main()
{
    int failures = 0;
    CThingCreatureBase creature;
    creature.AnimName = "ST_IDLE_COMB";
    creature.Entry.BankIndex = 321; creature.Entry.Group = 4; creature.Entry.AnimateThingFlags = 6;
    creature.Entry.DelayTime = 12; creature.Entry.BlendIn = 7;
    creature.Loaded = &creature.Entry;
    CAnimateThing animate;
    AddCombinationIdles(creature, animate);
    if (animate.Added != 1) { printf("  not added\n"); ++failures; }
    else {
        const CAnimInit& i = animate.LastInit;
        if (i.Type != 0x19 || i.TypeFlags != 0 || i.Anim != 321 || i.UpdateMode != ANIM_UPDATE_MODE_1 || i.Looping
            || i.Flags != 6 || i.Priority != 100 || i.Frame.Current != 0.0f || i.Frame.Last != 0.0f) {
            printf("  wrong init\n"); ++failures;
        }
        if (animate.LastBlendIn != 7 || animate.LastDelay != 12 || animate.LastA || animate.LastB) { printf("  wrong add arguments\n"); ++failures; }
    }
    CAnimSet running;
    animate.Existing = &running;
    AddCombinationIdles(creature, animate);
    if (animate.Added != 1) { printf("  added while one is running\n"); ++failures; }
    animate.Existing = 0;
    creature.Loaded = 0;
    AddCombinationIdles(creature, animate);
    if (animate.Added != 1) { printf("  added without a loaded animation\n"); ++failures; }
    creature.Loaded = &creature.Entry;
    creature.AnimName = "ST_IDLE";
    AddCombinationIdles(creature, animate);
    if (animate.Added != 1) { printf("  added without the animation\n"); ++failures; }
    if (gStrings != 0) { printf("  name string leaked\n"); ++failures; }
    if (failures == 0) {
        printf("008619c0_TEST PASS\n");
        return 0;
    }
    printf("008619c0_TEST FAIL\n");
    return 1;
}