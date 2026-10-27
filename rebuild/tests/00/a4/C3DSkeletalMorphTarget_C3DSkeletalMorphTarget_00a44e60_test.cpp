// behaviour test for 0x00A44E60: a default record has an empty name and an empty array
#include <cstdio>
#include <string>
#include <vector>
struct R { std::string Name; void* Scalar; std::vector<long> Arr; R() { } };
int main()
{
    R r;
    if (!r.Name.empty() || !r.Arr.empty() || r.Arr.size() != 0) { std::printf("00a44e60_TEST FAIL\n"); return 1; }
    std::printf("00a44e60_TEST PASS\n");
    return 0;
}