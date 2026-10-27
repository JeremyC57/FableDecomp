// behaviour test for 0x00624240: inserted keys come back in order with their values
#include <cstdio>
#include <map>
int main()
{
    std::map<unsigned long, unsigned long> m; m.insert(std::make_pair(5ul, 50ul)); m.insert(std::make_pair(1ul, 10ul)); m.insert(std::make_pair(9ul, 90ul));
    std::map<unsigned long, unsigned long>::iterator it = m.begin();
    if (m.size() != 3 || it->first != 1 || it->second != 10 || (++it)->first != 5 || (--m.end())->second != 90) { std::printf("00624240_TEST FAIL\n"); return 1; }
    std::printf("00624240_TEST PASS\n");
    return 0;
}