#include <cstdio>

// Fake device recording the last draw call (the COM slots are modelled as plain virtuals).
struct FakeDevice {
    int calls; long type; unsigned long a0, a1, a2, a3, a4; long fmt; const void* p0; const void* p1; long ret;
    FakeDevice() : calls(0), type(-1), a0(0), a1(0), a2(0), a3(0), a4(0), fmt(0), p0(0), p1(0), ret(0x1234) { }
    virtual long DrawPrimitive(long t, unsigned long s, unsigned long c) { ++calls; type = t; a0 = s; a1 = c; return ret; }
    virtual long DrawIndexedPrimitive(long t, long base, unsigned long mn, unsigned long nv, unsigned long si, unsigned long pc)
    { ++calls; type = t; a0 = (unsigned long)base; a1 = mn; a2 = nv; a3 = si; a4 = pc; return ret; }
    virtual long DrawIndexedPrimitiveUP(long t, unsigned long mn, unsigned long nv, unsigned long pc, const void* idx, long f,
                                        const void* v, unsigned long stride)
    { ++calls; type = t; a0 = mn; a1 = nv; a2 = pc; p0 = idx; fmt = f; p1 = v; a3 = stride; return ret; }
    virtual ~FakeDevice() { }
};

struct IndexBuffer { long primitiveType; };

struct Core {
    int realised; int deviceCallsAtRealise; FakeDevice* PD3DDevice; IndexBuffer* PSelectedIndexBuffer;
    Core(FakeDevice* d) : realised(0), deviceCallsAtRealise(-1), PD3DDevice(d), PSelectedIndexBuffer(0) { }
    void RealiseRenderState() { ++realised; deviceCallsAtRealise = PD3DDevice->calls; }
    long DrawCurrentBuffersAsIndexedTris(unsigned long minVertexIndex, unsigned long vertexCount,
                                         unsigned long startIndex, unsigned long triangleCount)
    {
        RealiseRenderState();
        return PD3DDevice->DrawIndexedPrimitive(4, 0, minVertexIndex, vertexCount, startIndex, triangleCount);
    }
};

int main()
{
    FakeDevice d; Core c(&d);
    long r = c.DrawCurrentBuffersAsIndexedTris(1, 2, 3, 4);
    if (r != 0x1234 || c.realised != 1 || c.deviceCallsAtRealise != 0 || d.calls != 1 || d.type != 4 || d.a0 != 0
        || d.a1 != 1 || d.a2 != 2 || d.a3 != 3 || d.a4 != 4)
    { std::printf("00a0ac60_TEST FAIL\n"); return 1; }
    std::printf("00a0ac60_TEST PASS\n");
    return 0;
}