// Behaviour test for 0x0067F430 GFSerialiseVectorBinaryIn<CDegradableInfo> (source body copied; stream overflow and
// the out-of-line resize stubbed). 32-bit `long`s of the source are written as int so the g++ x64 build reads 4-byte words.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

class CMemoryDataInputStream { public:
    void* __vftable;
    long  position;
    long  sourceLength;
    long  chunkBase;
    long  chunkPosition;
    long  chunkLength;
    void* StreamSource;
};
static int g_overflow = 0, g_resize = 0, g_zeroDefault = 0;

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size);   // 0x00993CA0
    __forceinline void Read(void* buffer, int size)
    {
        __int64 newPosition = (__int64)(unsigned int)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

#pragma pack(push, 1)
class CEngineGraphic {
public:
    unsigned int  BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)

class CDegradableInfo {
public:
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    int            SmashParticleEmitter;
    bool           BlocksNavigation;
};

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDegradableInfo> {
    CDegradableInfo DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CDegradableInfo* value);
};
void CPersistTraits<CDegradableInfo>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, CDegradableInfo* value)
{
    stream->Read(&value->HealthPercentage, 4);
    stream->Read(&value->EngineGraphic, 14);
    stream->Read(&value->SmashParticleEmitter, 4);
    stream->Read(&value->BlocksNavigation, 1);
}

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, int* out)
{
    int value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    int count;
    ReadCount(stream, &count);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(&stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<CDegradableInfo>(CMemoryDataInputStreamReader& stream, std::vector<CDegradableInfo>& out);

void CMemoryDataInputStreamReader::ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; memset(buffer, 0, size); }
namespace std {
template <> void vector<CDegradableInfo>::resize(size_type n, const CDegradableInfo& x)
{
    ++g_resize;
    if (x.EngineGraphic.BankIndex == 0 && x.EngineGraphic.Type == 0 && !x.EngineGraphic.AdditiveAlpha) ++g_zeroDefault;
    _M_start = (CDegradableInfo*)malloc(n * sizeof(CDegradableInfo) + 1);
    for (size_type i = 0; i < n; ++i) _M_start[i] = x;
    _M_finish = _M_end_of_storage = _M_start + n;
}
}

static unsigned char* Put(unsigned char* p, const void* v, int n) { memcpy(p, v, n); return p + n; }

int main()
{
    unsigned char buf[128];
    unsigned char* p = buf;
    int count = 2;
    p = Put(p, &count, 4);
    for (int k = 0; k < 2; ++k) {
        float health = 0.25f + k;
        unsigned int bank = 100 + k;
        float step = 0.5f, size = 2.0f + k;
        unsigned char additive = (unsigned char)k, type = (unsigned char)(7 + k);
        int emitter = -5 - k;
        unsigned char blocks = (unsigned char)(1 - k);
        p = Put(p, &health, 4); p = Put(p, &bank, 4); p = Put(p, &step, 4); p = Put(p, &size, 4);
        p = Put(p, &additive, 1); p = Put(p, &type, 1); p = Put(p, &emitter, 4); p = Put(p, &blocks, 1);
    }
    int total = (int)(p - buf);
    CMemoryDataInputStreamReader s;
    memset(&s, 0, sizeof s);
    s.chunkBase = (long)buf; s.chunkLength = total;
    std::vector<CDegradableInfo> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn<CDegradableInfo>(s, v);
    if (g_resize != 1 || g_zeroDefault != 1 || g_overflow != 0 || v._M_finish - v._M_start != 2) { printf("0067f430_TEST FAIL 1\n"); return 1; }
    for (int k = 0; k < 2; ++k) {
        const CDegradableInfo& e = v._M_start[k];
        float dh = e.HealthPercentage - (0.25f + k); if (dh < 0) dh = -dh;
        if (dh > 0.0001f || e.EngineGraphic.BankIndex != (unsigned int)(100 + k) || e.EngineGraphic.Type != 7 + k ||
            (e.EngineGraphic.AdditiveAlpha ? 1 : 0) != k || e.SmashParticleEmitter != -5 - k || (e.BlocksNavigation ? 1 : 0) != 1 - k) {
            printf("0067f430_TEST FAIL 2\n"); return 1;
        }
    }
    if (s.position != total || s.chunkLength != 0) { printf("0067f430_TEST FAIL 3\n"); return 1; }
    printf("0067f430_TEST PASS\n");
    return 0;
}