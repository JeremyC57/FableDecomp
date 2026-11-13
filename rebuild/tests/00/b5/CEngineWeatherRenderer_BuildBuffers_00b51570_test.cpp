// Behaviour test for 0x00B51570 CEngineWeatherRenderer::BuildBuffers: a host model of the row (same statements on stand-in
// buffers) checked against an independent reference of the seed sequence, the quad UV corners, the five constant sets and the
// index lists. Plain C++98 (g++ and VC7.1).
#include <cstdio>
#include <cmath>
#include <cstring>

static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static long GFROR13(unsigned int value) { return (long)Ror13(value); }
static float GFFloatRandom(float max, unsigned int* seed)
{
    double range = max;
    range *= 65536.0;
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = (unsigned int)GFROR13(*seed);
    if (range > 0.0001)
    {
        double result = std::fmod((double)*seed, range);
        result *= (1.0 / 65536.0);
        return (float)result;
    }
    return 0.0f;
}

struct Vertex { float X, Y, Z, U1, V1; unsigned short ConstantIndex, Pad;
    void SetU1V1(float u, float v) { U1 = u; V1 = v; } };

struct VertexBuffer {
    unsigned char Data[1024 * 24]; long Length; long Stride; int Locks;
    void Uninitialise() { Length = 0; }
    bool Initialise(int, long count, long stride, unsigned long, int) { Length = count * stride; Stride = stride; std::memset(Data, 0xcd, sizeof(Data)); return true; }
    long GetSize() const { return Length; }
    unsigned char* Lock(unsigned long, unsigned long, unsigned long) { ++Locks; return Data; }
    void Unlock() { --Locks; }
};
struct IndexBuffer {
    unsigned char Data[0x600]; long ByteLength; int Locks;
    void Uninitialise() { ByteLength = 0; }
    void Initialise(unsigned long bytes, unsigned long, int, int, int) { ByteLength = (long)bytes; std::memset(Data, 0xcd, sizeof(Data)); }
    long GetSize() const { return ByteLength; }
    unsigned char* Lock(unsigned long, unsigned long, unsigned long) { ++Locks; return Data; }
    void Unlock() { --Locks; }
};

static void SetIndex(unsigned short* indices, long n, unsigned short value) { indices[n] = value; }

struct Weather {
    VertexBuffer RainVertexBuffer; IndexBuffer RainIndexBuffer; VertexBuffer SnowVertexBuffer; VertexBuffer MistVertexBuffer;
    IndexBuffer MistIndexBuffer; unsigned int Seed;
    void BuildBuffers()
    {
        RainVertexBuffer.Uninitialise();
        RainVertexBuffer.Initialise(0, 512, sizeof(Vertex), 0, 1);
        Vertex* rain = (Vertex*)RainVertexBuffer.Lock(0, 0, RainVertexBuffer.GetSize());
        long i = 0;
        for (long v = 0; v < 512; v += 4, ++i)
        {
            rain[v + 0].X = rain[v + 1].X = rain[v + 2].X = rain[v + 3].X = GFFloatRandom(1.0f, &Seed) - 0.5f;
            rain[v + 0].Y = rain[v + 1].Y = rain[v + 2].Y = rain[v + 3].Y = GFFloatRandom(1.0f, &Seed) - 0.5f;
            rain[v + 0].Z = rain[v + 1].Z = rain[v + 2].Z = rain[v + 3].Z = GFFloatRandom(1.0f, &Seed) - 0.5f;
            rain[v + 0].ConstantIndex = rain[v + 1].ConstantIndex = rain[v + 2].ConstantIndex = rain[v + 3].ConstantIndex = (unsigned short)(i % 5);
            rain[v + 0].U1 = 0.0f; rain[v + 0].V1 = 0.0f;
            rain[v + 1].U1 = 0.0f; rain[v + 1].V1 = 1.0f;
            rain[v + 2].U1 = 1.0f; rain[v + 2].V1 = 1.0f;
            rain[v + 3].U1 = 1.0f; rain[v + 3].V1 = 0.0f;
        }
        RainVertexBuffer.Unlock();
        RainIndexBuffer.Uninitialise();
        RainIndexBuffer.Initialise(0x600, 0, 0x65, 1, 4);
        unsigned short* index = (unsigned short*)RainIndexBuffer.Lock(0, 0, RainIndexBuffer.GetSize());
        for (long v = 0; v < 512; v += 4)
        {
            *index++ = (unsigned short)v; *index++ = (unsigned short)(v + 1); *index++ = (unsigned short)(v + 2);
            *index++ = (unsigned short)v; *index++ = (unsigned short)(v + 2); *index++ = (unsigned short)(v + 3);
        }
        RainIndexBuffer.Unlock();
        SnowVertexBuffer.Uninitialise();
        SnowVertexBuffer.Initialise(0, 1024, sizeof(Vertex), 0, 1);
        Vertex* snow = (Vertex*)SnowVertexBuffer.Lock(0, 0, SnowVertexBuffer.GetSize());
        for (long j = 0; j < 1024; ++j)
        {
            snow[j].X = GFFloatRandom(2.0f, &Seed) - 1.0f;
            snow[j].Y = GFFloatRandom(2.0f, &Seed) - 1.0f;
            snow[j].Z = GFFloatRandom(2.0f, &Seed) - 1.0f;
            snow[j].ConstantIndex = (unsigned short)(j % 5);
            float u = GFFloatRandom(1.0f, &Seed) - 0.5f;
            float v = GFFloatRandom(1.0f, &Seed) - 0.5f;
            snow[j].SetU1V1(u, v);
        }
        SnowVertexBuffer.Unlock();
        MistVertexBuffer.Uninitialise();
        MistVertexBuffer.Initialise(0, 16, sizeof(Vertex), 0, 1);
        Vertex* mist = (Vertex*)MistVertexBuffer.Lock(0, 0, MistVertexBuffer.GetSize());
        for (long k = 0; k < 4; ++k)
        {
            mist->X = -1.0f; mist->Y = 1.0f; mist->Z = 1.0f; mist->U1 = 1.0f; mist->V1 = 0.0f; ++mist;
            mist->X = 1.0f; mist->Y = 1.0f; mist->Z = 1.0f; mist->U1 = 0.0f; mist->V1 = 0.0f; ++mist;
            mist->X = 1.0f; mist->Y = -1.0f; mist->Z = 1.0f; mist->U1 = 0.0f; mist->V1 = 1.0f; ++mist;
            mist->X = -1.0f; mist->Y = -1.0f; mist->Z = 1.0f; mist->U1 = 1.0f; mist->V1 = 1.0f; ++mist;
        }
        MistVertexBuffer.Unlock();
        MistIndexBuffer.Uninitialise();
        MistIndexBuffer.Initialise(0x30, 0, 0x65, 1, 4);
        unsigned short* mistIndex = (unsigned short*)MistIndexBuffer.Lock(0, 0, MistIndexBuffer.GetSize());
        static const unsigned short values[24] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15 };
        for (long n = 0; n < 24; ++n) SetIndex(mistIndex, n, values[n]);
        MistIndexBuffer.Unlock();
    }
};

static int fail(int line) { std::printf("00b51570_TEST FAIL (line %d)\n", line); return 1; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-6f; }

int main()
{
    static Weather w;
    std::memset(&w, 0, sizeof(w));
    w.Seed = 12345;
    w.BuildBuffers();
    // reference seed walk
    unsigned int seed = 12345;
    const Vertex* rain = (const Vertex*)w.RainVertexBuffer.Data;
    if (w.RainVertexBuffer.Length != 512 * 24 || w.RainVertexBuffer.Locks != 0) return fail(__LINE__);
    for (int q = 0; q < 128; ++q)
    {
        float r[3];
        for (int c = 0; c < 3; ++c) { seed = seed * 0x24a1 + 0x24df; seed = Ror13(seed); r[c] = (float)(std::fmod((double)seed, 65536.0) / 65536.0) - 0.5f; }
        for (int k = 0; k < 4; ++k)
        {
            const Vertex& v = rain[q * 4 + k];
            if (!near(v.X, r[0]) || !near(v.Y, r[1]) || !near(v.Z, r[2])) return fail(__LINE__);
            if (v.X < -0.5f || v.X >= 0.5f) return fail(__LINE__);
            if (v.ConstantIndex != q % 5) return fail(__LINE__);
        }
        if (rain[q*4].U1 != 0.0f || rain[q*4].V1 != 0.0f || rain[q*4+1].U1 != 0.0f || rain[q*4+1].V1 != 1.0f) return fail(__LINE__);
        if (rain[q*4+2].U1 != 1.0f || rain[q*4+2].V1 != 1.0f || rain[q*4+3].U1 != 1.0f || rain[q*4+3].V1 != 0.0f) return fail(__LINE__);
    }
    const unsigned short* ri = (const unsigned short*)w.RainIndexBuffer.Data;
    for (int q = 0; q < 128; ++q)
    {
        int b = q * 4;
        if (ri[q*6] != b || ri[q*6+1] != b+1 || ri[q*6+2] != b+2 || ri[q*6+3] != b || ri[q*6+4] != b+2 || ri[q*6+5] != b+3) return fail(__LINE__);
    }
    const Vertex* snow = (const Vertex*)w.SnowVertexBuffer.Data;
    for (int j = 0; j < 1024; ++j)
    {
        float r[5];
        for (int c = 0; c < 5; ++c)
        {
            seed = seed * 0x24a1 + 0x24df; seed = Ror13(seed);
            double range = c < 3 ? 131072.0 : 65536.0;
            r[c] = (float)(std::fmod((double)seed, range) / 65536.0) - (c < 3 ? 1.0f : 0.5f);
        }
        if (!near(snow[j].X, r[0]) || !near(snow[j].Y, r[1]) || !near(snow[j].Z, r[2])) return fail(__LINE__);
        if (!near(snow[j].U1, r[3]) || !near(snow[j].V1, r[4])) return fail(__LINE__);
        if (snow[j].ConstantIndex != j % 5) return fail(__LINE__);
        if (snow[j].X < -1.0f || snow[j].X >= 1.0f) return fail(__LINE__);
    }
    if (w.Seed != seed) return fail(__LINE__);
    const Vertex* mist = (const Vertex*)w.MistVertexBuffer.Data;
    static const float expect[4][5] = { {-1, 1, 1, 1, 0}, {1, 1, 1, 0, 0}, {1, -1, 1, 0, 1}, {-1, -1, 1, 1, 1} };
    for (int m = 0; m < 16; ++m)
    {
        const float* e = expect[m % 4];
        if (mist[m].X != e[0] || mist[m].Y != e[1] || mist[m].Z != e[2] || mist[m].U1 != e[3] || mist[m].V1 != e[4]) return fail(__LINE__);
    }
    const unsigned short* mi = (const unsigned short*)w.MistIndexBuffer.Data;
    for (int q = 0; q < 4; ++q)
    {
        int b = q * 4;
        if (mi[q*6] != b || mi[q*6+1] != b+1 || mi[q*6+2] != b+2 || mi[q*6+3] != b || mi[q*6+4] != b+2 || mi[q*6+5] != b+3) return fail(__LINE__);
    }
    if (w.MistIndexBuffer.ByteLength != 0x30 || w.RainIndexBuffer.ByteLength != 0x600) return fail(__LINE__);
    // GFFloatRandom of a non-positive range returns 0 and still steps the seed
    unsigned int s2 = 7; float z = GFFloatRandom(0.0f, &s2);
    if (z != 0.0f || s2 != Ror13(7u * 0x24a1 + 0x24df)) return fail(__LINE__);
    std::printf("00b51570_TEST PASS\n");
    return 0;
}