// behaviour test for 0x00B564E0 CIndexBuffer::~CIndexBuffer: Uninitialise releases the Direct3D index buffer once, then the base runs
#include <cstdio>

static int g_releases = 0;
static int g_baseDtors = 0;

struct IDirect3DIndexBuffer9 {                           // stand-in COM object
    virtual unsigned long AddRef() { return 1; }
    virtual unsigned long Release() { ++g_releases; return 0; }
};

class CMovableResource {
public:
    virtual ~CMovableResource() { ++g_baseDtors; }
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size) = 0;
};

class CIndexBuffer : public CMovableResource {
public:
    IDirect3DIndexBuffer9* PD3DIndexBuffer;
    unsigned long          ByteLength;
    long                   PrimitiveType;
    void*                  PAllocatedBuffer;
    CIndexBuffer() : PD3DIndexBuffer(0), ByteLength(0), PrimitiveType(0), PAllocatedBuffer(0) { }
    virtual ~CIndexBuffer();
    virtual void RelocateData(long, void*, void*, long) { }
    void Uninitialise()                                  // stand-in for 0x00AC1EE0
    {
        if (PD3DIndexBuffer) {
            PD3DIndexBuffer->Release();
            PD3DIndexBuffer = 0;
        }
    }
};

CIndexBuffer::~CIndexBuffer()
{
    Uninitialise();
}

int main()
{
    IDirect3DIndexBuffer9 device;
    CIndexBuffer* buffer = new CIndexBuffer;
    buffer->PD3DIndexBuffer = &device;
    CMovableResource* base = buffer;
    delete base;
    if (g_releases != 1 || g_baseDtors != 1) { std::printf("00b564e0_TEST FAIL\n"); return 1; }
    {
        CIndexBuffer empty;                              // no device buffer: nothing to release
    }
    if (g_releases != 1 || g_baseDtors != 2) { std::printf("00b564e0_TEST FAIL\n"); return 1; }
    std::printf("00b564e0_TEST PASS\n");
    return 0;
}