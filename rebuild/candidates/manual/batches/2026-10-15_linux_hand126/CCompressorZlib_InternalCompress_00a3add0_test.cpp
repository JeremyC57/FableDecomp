// Behaviour test for 00a3add0 CCompressorZlib::InternalCompress(src, size, CArray&) with a stub worker:
// the array is sized to size + size/100 + 16, the worker writes into its data, the array is cut to
// the produced size; a bound beyond 32 bits returns 0 without touching the array.
#include <cstdio>

struct CArray { unsigned long m_size; unsigned char m_data[256]; int m_trims;
    unsigned long GetSize() const { return m_size; } unsigned char* GetData() { return m_data; }
    void resize(unsigned long n, const unsigned char&) { m_size = n; } void DeallocateUnusedMemory() { ++m_trims; } };
static unsigned long g_seenDest = 0;
static unsigned long Worker(const void*, unsigned long srcSize, void*, unsigned long destSize) { g_seenDest = destSize; return srcSize / 3; }

static unsigned long Compress(const void* src, unsigned long srcSize, CArray& out)
{
#ifdef _MSC_VER
    unsigned __int64 bound = (unsigned __int64)srcSize / 100 + srcSize + 16;
#else
    unsigned long long bound = (unsigned long long)srcSize / 100 + srcSize + 16;
#endif
    if (bound > 0xFFFFFFFFul) return 0;
    out.resize(srcSize / 100 + srcSize + 16, 0);
    unsigned long produced = Worker(src, srcSize, out.GetData(), out.GetSize());
    out.resize(produced, 0);
    out.DeallocateUnusedMemory();
    return produced;
}

int main()
{
    CArray a; a.m_size = 0; a.m_trims = 0; unsigned char src[1];
    bool ok = Compress(src, 200, a) == 66 && g_seenDest == 218 && a.m_size == 66 && a.m_trims == 1;
    a.m_size = 5;
    ok = ok && Compress(src, 0xFFFFFFF0ul, a) == 0 && a.m_size == 5 && a.m_trims == 1;
    if (!ok) { std::printf("00a3add0_TEST FAIL\n"); return 1; }
    std::printf("00a3add0_TEST PASS\n");
    return 0;
}
