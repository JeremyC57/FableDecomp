// Behaviour test for CEngineSurfaceCompositionManager::CopyIntermediateBank @ 0x00B2C8E0 -- staged file copy with pool retry / heap fallback
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Pool {
    long Limit;          // largest size the pool can hand out (0 = never)
    int Allocs, Frees, Locks, Unlocks;
    unsigned char Block[0x100000];
    bool Allocate(long* handle, long size) { if (size > Limit) return false; *handle = 5; ++Allocs; return true; }
    void Lock(long) { ++Locks; }
    void* Get(long) { return Block; }
    void Unlock(long) { ++Unlocks; }
    void Free(long) { ++Frees; }
};
struct File {
    unsigned char* Data;
    long Length;
    long Position;
    int Calls;
    void Read(void* out, long size) { std::memcpy(out, Data + Position, size); Position += size; ++Calls; }
    void Write(const void* in, long size) { std::memcpy(Data + Position, in, size); Position += size; ++Calls; }
};
struct Cache { unsigned long BankFileIndex; };
static bool IsPositive(long n) { if (n <= 0) return false; return true; }
static long g_chunk = 0;
static void Copy(Pool* pool, File& source, File& dest)
{
    long handle = -1;
    unsigned char* buffer = 0;
    long bufferSize = 0x100000;
    while (IsPositive(bufferSize)) {
        if (pool->Allocate(&handle, bufferSize)) {
            pool->Lock(handle);
            buffer = (unsigned char*)pool->Get(handle);
            break;
        }
        bufferSize -= 0x10000;
    }
    unsigned char* heap = 0;
    if (!buffer) {
        bufferSize = 0x10000;
        heap = (unsigned char*)std::calloc(bufferSize, 1);
        buffer = heap;
    }
    g_chunk = bufferSize;
    for (long remaining = source.Length; remaining > 0; ) {
        long chunk = remaining < bufferSize ? remaining : bufferSize;
        source.Read(buffer, chunk);
        dest.Write(buffer, chunk);
        remaining -= chunk;
    }
    if (handle != -1) { pool->Unlock(handle); pool->Free(handle); }
    std::free(heap);
}
static Pool g_pool;
int main()
{
    bool ok = true;
    const long n = 0x48123;
    unsigned char* src = (unsigned char*)std::malloc(n);
    unsigned char* dst = (unsigned char*)std::malloc(n);
    for (long i = 0; i < n; ++i) src[i] = (unsigned char)(i * 7 + 3);
    // the pool only fits 0xC0000: the retry loop steps down 0x10000 at a time
    g_pool.Limit = 0xC0000;
    File s = { src, n, 0, 0 }; File d = { dst, n, 0, 0 };
    Copy(&g_pool, s, d);
    ok = ok && g_chunk == 0xC0000 && std::memcmp(src, dst, n) == 0 && s.Calls == 1 && d.Calls == 1;
    ok = ok && g_pool.Allocs == 1 && g_pool.Locks == 1 && g_pool.Unlocks == 1 && g_pool.Frees == 1;
    // the pool never fits: 64 KB heap chunks
    g_pool.Limit = 0;
    std::memset(dst, 0, n);
    File s2 = { src, n, 0, 0 }; File d2 = { dst, n, 0, 0 };
    Copy(&g_pool, s2, d2);
    ok = ok && g_chunk == 0x10000 && std::memcmp(src, dst, n) == 0 && s2.Calls == 5 && g_pool.Frees == 1;
    // the cached surfaces lose their completed-bank index
    Cache caches[3] = { { 4 }, { 0 }, { 9 } };
    for (int i = 0; i < 3; ++i) caches[i].BankFileIndex = 0;
    ok = ok && caches[0].BankFileIndex == 0 && caches[2].BankFileIndex == 0;
    std::free(src); std::free(dst);
    if (ok) std::printf("00b2c8e0_TEST PASS\n"); else std::printf("00b2c8e0_TEST FAIL\n");
    return ok ? 0 : 1;
}