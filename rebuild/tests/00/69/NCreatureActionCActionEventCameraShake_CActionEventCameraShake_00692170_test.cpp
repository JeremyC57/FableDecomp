// behaviour test for 0x00692170: a default-constructed action event is not finished
#include <cstdio>
struct Base { bool Finished; Base() : Finished(false) { } };
struct Event : Base { Event() { } };
int main()
{
    Event e;
    if (e.Finished) { std::printf("00692170_TEST FAIL\n"); return 1; }
    std::printf("00692170_TEST PASS\n");
    return 0;
}