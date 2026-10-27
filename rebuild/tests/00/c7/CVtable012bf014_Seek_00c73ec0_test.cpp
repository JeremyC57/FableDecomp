#include <cstdio>
// CVtable_012bf014::Seek: any position is accepted and returned unchanged.
struct Tone { int Seek(unsigned int position, bool exact) { return (int)position; } };
int main()
{
    Tone t;
    if (t.Seek(0, true) != 0 || t.Seek(12345, false) != 12345 || t.Seek(44100, true) != 44100) {
        std::printf("00c73ec0_TEST FAIL\n"); return 1;
    }
    std::printf("00c73ec0_TEST PASS\n");
    return 0;
}