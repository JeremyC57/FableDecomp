// Behaviour test for 0x0067E720 GFSerialiseVectorBinaryOut<CDegradableInfo>: the count goes out first, then per record
// the float, the 14 raw engine-graphic bytes, the emitter, the flag and the 0x01AB6CE3 check word, in that order.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif

struct CDataOutputStreamWriter {                            // records every write as (kind, value)
    int  Kind[64];
    int  Value[64];
    unsigned char Raw[64];
    int  RawCount;
    int  Count;
    virtual ~CDataOutputStreamWriter() {}
    virtual void Write(const void* data, int size)          // models virtual slot 4
    {
        Kind[Count] = 'W'; Value[Count++] = size;
        std::memcpy(Raw + RawCount, data, size); RawCount += size;
    }
    void WriteSLONG(int value) { Kind[Count] = 'L'; Value[Count++] = value; }          // 0x00993EB0
    void WriteFloat(float value) { Kind[Count] = 'F'; Value[Count++] = (int)(value * 100.0f); }   // 0x00993EE0
    void WriteEBOOL(bool value) { Kind[Count] = 'B'; Value[Count++] = value ? 1 : 0; }  // 0x00993E30
};

#pragma pack(push, 1)
class CEngineGraphic {
public:
    unsigned int  BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)

class CDegradableInfo {
public:
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    int            SmashParticleEmitter;
    bool           BlocksNavigation;
};

namespace nav {
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

const int DEGRADABLE_INFO_RECORD_CHECK = 0x01AB6CE3;

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDegradableInfo> {
    CDegradableInfo DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CDegradableInfo* value) const
    {
        stream.WriteFloat(value->HealthPercentage);
        stream.Write(&value->EngineGraphic, sizeof(CEngineGraphic));
        stream.WriteSLONG(value->SmashParticleEmitter);
        stream.WriteEBOOL(value->BlocksNavigation);
        stream.WriteSLONG(DEGRADABLE_INFO_RECORD_CHECK);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, nav::vector<T>& value)
{
    CPersistTraits<T> traits;
    int count = value.size();
    stream.WriteSLONG(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

int main()
{
    if (sizeof(CEngineGraphic) != 14 || sizeof(CDegradableInfo) != 28) { std::printf("0067e720_TEST FAIL\n"); return 1; }
    CDegradableInfo d[2];
    std::memset(d, 0, sizeof(d));
    d[0].HealthPercentage = 0.5f; d[0].EngineGraphic.BankIndex = 7; d[0].EngineGraphic.Type = 3;
    d[0].SmashParticleEmitter = 11; d[0].BlocksNavigation = true;
    d[1].HealthPercentage = 0.25f; d[1].EngineGraphic.BankIndex = 9; d[1].SmashParticleEmitter = -2;
    nav::vector<CDegradableInfo> v;
    v._M_start = d; v._M_finish = d + 2; v._M_end_of_storage = d + 2;
    CDataOutputStreamWriter s;
    s.Count = 0; s.RawCount = 0;
    GFSerialiseVectorBinaryOut<CDegradableInfo>(s, v);
    const char kinds[] = "LFWLBLFWLBL";
    if (s.Count != 11 || s.RawCount != 28) { std::printf("0067e720_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 11; ++i) if (s.Kind[i] != kinds[i]) { std::printf("0067e720_TEST FAIL\n"); return 1; }
    if (s.Value[0] != 2 || s.Value[1] != 50 || s.Value[2] != 14 || s.Value[3] != 11 || s.Value[4] != 1 ||
        s.Value[5] != 0x01AB6CE3 || s.Value[6] != 25 || s.Value[8] != -2 || s.Value[9] != 0 || s.Value[10] != 0x01AB6CE3 ||
        s.Raw[0] != 7 || s.Raw[13] != 3 || s.Raw[14] != 9) {
        std::printf("0067e720_TEST FAIL\n"); return 1;
    }
    std::printf("0067e720_TEST PASS\n");
    return 0;
}