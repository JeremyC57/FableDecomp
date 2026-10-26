#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_destroyed = 0;
class CVertexStreamFormat { public: unsigned int Size; unsigned int CalculateVertexSize() const { return Size; } };
struct CStreamInfo { unsigned int StreamIndex; CVertexStreamFormat StreamFormat; };
class CVertexShaderInput {
public:
    CStreamInfo Streams[2];
    CVertexShaderInput() { Streams[0].StreamFormat.Size = 0; Streams[1].StreamFormat.Size = 0; }
    virtual ~CVertexShaderInput() { ++g_destroyed; }
    const CVertexStreamFormat& GetStreamFormat(unsigned int i) const { return Streams[i].StreamFormat; }
};
class CVertexShaderInputAnimated : public CVertexShaderInput {
public:
    void Initialise(unsigned int flags, bool) { Streams[0].StreamFormat.Size = 0x30 + 4 * (flags & 3); }
};
class CVertexShaderInputStatic : public CVertexShaderInput {
public:
    void Initialise(unsigned int flags, bool) { Streams[0].StreamFormat.Size = 0x18 + 4 * (flags & 3); }
};

// the row: C3DPrimitive2::CalculateVertexSize
static unsigned int __fastcall CalculateVertexSize(unsigned int vertexFlags, bool animated, bool compressedLayout)
{
    if (compressedLayout)
        return 0x24;
    if (animated) {
        CVertexShaderInputAnimated input;
        input.Initialise(vertexFlags, true);
        return input.GetStreamFormat(0).CalculateVertexSize();
    } else {
        CVertexShaderInputStatic input;
        input.Initialise(vertexFlags, true);
        return input.GetStreamFormat(0).CalculateVertexSize();
    }
}

int main()
{
    Check(CalculateVertexSize(3, true, true) == 36 && CalculateVertexSize(0, false, true) == 36);
    Check(g_destroyed == 0);
    Check(CalculateVertexSize(2, true, false) == 0x38);
    Check(g_destroyed == 1);
    Check(CalculateVertexSize(1, false, false) == 0x1c);
    Check(g_destroyed == 2);

    if (g_fail) {
        std::printf("00a910e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a910e0_TEST PASS\n");
    return 0;
}