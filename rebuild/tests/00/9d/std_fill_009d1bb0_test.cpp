// behaviour test for 0x009d1bb0 (std::fill over pair<unsigned long, CSoundPair>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
class CSoundPair { public:                             // 8 bytes
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<unsigned long, CSoundPair> CSoundEntry;
template void __fastcall fill(CSoundEntry*, CSoundEntry*, const CSoundEntry&);
}

int main()
{
    std::CSoundEntry v[4];
    for (int i = 0; i < 4; ++i) { v[i].first = 0; v[i].second.FirstSound = 0; v[i].second.LastSound = 0; }
    std::CSoundEntry val; val.first = 7; val.second.FirstSound = 11; val.second.LastSound = 12;
    std::fill(v + 1, v + 3, val);
    std::fill(v + 3, v + 3, val);
    int ok = v[0].first == 0 && v[1].first == 7 && v[1].second.FirstSound == 11 && v[2].second.LastSound == 12 && v[3].first == 0;
    if (!ok) { printf("009d1bb0_TEST FAIL\n"); return 1; }
    printf("009d1bb0_TEST PASS\n");
    return 0;
}