// Behaviour test for CDecodedStream::Initialise 0x00C54510: effect-initialise failure returns false and touches nothing;
// success copies the default source properties, zeroes both pans, sets room -10000 / LF 0 / HF 0 with their dirty bits,
// runs the sixteen clamped setters in retail order with retail values, then configures the effect from the block (mode 1).
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct Format { long size0, size1, rate, channels; Format() : size0(0), size1(0), rate(48000), channels(2) {} };
struct SourceProps { Vec Position, Velocity; long levels[3]; float values[18]; };
static const Vec g_zero = { 0.0f, 0.0f, 0.0f };
static SourceProps g_defaults;

struct Params {
    unsigned long DirtyFlags; long m_room, m_roomHF, m_roomLF; float m_values[15]; Vec m_reflectionsPan, m_reverbPan; float m_size;
    void SetReflectionsPan(const Vec& v) { m_reflectionsPan = v; DirtyFlags |= 0x800; }
    void SetReverbPan(const Vec& v) { m_reverbPan = v; DirtyFlags |= 0x4000; }
    void SetRoom(long v) { m_room = v; DirtyFlags |= 0x20; }
    void SetRoomHF(long v) { m_roomHF = v; DirtyFlags |= 0x40; }
    void SetRoomLF(long v) { m_roomLF = v; DirtyFlags |= 0x80; }
};
static int g_order[16]; static float g_vals[16]; static int g_n;
static void Clamped(int id, float v) { if (g_n < 16) { g_order[g_n] = id; g_vals[g_n] = v; } ++g_n; }
struct Effect {
    bool initResult; long gotRate, gotChannels; const Params* configured; long mode;
    Effect() : initResult(true), gotRate(0), gotChannels(0), configured(0), mode(-1) {}
    bool Initialise(Format* f) { gotRate = f->rate; gotChannels = f->channels; return initResult; }
    void Configure(const Params* p, long m) { configured = p; mode = m; }
};
struct Stream {
    SourceProps m_properties; Params m_pending; Effect m_effect;
    Params* GetPendingParameters() { return &m_pending; }
    bool Initialise()
    {
        Format format; format.rate = 44100; format.channels = 4;
        if (!m_effect.Initialise(&format)) return false;
        m_properties = g_defaults;
        GetPendingParameters()->SetReflectionsPan(g_zero);
        GetPendingParameters()->SetReverbPan(g_zero);
        GetPendingParameters()->SetRoom(-10000);
        GetPendingParameters()->SetRoomLF(0);
        GetPendingParameters()->SetRoomHF(0);
        Clamped(0x10, 1.0f); Clamped(0x18, 0.5f); Clamped(0x14, 1.0f); Clamped(0x24, 0.0f);
        Clamped(0x28, 0.02f); Clamped(0x2c, 0.0f); Clamped(0x30, 0.04f); Clamped(0x64, 7.5f);
        Clamped(0x34, 1.0f); Clamped(0x38, 1.0f); Clamped(0x3c, 0.075f); Clamped(0x40, 0.0f);
        Clamped(0x44, 0.04f); Clamped(0x48, 0.0f); Clamped(0x1c, 200.0f); Clamped(0x20, 5000.0f);
        m_effect.Configure(&m_pending, 1);
        return true;
    }
};
int main()
{
    bool ok = true;
    std::memset(&g_defaults, 0, sizeof g_defaults); g_defaults.Position.X = 3.0f; g_defaults.levels[1] = -500; g_defaults.values[17] = 2.5f;
    {
        Stream s; std::memset(&s.m_properties, 0x11, sizeof s.m_properties); std::memset(&s.m_pending, 0x22, sizeof s.m_pending);
        s.m_effect.initResult = false; g_n = 0;
        unsigned char before[sizeof(SourceProps)]; std::memcpy(before, &s.m_properties, sizeof before);
        ok = ok && !s.Initialise() && s.m_effect.gotRate == 44100 && s.m_effect.gotChannels == 4 && g_n == 0
                && std::memcmp(before, &s.m_properties, sizeof before) == 0 && s.m_effect.configured == 0;
    }
    {
        Stream s; std::memset(&s.m_pending, 0, sizeof s.m_pending); s.m_pending.DirtyFlags = 0x1; s.m_pending.m_reverbPan.Y = 9.0f;
        s.m_pending.m_room = 5; g_n = 0;
        ok = ok && s.Initialise() && std::memcmp(&s.m_properties, &g_defaults, sizeof g_defaults) == 0
                && s.m_pending.m_reverbPan.Y == 0.0f && s.m_pending.m_room == -10000 && s.m_pending.m_roomLF == 0 && s.m_pending.m_roomHF == 0
                && s.m_pending.DirtyFlags == (0x1u | 0x800 | 0x4000 | 0x20 | 0x80 | 0x40)
                && g_n == 16 && g_order[0] == 0x10 && g_order[7] == 0x64 && g_vals[7] == 7.5f && g_order[15] == 0x20 && g_vals[15] == 5000.0f
                && s.m_effect.configured == &s.m_pending && s.m_effect.mode == 1;
    }
    if (ok) std::printf("00c54510_TEST PASS\n"); else std::printf("00c54510_TEST FAIL\n");
    return ok ? 0 : 1;
}