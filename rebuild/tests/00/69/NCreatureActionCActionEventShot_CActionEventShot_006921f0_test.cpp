// behaviour test for 0x006921F0: a default-constructed action event is not finished
#include <cstdio>
struct Base { bool Finished; Base() : Finished(false) { } };
struct Event : Base { Event() { } };
int main()
{
    Event e;
    if (e.Finished) { std::printf("006921f0_TEST FAIL\n"); return 1; }
    std::printf("006921f0_TEST PASS\n");
    return 0;
}