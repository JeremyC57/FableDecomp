#include <cstdio>
struct Process { int removed; void RemoveFromInterface() { ++removed; } };
int main() {
    Process p[18] = {};
    for (int i=0;i<18;++i) p[i].RemoveFromInterface();
    for (int i=0;i<18;++i) if (p[i].removed != 1) { std::printf("00633420_TEST FAIL\n"); return 1; }
    std::printf("00633420_TEST PASS\n"); return 0;
}
