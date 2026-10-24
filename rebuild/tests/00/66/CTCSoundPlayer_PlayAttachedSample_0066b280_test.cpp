// Behaviour test for 0x0066B280 CTCSoundPlayer::PlayAttachedSample (model copy)
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

static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
struct Rng { unsigned int Seed; unsigned int Random(unsigned int n) { Seed = Ror13(Seed * 0x24a1 + 0x24df); return n ? Seed % n : 0; } };
static Rng g_rng = { 99 };
struct Pair { int FirstSound, LastSound; };
struct Player {
    Thing* ParentThing; int lastSample; Bank* lastBank;
    unsigned int InternalPlayAttachedSample(Bank& bank, int sample, const Init&, bool, bool, bool) { lastBank = &bank; lastSample = sample; return 1; }
    unsigned int PlayAttachedSample(const Pair& sound, const Init& init, bool flag, bool loop, bool attached)
    {
        Rng& rng = g_rng;
        int sample = sound.FirstSound + rng.Random(sound.LastSound - sound.FirstSound + 1);
        Bank& bank = ParentThing->GetSampleBank();
        return InternalPlayAttachedSample(bank, sample, init, flag, loop, attached);
    }
};
int main()
{
    Thing t = { 0 }; Player p = { &t, 0, 0 }; Pair pr = { 10, 13 }; Init init;
    bool ok = true; bool seen[4] = { false, false, false, false };
    for (int k = 0; k < 64; ++k) { p.PlayAttachedSample(pr, init, false, false, false); ok = ok && p.lastSample >= 10 && p.lastSample <= 13; if (ok) seen[p.lastSample - 10] = true; }
    ok = ok && p.lastBank == &g_sampleBank && seen[0] && seen[3];
    Pair one = { 5, 5 }; p.PlayAttachedSample(one, init, false, false, false); ok = ok && p.lastSample == 5;
    std::printf(ok ? "0066b280_TEST PASS\n" : "0066b280_TEST FAIL\n");
    return ok ? 0 : 1;
}