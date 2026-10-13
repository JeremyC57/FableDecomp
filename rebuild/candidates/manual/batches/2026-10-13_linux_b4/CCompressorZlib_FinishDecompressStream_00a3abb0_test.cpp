#include <cstdio>

struct Z { unsigned char* next_in; unsigned long avail_in; unsigned long total_in; unsigned char* next_out; unsigned long avail_out; unsigned char rest[0x24]; };
static int g_end = 0;
extern "C" int inflateEnd(Z*) { ++g_end; return 0; }
struct C { unsigned char pad[0x1c]; Z Stream; bool Init;
    void Finish() { if (Init) { inflateEnd(&Stream); Stream.next_in = 0; Stream.next_out = 0; Stream.avail_in = 0; Stream.avail_out = 0; Init = false; } } };
int main()
{
    unsigned char b; C c; c.Init = true; c.Stream.next_in = &b; c.Stream.avail_in = 5; c.Stream.next_out = &b; c.Stream.avail_out = 6; c.Stream.total_in = 7;
    c.Finish(); if (g_end != 1 || c.Init || c.Stream.next_in || c.Stream.avail_in || c.Stream.next_out || c.Stream.avail_out || c.Stream.total_in != 7) { std::printf("00a3abb0_TEST FAIL\n"); return 1; }
    c.Finish(); if (g_end != 1) { std::printf("00a3abb0_TEST FAIL\n"); return 1; }
    std::printf("00a3abb0_TEST PASS\n");
    return 0;
}
