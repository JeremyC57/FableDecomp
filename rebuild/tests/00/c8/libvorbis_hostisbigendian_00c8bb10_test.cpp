// Behaviour test for 00c8bb10: host_is_big_endian (Xiph vorbisfile.c)
#include <cstdio>
#include <cstring>

static int host_is_big_endian(void)
{
    long pattern = 0xfeedface;          // ogg_int32_t
    unsigned char* bytewise = (unsigned char*)&pattern;
    if (bytewise[0] == 0xfe) return 1;
    return 0;
}

int main()
{
    int ok;
    ok = host_is_big_endian() == 0;
    std::printf(ok ? "00c8bb10_TEST PASS\n" : "00c8bb10_TEST FAIL\n");
    return ok ? 0 : 1;
}
