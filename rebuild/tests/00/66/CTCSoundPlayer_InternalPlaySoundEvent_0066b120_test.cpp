// Behaviour test for 0x0066B120 CTCSoundPlayer::InternalPlaySoundEvent (model copy)
#include <cstdio>
struct Name { const char* s; };
struct Vec3 { float X, Y, Z; };
struct Init {
    float Volume, Pitch, Pan; int NoPlays, PlaybackType; unsigned int GainFaderBits, PitchFaderBits; int OverrideID;
    Init() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1) { PlaybackType = 1; GainFaderBits = 0; }
};
struct Bank { bool hasEvent; bool IsSoundEventValid(const Name&) { return hasEvent; } };
struct Thing;
struct Emitter {
    Bank* bank; int sample; const char* name; bool loop, attached; Init init; const Thing* parent; Vec3 pos; bool flag;
    unsigned int PlaySample(Bank& b, int s, bool l, const Init& i, bool a) { bank = &b; sample = s; loop = l; init = i; attached = a; return 100 + s; }
    unsigned int PlaySoundEvent(Bank& b, const Name& n, bool l, Init& i, bool a) { bank = &b; name = n.s; loop = l; init = i; attached = a; return 200; }
};
static Emitter g_emitter;
static bool g_emitterOk = true;
static Emitter* Create(const Vec3& pos, const Thing* parent, bool flag)
{ if (!g_emitterOk) return 0; g_emitter.pos = pos; g_emitter.parent = parent; g_emitter.flag = flag; return &g_emitter; }
static Bank g_sampleBank = { true };
struct Physics { Vec3 Position; };
struct Thing { Physics* PhysicsTC; Bank& GetSampleBank() const { return g_sampleBank; } };
static void* g_pGameState = &g_emitter;
static const unsigned int g_Invalid = 0xFFFFFFFF;

struct Player {
    bool lastFlag;
    Emitter* CreatePAttachedEmitter(bool flag) { lastFlag = flag; return g_emitterOk ? &g_emitter : 0; }
    unsigned int InternalPlaySoundEvent(Bank& bank, const Name& name, bool flag, bool loop, bool attached, int overrideID)
    {
        if (g_pGameState) {
            Emitter* e = CreatePAttachedEmitter(flag);
            if (e && bank.IsSoundEventValid(name)) { Init init; init.GainFaderBits = 0x8010; init.OverrideID = overrideID; return e->PlaySoundEvent(bank, name, loop, init, attached); }
        }
        return g_Invalid;
    }
};
int main()
{
    Player p = { false }; Bank bank = { true }; Name n = { "X" };
    bool ok = p.InternalPlaySoundEvent(bank, n, true, false, true, 42) == 200 && p.lastFlag && g_emitter.init.OverrideID == 42 && g_emitter.init.GainFaderBits == 0x8010 && g_emitter.attached && g_emitter.name == n.s;
    bank.hasEvent = false; ok = ok && p.InternalPlaySoundEvent(bank, n, true, false, true, 42) == g_Invalid;
    std::printf(ok ? "0066b120_TEST PASS\n" : "0066b120_TEST FAIL\n");
    return ok ? 0 : 1;
}