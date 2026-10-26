// Behaviour test for 0x00A99510 C3DAnimFileLoader::LoadAnimFileCompressed (model copy + recording stubs).
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
static int g_destroyed = 0;
static void __fastcall DestroyFile(void*) { ++g_destroyed; }

template<class T> class CCountedPointer { public:
    T* Object;
    SCountBlock* Count;
    CCountedPointer(T* o, SCountBlock* c) : Object(o), Count(c) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
};

class CDataInputStream { public: };
static int g_streamsAlive = 0;
class CMemoryDataInputStream : public CDataInputStream { public:
    const void* StreamSource; long sourceLength;
    CMemoryDataInputStream(const void* data, long size) : StreamSource(data), sourceLength(size) { ++g_streamsAlive; }
    ~CMemoryDataInputStream() { --g_streamsAlive; }
};

class CHandleBasedMemoryPool { public: int dummy; };
class CScratchBuffer { public: int dummy; };
CScratchBuffer g_ScratchBuffer;

static unsigned char g_bufferBytes[64];
static unsigned long g_bufSize = 0; static CHandleBasedMemoryPool* g_bufPool = 0; static CScratchBuffer* g_bufScratch = 0;
static int g_buffersAlive = 0;
struct CTemporaryBuffer {
    void* Data;
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch)
        : Data(g_bufferBytes) { g_bufSize = size; g_bufPool = pool; g_bufScratch = scratch; ++g_buffersAlive; }
    ~CTemporaryBuffer() { --g_buffersAlive; }
};

static const unsigned char* g_lzSrc = 0; static unsigned long g_lzLen = 0; static unsigned char* g_lzDst = 0; static unsigned long g_lzOut = 0;
int lzo1x_decompress_safe(const unsigned char* src, unsigned long src_len, unsigned char* dst, unsigned long* dst_len, void*)
{
    g_lzSrc = src; g_lzLen = src_len; g_lzDst = dst; g_lzOut = *dst_len; return 0;
}

class C3DAnimFile { public: int dummy; };
static C3DAnimFile g_file;
static const void* g_loadedFrom = 0; static long g_loadedLength = 0; static int g_streamsAtLoad = 0;
struct C3DAnimFileLoader {
    CCountedPointer<C3DAnimFile> LoadAnimFile(CDataInputStream& in)
    {
        CMemoryDataInputStream& s = (CMemoryDataInputStream&)in;
        g_loadedFrom = s.StreamSource; g_loadedLength = s.sourceLength; g_streamsAtLoad = g_streamsAlive;
        SCountBlock* c = new SCountBlock; c->m_references = 1; c->m_pfnDestroy = DestroyFile; c->m_pObject = &g_file;
        return CCountedPointer<C3DAnimFile>(&g_file, c);
    }
    CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool);
};

CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool)
{
    if (memcmp(data, ">>>>", 4) == 0) {
        CMemoryDataInputStream stream(data, size);
        return LoadAnimFile(stream);
    }
    unsigned long decompressedSize = *(const unsigned long*)data;
    CTemporaryBuffer buffer(decompressedSize + 3, &pool, &g_ScratchBuffer);
    unsigned char* dst = (unsigned char*)buffer.Data;
    unsigned long outLength = decompressedSize;
    lzo1x_decompress_safe((const unsigned char*)data + 4, size - 4, dst, &outLength, 0);
    CMemoryDataInputStream stream(dst, decompressedSize);
    CCountedPointer<C3DAnimFile> file = LoadAnimFile(stream);
    return file;
}

static int fail() { std::printf("00a99510_TEST FAIL\n"); return 1; }

int main()
{
    C3DAnimFileLoader loader; CHandleBasedMemoryPool pool;
    unsigned char raw[16] = { '>', '>', '>', '>', 1, 2, 3, 4 };
    {
        CCountedPointer<C3DAnimFile> f = loader.LoadAnimFileCompressed(raw, 16, pool);
        if (f.Object != &g_file || f.Count->m_references != 1) return fail();
        if (g_loadedFrom != raw || g_loadedLength != 16 || g_streamsAtLoad != 1) return fail();
        if (g_bufSize != 0 || g_lzSrc != 0 || g_streamsAlive != 0) return fail();
    }
    if (g_destroyed != 1) return fail();

    unsigned char packed[16] = { 40 };   // little-endian size 40 (zero high bytes: also right for an 8-byte host long)
    {
        CCountedPointer<C3DAnimFile> f = loader.LoadAnimFileCompressed(packed, 12, pool);
        if (f.Object != &g_file || f.Count->m_references != 1 || g_destroyed != 1) return fail();
        if (g_bufSize != 43 || g_bufPool != &pool || g_bufScratch != &g_ScratchBuffer) return fail();
        if (g_lzSrc != packed + 4 || g_lzLen != 8 || g_lzDst != g_bufferBytes || g_lzOut != 40) return fail();
        if (g_loadedFrom != g_bufferBytes || g_loadedLength != 40 || g_streamsAtLoad != 1) return fail();
        if (g_streamsAlive != 0 || g_buffersAlive != 0) return fail();
    }
    if (g_destroyed != 2) return fail();
    std::printf("00a99510_TEST PASS\n");
    return 0;
}