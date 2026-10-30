// Behaviour test for vorbisfile.c _seek_helper (0x00C89C80): with a reader, seek it to the low dword of offset from the
// start, record the offset and reset the sync state; without one, do nothing.
#include <cstdio>
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
struct Reader { long pos; int origin; bool Seek(long o, int org) { pos = o; origin = org; return true; } };
struct VF { Reader* datasource; i64 offset; int resets; };
static void _seek_helper(VF* vf, i64 offset)
{
    if (vf->datasource) {
        vf->datasource->Seek((long)offset, 2);
        vf->offset = offset;
        vf->resets++;
    }
}
int main()
{
    Reader r = { 0, 0 }; VF vf = { &r, 0, 0 };
    _seek_helper(&vf, 1234);
    if (r.pos != 1234 || r.origin != 2 || vf.offset != 1234 || vf.resets != 1) { std::printf("00c89c80_TEST FAIL\n"); return 1; }
    VF none = { 0, 7, 0 };
    _seek_helper(&none, 99);
    if (none.offset != 7 || none.resets != 0) { std::printf("00c89c80_TEST FAIL\n"); return 1; }
    std::printf("00c89c80_TEST PASS\n"); return 0;
}