#include <cstdio>
#include <string>
// Behaviour model: empty for a null thing, else the scratchpad's string.
static std::string get(const std::string* field) { if (!field) return std::string(""); return *field; }
int main()
{
    std::string s("SEAT_1");
    if (get(0) != "" || get(&s) != "SEAT_1") { std::printf("008a42c0_TEST FAIL\n"); return 1; }
    std::printf("008a42c0_TEST PASS\n");
    return 0;
}