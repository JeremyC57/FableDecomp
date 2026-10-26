// Behaviour test for NCreatureMode::CModeSitBase::AnimUpdate (0x008D6AC0): the pending-remove guard, the idle countdown,
// the one-off ST_IDLE action, the ST_IDLE_SUBTLE restart and the random re-seed, on a host copy of the model.
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[32];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); }
};
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) { } };
class CThing { public: int id; };
class CAnimationEntry { public:
    long BankIndex;
    unsigned long GetAnimateThingFlags() const { return 8; }
    long GetTimeToBlendIn(CThing*) const { return 3; }
};
class CAnimInit { public:
    unsigned char type; long anim, updateMode; bool looping; long priority;
    CAnimInit(unsigned char t, unsigned long, long a, long m, bool l, unsigned long, long p)
        : type(t), anim(a), updateMode(m), looping(l), priority(p) { }
};
class CAnimateThing { public:
    short* m_animsFirst; short* m_animsLast;
    unsigned char Type; bool Finished;
    int calls; unsigned char lastType; long lastAnim;
    void SetMainAnim(const CAnimInit& init, long, long) { ++calls; lastType = init.type; lastAnim = init.anim; }
};
class CCreatureActionBase { public: char name[32]; };
class CThingCreatureBase;
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString& n, const CSoundPair&, bool, bool, bool, long, long)
    { std::strcpy(name, n.text); }
};
static CAnimationEntry g_entry = { 17 };
static char g_lastAction[32], g_lastLookup[32];
class CThingCreatureBase : public CThing { public:
    bool accept;
    CAnimationEntry* GetAnimFromName(const CCharString& n, long) { std::strcpy(g_lastLookup, n.text); return &g_entry; }
    bool SetCurrentAction(const CCreatureActionBase& a) { std::strcpy(g_lastAction, a.name); return accept; }
};
struct CRandomSeed { unsigned long Seed; };
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(CRandomSeed* r)
{
    r->Seed = r->Seed * 0x24a1 + 0x24df;
    r->Seed = GFROR13(r->Seed) & 0xffffffffUL;
    return r->Seed;
}
static CRandomSeed g_seed = { 12345 };
struct CModeSitBase {
    CThingCreatureBase* ParentThing;
    long IdleCounter;
    bool PlayingIdle;
    CCreatureActionBase* PRemoveAction;
    unsigned long GetType() const { return 7; }
    long GetNoFramesBeforePlayingCombinationIdleAnims(unsigned long) { return 40; }
    void AnimUpdate(CAnimateThing& anim)
    {
        if (PRemoveAction == 0) {
            --IdleCounter;
            CThingCreatureBase* creature = ParentThing;
            if (anim.m_animsFirst != anim.m_animsLast && !anim.Finished && anim.Type == 0x15) {
                if (!PlayingIdle && IdleCounter < 0) {
                    bool started;
                    {
                        CCharString name("ST_IDLE");
                        started = creature->SetCurrentAction(CCreatureAction_PlayAnimation(*creature, name, CSoundPair(),
                            false, true, false, 0, 0));
                    }
                    if (started)
                        PlayingIdle = true;
                }
            }
            else {
                CAnimationEntry* entry;
                {
                    CCharString name("ST_IDLE_SUBTLE");
                    entry = creature->GetAnimFromName(name, 0L);
                }
                CThing* thing = ParentThing;
                anim.SetMainAnim(CAnimInit(0x15, 0, entry->BankIndex, 1, true, entry->GetAnimateThingFlags(), 100),
                    entry->GetTimeToBlendIn(thing), 0);
                if (PlayingIdle) {
                    float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims(GetType());
                    long minFrames = (long)(frames * 0.75f);
                    CRandomSeed* random = &g_seed;
                    unsigned long range = (unsigned long)(-minFrames - (long)(frames * -1.5f));
                    unsigned long r = GFRandom(random);
                    IdleCounter = (long)((range ? r % range : 0) + minFrames);
                }
                PlayingIdle = false;
            }
        }
    }
};

int main()
{
    int fail = 0;
    short anims[1];
    CAnimateThing anim; anim.m_animsFirst = anims; anim.m_animsLast = anims + 1; anim.Type = 0x15; anim.Finished = false;
    anim.calls = 0;
    CThingCreatureBase creature; creature.accept = true;
    CModeSitBase mode; mode.ParentThing = &creature; mode.IdleCounter = 1; mode.PlayingIdle = false; mode.PRemoveAction = 0;
    g_lastAction[0] = 0;
    mode.AnimUpdate(anim);                   // counter 1 -> 0: not yet negative, no action
    if (mode.IdleCounter != 0 || g_lastAction[0] || mode.PlayingIdle) fail |= 1;
    mode.AnimUpdate(anim);                   // counter 0 -> -1: the idle action starts
    if (mode.IdleCounter != -1 || std::strcmp(g_lastAction, "ST_IDLE") || !mode.PlayingIdle || anim.calls) fail |= 2;
    anim.Finished = true;                    // the sitting anim finished: restart ST_IDLE_SUBTLE and re-seed in [30, 60)
    mode.AnimUpdate(anim);
    if (anim.calls != 1 || anim.lastType != 0x15 || anim.lastAnim != 17 || std::strcmp(g_lastLookup, "ST_IDLE_SUBTLE")
        || mode.PlayingIdle || mode.IdleCounter < 30 || mode.IdleCounter >= 60) fail |= 4;
    long counter = mode.IdleCounter;
    mode.AnimUpdate(anim);                   // not idling: restart only, the counter just ticks
    if (anim.calls != 2 || mode.IdleCounter != counter - 1) fail |= 8;
    CCreatureActionBase pending; mode.PRemoveAction = &pending; counter = mode.IdleCounter;
    mode.AnimUpdate(anim);                   // a pending remove action freezes everything
    if (anim.calls != 2 || mode.IdleCounter != counter) fail |= 16;
    mode.PRemoveAction = 0; anim.Finished = false; mode.IdleCounter = -5; creature.accept = false;
    mode.AnimUpdate(anim);                   // refused action: PlayingIdle stays clear
    if (mode.PlayingIdle) fail |= 32;
    if (fail) { std::printf("008d6ac0_TEST FAIL\n"); return 1; }
    std::printf("008d6ac0_TEST PASS\n");
    return 0;
}