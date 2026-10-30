// Behaviour test for vorbisfile.c _get_data (0x00C89C10): at most 8500 bytes, clamped to what is left; 0 at the end or without a source; -1 when the read fails.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
#ifdef _MSC_VER
typedef __int64 i64;
#else
#include <stdint.h>
typedef int64_t i64;
#endif
#endif
#define FAIL(a) do { std::printf(a "_TEST FAIL\n"); return 1; } while (0)
struct Reader { unsigned long size, pos; bool fail; unsigned long Tell() { return pos; } unsigned long GetSize() { return size; }
    bool ReadBlock(void*, unsigned long n) { if (fail) return false; pos += n; return true; } };
struct VF { Reader* datasource; long wrote; };
static long _get_data(VF* vf)
{
    if (vf->datasource) {
        unsigned long bytes = vf->datasource->Tell();
        unsigned long left = vf->datasource->GetSize() - bytes;
        bytes = 8500;
        if (left < 8500) { bytes = left; if (bytes == 0) return 0; }
        static char buffer[8500];
        if (!vf->datasource->ReadBlock(buffer, bytes)) return -1;
        vf->wrote += bytes;
        return bytes;
    }
    return 0;
}
int main()
{
    Reader r = { 10000, 0, false }; VF vf = { &r, 0 };
    if (_get_data(&vf) != 8500 || _get_data(&vf) != 1500 || _get_data(&vf) != 0 || vf.wrote != 10000) FAIL("00c89c10");
    Reader bad = { 100, 0, true }; VF vb = { &bad, 0 };
    if (_get_data(&vb) != -1) FAIL("00c89c10");
    VF none = { 0, 0 };
    if (_get_data(&none) != 0) FAIL("00c89c10");
    std::printf("00c89c10_TEST PASS\n"); return 0;
}