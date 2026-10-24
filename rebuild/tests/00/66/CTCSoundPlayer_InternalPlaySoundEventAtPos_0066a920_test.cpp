// Behaviour test for 0x0066A920 CTCSoundPlayer::InternalPlaySoundEventAtPos (model copy)
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

unsigned int PlayAtPos(Bank& bank, const Vec3& pos, const Name& name, bool loop, bool attached)
{
    if (g_pGameState) {
        Emitter* e = Create(pos, 0, true);
        if (e && bank.IsSoundEventValid(name)) { Init init; init.GainFaderBits = 0x8010; return e->PlaySoundEvent(bank, name, loop, init, attached); }
    }
    return g_Invalid;
}
int main()
{
    Bank bank = { true }; Vec3 pos = { 1, 2, 3 }; Name n = { "EV" };
    bool ok = PlayAtPos(bank, pos, n, true, false) == 200 && g_emitter.parent == 0 && g_emitter.flag && g_emitter.pos.Y == 2
        && g_emitter.init.GainFaderBits == 0x8010 && g_emitter.init.OverrideID == -1 && g_emitter.init.PlaybackType == 1 && g_emitter.loop && !g_emitter.attached;
    bank.hasEvent = false; ok = ok && PlayAtPos(bank, pos, n, true, false) == g_Invalid;
    bank.hasEvent = true; g_pGameState = 0; ok = ok && PlayAtPos(bank, pos, n, true, false) == g_Invalid;
    g_pGameState = &bank; g_emitterOk = false; ok = ok && PlayAtPos(bank, pos, n, true, false) == g_Invalid;
    std::printf(ok ? "0066a920_TEST PASS\n" : "0066a920_TEST FAIL\n");
    return ok ? 0 : 1;
}