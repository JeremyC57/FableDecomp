// Behaviour test for NParticleEngine::CParticleEmitter::ReadBinary (0x00AEFAA0): a standalone model of the read in
// CParticleEmitter::WriteBinary 0x00AEDAF0 order -- version long (ignored), name, seven EBOOL flags, five distances (FadeInEnd
// before FadeInStart), priority (low three bits), five more flags, the system count, then per system its own record (modelled
// as an enabled byte) and one separator byte; only enabled systems are kept, with the emitter as their parent.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
struct Stream {
    std::vector<unsigned char> data; unsigned int pos;
    Stream() : pos(0) {}
    void Read(void* out, int size) { if (size <= 0 || pos + size > data.size()) return; memcpy(out, &data[pos], size); pos += size; }
    unsigned int ReadULONG() { unsigned int v = 0; Read(&v, 4); return v; }
    int ReadSLONG() { int v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadEBOOL() { unsigned char v = 0; Read(&v, 1); return v != 0; }
    std::string ReadString() { std::string s; while (pos < data.size() && data[pos]) s += (char)data[pos++]; ++pos; return s; }
    void PutU(unsigned int u) { for (int i = 0; i < 4; ++i) data.push_back((unsigned char)(u >> (8 * i))); }
    void PutF(float f) { unsigned int u; memcpy(&u, &f, 4); PutU(u); }
    void PutB(unsigned char b) { data.push_back(b); }
    void PutS(const char* s) { for (size_t i = 0; i <= strlen(s); ++i) data.push_back((unsigned char)s[i]); }
};
struct Emitter;
struct System { const Emitter* parent; bool enabled; int id; void ReadBinary(Stream& in) { enabled = in.ReadEBOOL(); id = in.ReadSLONG(); } };
struct Emitter {
    unsigned short Priority : 3, IsScreenDisplacement : 1, ReadZBuffer : 1, Dead : 1, ParticleCreationDisabled : 1,
        ContinuousEmitter : 1, Initialised : 1, WaitingToSleep : 1, Sleeping : 1, Emitter2D : 1;
    unsigned char PreWaterEffect : 1, WaterEffect : 1, ZBufferWriteable : 1, Visible : 1, AlwaysUpdate : 1, DieIfOffsceen : 1,
        OffscreenUpdate : 1, ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1, CalcBoundingSphereOnceOnly : 1;
    std::string Name; float MaxSpawnDistance, MaxDrawDistance, FadeOutStart, FadeInStart, FadeInEnd;
    std::vector<System> ParticleSystems;
    void ReadBinary(Stream& in)
    {
        in.ReadULONG();
        Name = in.ReadString();
        Emitter2D = in.ReadEBOOL(); PreWaterEffect = in.ReadEBOOL(); WaterEffect = in.ReadEBOOL();
        ZBufferWriteable = in.ReadEBOOL(); ContinuousEmitter = in.ReadEBOOL(); IsScreenDisplacement = in.ReadEBOOL();
        ReadZBuffer = in.ReadEBOOL();
        MaxSpawnDistance = in.ReadFloat(); MaxDrawDistance = in.ReadFloat(); FadeOutStart = in.ReadFloat();
        FadeInEnd = in.ReadFloat(); FadeInStart = in.ReadFloat();
        Priority = in.ReadSLONG();
        DieIfOffsceen = in.ReadEBOOL(); OffscreenUpdate = in.ReadEBOOL(); ClipEffectToWeatherMask = in.ReadEBOOL();
        EnableDithering = in.ReadEBOOL(); CalcBoundingSphereOnceOnly = in.ReadEBOOL();
        unsigned int count = in.ReadULONG();
        ParticleSystems.reserve(count);
        for (unsigned int i = 0; i < count; ++i) {
            System s; s.parent = this; s.ReadBinary(in);
            if (s.enabled) ParticleSystems.push_back(s);
            in.ReadEBOOL();
        }
    }
};
int main()
{
    Stream s;
    s.PutU(100); s.PutS("Smoke");
    const unsigned char f1[7] = { 1, 0, 1, 1, 0, 1, 0 };
    for (int i = 0; i < 7; ++i) s.PutB(f1[i]);
    s.PutF(10.0f); s.PutF(20.0f); s.PutF(15.0f); s.PutF(3.0f); s.PutF(1.0f);
    s.PutU(0xFFFFFFFDu);                                   // priority: only the low three bits survive (5)
    const unsigned char f2[5] = { 0, 1, 1, 0, 1 };
    for (int i = 0; i < 5; ++i) s.PutB(f2[i]);
    s.PutU(3);
    for (int i = 0; i < 3; ++i) { s.PutB(i != 1); s.PutU(40 + i); s.PutB(0x26); }
    Emitter e; memset((void*)&e, 0, 4); e.ReadBinary(s);
    bool ok = e.Name == "Smoke" && e.Emitter2D == 1 && e.PreWaterEffect == 0 && e.WaterEffect == 1 && e.ZBufferWriteable == 1
        && e.ContinuousEmitter == 0 && e.IsScreenDisplacement == 1 && e.ReadZBuffer == 0
        && std::fabs(e.MaxSpawnDistance - 10.0f) < 1e-6f && std::fabs(e.MaxDrawDistance - 20.0f) < 1e-6f
        && std::fabs(e.FadeOutStart - 15.0f) < 1e-6f && std::fabs(e.FadeInEnd - 3.0f) < 1e-6f
        && std::fabs(e.FadeInStart - 1.0f) < 1e-6f && e.Priority == 5
        && e.DieIfOffsceen == 0 && e.OffscreenUpdate == 1 && e.ClipEffectToWeatherMask == 1 && e.EnableDithering == 0
        && e.CalcBoundingSphereOnceOnly == 1
        && e.ParticleSystems.size() == 2 && e.ParticleSystems[0].id == 40 && e.ParticleSystems[1].id == 42
        && e.ParticleSystems[0].parent == &e && s.pos == s.data.size();
    printf(ok ? "00aefaa0_TEST PASS\n" : "00aefaa0_TEST FAIL\n");
    return ok ? 0 : 1;
}