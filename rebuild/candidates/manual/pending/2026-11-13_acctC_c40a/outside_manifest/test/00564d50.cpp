// Behaviour test for 0x00564D50 std::char_traits<char>::copy: n characters are copied from the source to the destination (all byte
// counts, including lengths that are not a multiple of four), nothing past n is written, a zero length copies nothing, and the
// destination pointer is returned in every case.
#include <cstdio>
#include <cstring>
static char* copy(char* s1, const char* s2, size_t n) { return (n == 0 ? s1 : (char*)std::memcpy(s1, s2, n * sizeof(char))); }
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    const char src[] = "ABCDEFGHIJKLMNOP";
    for (size_t n = 0; n <= 11; ++n) {
        char dst[16]; std::memset(dst, '#', sizeof(dst));
        char* r = copy(dst, src, n);
        check(r == dst, "the destination is returned");
        check(std::memcmp(dst, src, n) == 0, "n characters copied");
        bool clean = true; for (size_t i = n; i < sizeof(dst); ++i) if (dst[i] != '#') clean = false;
        check(clean, "nothing past n is written");
    }
    char keep[4] = { 'k', 'e', 'e', 'p' };
    check(copy(keep, 0, 0) == keep && std::memcmp(keep, "keep", 4) == 0, "zero length never reads the source");
    if (!fails) std::printf("00564d50_TEST PASS\n");
    return fails;
}
