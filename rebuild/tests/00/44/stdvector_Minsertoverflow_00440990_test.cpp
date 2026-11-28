#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#endif
#include <stdio.h>
#include "stlport/fable_containers.h"
class LongVectorTest : public std::vector<long> {
public:
    void add(unsigned int position, const long& value, unsigned int count) {
        _M_insert_overflow(begin() + position, value, std::__true_type(), count, position == size());
    }
};
int main() {
    LongVectorTest v;
    v.add(0, 4, 2);
    v.add(1, v[0], 3);
    v.add(0, -9, 1);
    v.add(6, 12, 2);
    const long expected[] = {-9,4,4,4,4,4,12,12};
    if (v.size() != 8) return 1;
    for (unsigned int i=0; i<v.size(); ++i) if(v[i] != expected[i]) return 2;
    puts("00440990_TEST PASS");
    return 0;
}
