// Behaviour test (0x006dbe00): CPersistTraits<CComboMultiplierData>::TransferBinaryIn reads two floats, the entry count, then count (attribute, multiplier) pairs inserted into the map (an existing key keeps its value)
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Memory input stream model: the chunk pointer/length fast path of CMemoryDataInputStream::Read and a
// ReadWithSrcChunkOverflow that serves the rest from the backing buffer (counted, so the slow path is checked).
struct TestStream {
    unsigned int         position;
    int                  sourceLength;
    const unsigned char* chunkBase;
    int                  chunkPosition;
    int                  chunkLength;
    const unsigned char* data;
    int                  overflowCalls;
    void Init(const unsigned char* bytes, int size, int chunk)
    {
        position = 0; sourceLength = size; chunkBase = bytes; chunkPosition = 0; chunkLength = chunk; data = bytes; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++overflowCalls;
        std::memcpy(buffer, data + position, size);
        position += size;
        chunkBase = data + position;
        chunkLength = sourceLength - (int)position;
    }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        i64 newPosition = (i64)(unsigned int)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void PutLong(unsigned char* p, int v) { std::memcpy(p, &v, 4); }
static void PutFloat(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
struct CMultiplierMap {                         // map<EDamageAttribute, float> stand-in with std::map insert semantics
    int Keys[8]; float Values[8]; int Size;
    void insert(int key, float value) { for (int i = 0; i < Size; ++i) if (Keys[i] == key) return; Keys[Size] = key; Values[Size] = value; ++Size; }
    float Get(int key) const { for (int i = 0; i < Size; ++i) if (Keys[i] == key) return Values[i]; return -1.0f; }
};
struct CComboMultiplierData { CMultiplierMap ComboMultiplier; float ExperienceMultiplierNumSecondsForAttenuation; float ExperienceMultiplierParamDecayDeltaPerSecond; };
static void ReadFloat(TestStream* s, float* v) { float f; s->Read(&f, 4); *v = f; }
static void TransferBinaryIn(TestStream* stream, CComboMultiplierData* value)
{
    ReadFloat(stream, &value->ExperienceMultiplierNumSecondsForAttenuation);
    ReadFloat(stream, &value->ExperienceMultiplierParamDecayDeltaPerSecond);
    unsigned int count;
    { unsigned int n; stream->Read(&n, 4); count = n; }
    for (; count != 0; --count) {
        int attribute; { int n; stream->Read(&n, 4); attribute = n; }
        float multiplier; ReadFloat(stream, &multiplier);
        value->ComboMultiplier.insert(attribute, multiplier);
    }
}
int main()
{
    unsigned char bytes[36];
    PutFloat(bytes, 3.0f); PutFloat(bytes + 4, 0.5f); PutLong(bytes + 8, 3);
    PutLong(bytes + 12, 2); PutFloat(bytes + 16, 1.5f);
    PutLong(bytes + 20, 7); PutFloat(bytes + 24, 2.5f);
    PutLong(bytes + 28, 2); PutFloat(bytes + 32, 9.0f);     // duplicate key: insert keeps 1.5
    TestStream s; s.Init(bytes, 36, 36);
    CComboMultiplierData d; d.ComboMultiplier.Size = 0;
    TransferBinaryIn(&s, &d);
    if (std::fabs(d.ExperienceMultiplierNumSecondsForAttenuation - 3.0f) > 1e-6f || std::fabs(d.ExperienceMultiplierParamDecayDeltaPerSecond - 0.5f) > 1e-6f
        || d.ComboMultiplier.Size != 2 || std::fabs(d.ComboMultiplier.Get(2) - 1.5f) > 1e-6f || std::fabs(d.ComboMultiplier.Get(7) - 2.5f) > 1e-6f
        || s.position != 36) { std::printf("006dbe00_TEST FAIL\n"); return 1; }
    std::printf("006dbe00_TEST PASS\n");
    return 0;
}