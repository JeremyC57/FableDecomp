// Behaviour test for NCreatureMode::CModeCrouch::AnimUpdate (0x008D8B00): the sneak animation choice (moving/standing x
// upright/low), the "already playing" skip and the remembered name, on a host copy of the model.
#include <cstdio>
#include <cstring>
#include <cmath>

class CCharString { public:
    char text[32];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); }
    bool operator!=(const CCharString& o) const { return std::strcmp(text, o.text) != 0; }
    const CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
};
class CThing { public: int id; };
class CAnimInit { public:
    long animType, arg24, animIndex, mode; bool flag05; unsigned long flags; long percent;
    CAnimInit(long t, long a, long i, long m, bool f, unsigned long fl, long p)
        : animType(t), arg24(a), animIndex(i), mode(m), flag05(f), flags(fl), percent(p) { }
};
class CAnimationEntry { public:
    long AnimIndex;
    unsigned long GetAnimateThingFlags() { return 0x40; }
    long GetAnimationTransitionInTime() { return 7; }
    long GetTimeToBlendIn(CThing*) { return 5; }
};
class C3DVector { public: float X, Y, Z; };
class CTCStealth { public: bool Viewable; };
static char g_requested[32];
static CAnimationEntry g_entry = { 42 };
class CThingCreatureBase : public CThing { public:
    C3DVector MovementVector;
    CTCStealth* stealth;
    template <class T> bool GetInterface(const T** out, long type) const
    {
        if (type == 0x36 && stealth) { *out = stealth; return true; }
        return false;
    }
    CAnimationEntry* GetAnimFromName(const CCharString& name, long) { std::strcpy(g_requested, name.text); return &g_entry; }
};
class CAnimateThing { public:
    void* AnimsBegin; void* AnimsEnd;
    unsigned char CurrentAnimType, CurrentAnimFlags;
    int calls; long lastType, lastMode, lastIndex, lastBlend, lastTransition;
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition)
    {
        ++calls; lastType = init.animType; lastMode = init.mode; lastIndex = init.animIndex;
        lastBlend = blendIn; lastTransition = transition;
    }
    bool IsPlayingMainAnimType(unsigned char type) const
    {
        return AnimsBegin != AnimsEnd && !(CurrentAnimFlags & 2) && CurrentAnimType == type;
    }
};
struct CModeCrouch {
    CThing* ParentThing;
    CCharString SneakAnimName;
    CModeCrouch() : SneakAnimName("") { }
    void AnimUpdate(CAnimateThing& animate)
    {
        CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
        bool upright = true;
        const CTCStealth* stealth;
        if (creature->GetInterface(&stealth, 0x36) && !stealth->Viewable)
            upright = false;
        float speed = (float)std::sqrt(creature->MovementVector.X * creature->MovementVector.X
            + creature->MovementVector.Y * creature->MovementVector.Y);
        if (speed > 0.0001f) {
            CCharString name(upright ? "ST_SNEAK_UPRIGHT" : "ST_SNEAK");
            if (name != SneakAnimName || !animate.IsPlayingMainAnimType(0x0e)) {
                CAnimationEntry* entry = creature->GetAnimFromName(name, 0);
                CThing* thing = ParentThing;
                animate.SetMainAnim(CAnimInit(0x0e, 0, entry->AnimIndex, 2, true, entry->GetAnimateThingFlags(), 100),
                    entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
            }
            SneakAnimName = name;
        }
        else {
            CCharString name(upright ? "ST_SNEAK_UPRIGHT_POSE" : "ST_SNEAK_POSE");
            if (name != SneakAnimName || !animate.IsPlayingMainAnimType(0x0f)) {
                CAnimationEntry* entry = creature->GetAnimFromName(name, 0);
                CThing* thing = ParentThing;
                animate.SetMainAnim(CAnimInit(0x0f, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100),
                    entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
            }
            SneakAnimName = name;
        }
    }
};

int main()
{
    int fail = 0;
    CTCStealth stealth; stealth.Viewable = false;
    CThingCreatureBase creature; creature.MovementVector.X = 0.3f; creature.MovementVector.Y = 0.4f; creature.MovementVector.Z = 9.0f;
    creature.stealth = &stealth;
    int anims[2];
    CAnimateThing animate; animate.AnimsBegin = anims; animate.AnimsEnd = anims + 1; animate.CurrentAnimType = 0x0e;
    animate.CurrentAnimFlags = 0; animate.calls = 0;
    CModeCrouch mode; mode.ParentThing = &creature;
    // moving, not viewable -> ST_SNEAK; the name differs so it plays
    mode.AnimUpdate(animate);
    if (animate.calls != 1 || std::strcmp(g_requested, "ST_SNEAK") || animate.lastType != 0x0e || animate.lastMode != 2
        || animate.lastIndex != 42 || animate.lastBlend != 5 || animate.lastTransition != 7
        || std::strcmp(mode.SneakAnimName.text, "ST_SNEAK")) fail |= 1;
    // same name, type 0x0e already playing -> skipped
    mode.AnimUpdate(animate);
    if (animate.calls != 1) fail |= 2;
    // finishing flag set -> plays again
    animate.CurrentAnimFlags = 2; mode.AnimUpdate(animate);
    if (animate.calls != 2) fail |= 4;
    // standing, viewable (no stealth component) -> ST_SNEAK_UPRIGHT_POSE, mode 1, type 0x0f
    animate.CurrentAnimFlags = 0; creature.MovementVector.X = 0.0f; creature.MovementVector.Y = 0.00005f; creature.stealth = 0;
    mode.AnimUpdate(animate);
    if (animate.calls != 3 || std::strcmp(g_requested, "ST_SNEAK_UPRIGHT_POSE") || animate.lastType != 0x0f
        || animate.lastMode != 1 || std::strcmp(mode.SneakAnimName.text, "ST_SNEAK_UPRIGHT_POSE")) fail |= 8;
    // the Z component is ignored: a vertical-only movement still counts as standing
    creature.MovementVector.Y = 0.0f; creature.MovementVector.Z = 5.0f; animate.CurrentAnimType = 0x0f;
    mode.AnimUpdate(animate);
    if (animate.calls != 3) fail |= 16;
    if (fail) { std::printf("008d8b00_TEST FAIL\n"); return 1; }
    std::printf("008d8b00_TEST PASS\n");
    return 0;
}