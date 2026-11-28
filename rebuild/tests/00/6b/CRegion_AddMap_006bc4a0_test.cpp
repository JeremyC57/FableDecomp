#include <vector>
#include <algorithm>
#include <stdio.h>
#include "fable_assert.h"

class CRegion {
public:
    std::vector<long> Maps;
    void AddMap(long);
};

void CRegion::AddMap(long map_index)
{
    FABLE_RETAIL_ASSERT(std::find(Maps.begin(), Maps.end(), map_index) == Maps.end());
    Maps.push_back(map_index);
}

int main()
{
    CRegion region;
    for (long i = 0; i < 192; ++i) {
        const long value = (i % 13) - 6;
        region.AddMap(value);
        if (region.Maps.size() != (unsigned long)(i + 1)) return 1;
        for (long j = 0; j <= i; ++j)
            if (region.Maps[j] != (j % 13) - 6) return 2;
    }
    puts("006bc4a0_TEST PASS");
    return 0;
}
