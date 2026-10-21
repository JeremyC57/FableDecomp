#include <cstdio>
// Behaviour test for NParticleEngine::CParticleEmitter::WriteBinary (0x00AEDAF0): records the stream calls.
struct Stream {
    char kinds[64]; long vals[64]; int n;
    Stream() : n(0) {}
    void Put(char k, long v) { kinds[n] = k; vals[n] = v; ++n; }
    void WriteEBOOL(bool v) { Put('b', v ? 1 : 0); }
    void WriteUBYTE(unsigned char v) { Put('y', v); }
    void WriteLong(long v) { Put('l', v); }
    void WriteULong(unsigned long v) { Put('u', (long)v); }
    void WriteFloat(float v) { Put('f', (long)v); }
    void WriteNullTerminatedString(const char* s) { Put('s', s[0]); }
};
struct CParticleSystem {
    int id;
    void WriteBinary(Stream* out) { out->Put('p', id); }
};
struct CParticleSystemPointer { CParticleSystem* m_p; void* m_count; CParticleSystem* operator->() const { return m_p; } };
struct CParticleEmitter {
    CParticleSystemPointer* ParticleSystems_start;
    CParticleSystemPointer* ParticleSystems_finish;
    unsigned short Priority : 3;
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned char PreWaterEffect : 1;
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;
    unsigned char CalcBoundingSphereOnceOnly : 1;
    const char* Name;
    float MaxSpawnDistance, MaxDrawDistance, FadeOutStart, FadeInStart, FadeInEnd;
    void WriteBinary(Stream* out)
    {
        out->WriteULong(100);
        out->WriteNullTerminatedString(Name);
        out->WriteEBOOL(Emitter2D);
        out->WriteEBOOL(PreWaterEffect);
        out->WriteEBOOL(WaterEffect);
        out->WriteEBOOL(ZBufferWriteable);
        out->WriteEBOOL(ContinuousEmitter);
        out->WriteEBOOL(IsScreenDisplacement);
        out->WriteEBOOL(ReadZBuffer);
        out->WriteFloat(MaxSpawnDistance);
        out->WriteFloat(MaxDrawDistance);
        out->WriteFloat(FadeOutStart);
        out->WriteFloat(FadeInEnd);
        out->WriteFloat(FadeInStart);
        out->WriteLong(Priority);
        out->WriteEBOOL(DieIfOffsceen);
        out->WriteEBOOL(OffscreenUpdate);
        out->WriteEBOOL(ClipEffectToWeatherMask);
        out->WriteEBOOL(EnableDithering);
        out->WriteEBOOL(CalcBoundingSphereOnceOnly);
        out->WriteULong(ParticleSystems_finish - ParticleSystems_start);
        for (CParticleSystemPointer* it = ParticleSystems_start; it != ParticleSystems_finish; ++it) {
            (*it)->WriteBinary(out);
            out->WriteUBYTE(0x26);
        }
    }
};
int main()
{
    CParticleSystem a = { 41 }, b = { 42 };
    CParticleSystemPointer v[2] = { { &a, 0 }, { &b, 0 } };
    CParticleEmitter e;
    e.ParticleSystems_start = v; e.ParticleSystems_finish = v + 2;
    e.Priority = 5; e.IsScreenDisplacement = 1; e.ReadZBuffer = 0; e.ContinuousEmitter = 1; e.Emitter2D = 1;
    e.PreWaterEffect = 0; e.WaterEffect = 1; e.ZBufferWriteable = 0; e.DieIfOffsceen = 1; e.OffscreenUpdate = 0;
    e.ClipEffectToWeatherMask = 1; e.EnableDithering = 0; e.CalcBoundingSphereOnceOnly = 1;
    e.Name = "N"; e.MaxSpawnDistance = 1; e.MaxDrawDistance = 2; e.FadeOutStart = 3; e.FadeInStart = 4; e.FadeInEnd = 5;
    Stream s;
    e.WriteBinary(&s);
    const char* kinds = "usbbbbbbbffffflbbbbbupypy";
    long vals[] = { 100, 'N', 1, 0, 1, 0, 1, 1, 0, 1, 2, 3, 5, 4, 5, 1, 0, 1, 0, 1, 2, 41, 0x26, 42, 0x26 };
    int bad = s.n != 25;
    for (int i = 0; i < 25 && !bad; ++i) bad |= s.kinds[i] != kinds[i] || s.vals[i] != vals[i];
    if (bad) { std::printf("00aedaf0_TEST FAIL\n"); return 1; }
    std::printf("00aedaf0_TEST PASS\n");
    return 0;
}
