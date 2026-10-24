#include <cstdio>
#include <cstring>

// Model of 0x00405BC0 (STLport __copy_trivial_backward): memmove [first, last) so it ends at result and return
// the new start; an empty range returns result untouched.
static char* CopyTrivialBackward(const char* first, const char* last, char* result)
{
    const int n = (int)(last - first);
    return (n > 0) ? (char*)std::memmove(result - n, first, n) : result;
}

int main()
{
    char buf[16];
    std::strcpy(buf, "abcdefgh");
    char* start = CopyTrivialBackward(buf, buf + 5, buf + 7);   // overlapping shift right by 2
    if (start != buf + 2 || std::memcmp(buf, "ababcdeh", 8) != 0) {
        std::printf("00405bc0_TEST FAIL\n");
        return 1;
    }
    if (CopyTrivialBackward(buf + 3, buf + 3, buf + 6) != buf + 6 || std::memcmp(buf, "ababcdeh", 8) != 0) {
        std::printf("00405bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00405bc0_TEST PASS\n");
    return 0;
}