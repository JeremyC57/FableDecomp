// behaviour test for 0x009B5D30: a default record has an empty name and an empty array
#include <cstdio>
#include <string>
#include <vector>
struct R { std::string Name; void* Scalar; std::vector<long> Arr; R() { } };
int main()
{
    R r;
    if (!r.Name.empty() || !r.Arr.empty() || r.Arr.size() != 0) { std::printf("009b5d30_TEST FAIL\n"); return 1; }
    std::printf("009b5d30_TEST PASS\n");
    return 0;
}