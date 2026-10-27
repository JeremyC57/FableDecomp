// Behaviour test for 0x007C4320 CTCHealLife::FinalCleanup: mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

class CTCSpecialAbilities { public: bool AbilityMutexLock; int CurrentAbility; };
class CPlayer { public: void EndSpecialMoveMode() { Log("EndSpecialMove"); } };
class CTCCreatureModeManager { public: void RemoveMode(int m) { char b[32]; std::sprintf(b, "RemoveMode(%x)", m); Log(b); } };
class CThing {
public:
    bool Dead; int State; void* Iface[0x120]; unsigned long TCMask; CPlayer Player;
    CThing() : Dead(false), State(0), TCMask(0) { for (int i = 0; i < 0x120; ++i) Iface[i] = 0; }
    bool IsDead() const { return Dead; }
    template <class T> bool GetInterface(T** out, long type) const { *out = (T*)Iface[type]; return Iface[type] != 0; }
    unsigned long HasTC(long type) const { return type == 0x109 ? TCMask : 0; }
    void Kill(bool b) { Log(b ? "Kill(1)" : "Kill(0)"); }
    CPlayer& GetPlayer() { return Player; }
};

class CThing;
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
class CTCSoundPlayer { public: void StopAttachedSound(unsigned long s) { char b[32]; std::sprintf(b, "StopSound(%lu)", s); Log(b); } };
class CTCDParticleEmitter { public: int Id; void EnterTimelineEnd() { char b[32]; std::sprintf(b, "TimelineEnd(%d)", Id); Log(b); } };
class CEffects {
public:
    CIntelligentPointer<CThing> Items[4]; unsigned int Count;
    unsigned int size() const { return Count; }
    CIntelligentPointer<CThing>& operator[](unsigned int i) { return Items[i]; }
};
class CTCHealLife {
public:
    CThing* ParentThing; unsigned long LoopingSound; CEffects HealingEffects;
    void FinalCleanup();
};
void CTCHealLife::FinalCleanup()
{
    CTCDParticleEmitter* emitter = 0;
    for (long i = 0; i < (long)HealingEffects.size(); ++i) {
        if (IS_THING_ALIVE(HealingEffects[i])) {
            HealingEffects[i].GetPItem()->GetInterface(&emitter, 0x4b);
            emitter->EnterTimelineEnd();
        }
    }
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10))
        sound->StopAttachedSound(LoopingSound);
}
int main()
{
    CThing hero, e0, e1, e2; CTCSoundPlayer sp; CTCDParticleEmitter p0, p2; p0.Id = 0; p2.Id = 2;
    e0.Iface[0x4b] = &p0; e2.Iface[0x4b] = &p2; e1.Dead = true; hero.Iface[0x10] = &sp;
    CTCHealLife h; h.ParentThing = &hero; h.LoopingSound = 9; h.HealingEffects.Count = 4;
    h.HealingEffects.Items[0].Item = &e0; h.HealingEffects.Items[1].Item = &e1; h.HealingEffects.Items[2].Item = &e2;
    int ok = 1;
    h.FinalCleanup(); ok &= Check("TimelineEnd(0);TimelineEnd(2);StopSound(9);");
    h.HealingEffects.Count = 0; hero.Iface[0x10] = 0; h.FinalCleanup(); ok &= Check("");
    if (ok) std::printf("007c4320_TEST PASS\n");
    return ok ? 0 : 1;
}