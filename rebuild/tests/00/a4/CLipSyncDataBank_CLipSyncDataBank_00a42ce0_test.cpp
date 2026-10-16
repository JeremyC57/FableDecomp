#include <cstdio>
// ~CLipSyncDataBank: clear empties the entries (finish = start) and the counted pointer drops its reference.
struct Block { long refs; };
int main() {
    int entries[4]; int* start = entries; int* finish = entries + 4;
    Block b = { 2 }; Block* count = &b; void* object = &b;
    finish = start;                                    // Entries.clear(): erase(begin(), end())
    if (count) { --count->refs; } object = 0; count = 0; // ~CCountedPointer
    if (finish != start || b.refs != 1 || object || count) { std::printf("00a42ce0_TEST FAIL\n"); return 1; }
    std::printf("00a42ce0_TEST PASS\n"); return 0; }