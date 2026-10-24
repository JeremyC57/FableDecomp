// behaviour test for 0x00B8F1C0 _STL::__lower_bound over the flare vertex-buffer map (key compare via MapperPred)
#include <cstdio>

struct Entry { unsigned int first; void* Data; void* Info; };

struct MapperPred {
    MapperPred() { }
    bool operator()(const Entry& e, unsigned int id) const { return e.first < id; }
};

static Entry* LowerBound(Entry* first, Entry* last, const unsigned int& val, MapperPred comp, int*)
{
    int len = (int)(last - first);
    while (len > 0) {
        int half = len >> 1;
        Entry* middle = first + half;
        if (comp(*middle, val)) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

int main()
{
    Entry e[5];
    unsigned int keys[5] = { 2, 5, 5, 9, 0x80000001u };
    for (int i = 0; i < 5; ++i) { e[i].first = keys[i]; e[i].Data = 0; e[i].Info = 0; }
    unsigned int q;
    q = 0;  if (LowerBound(e, e + 5, q, MapperPred(), 0) != e)     { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    q = 5;  if (LowerBound(e, e + 5, q, MapperPred(), 0) != e + 1) { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    q = 6;  if (LowerBound(e, e + 5, q, MapperPred(), 0) != e + 3) { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    q = 0x80000000u; if (LowerBound(e, e + 5, q, MapperPred(), 0) != e + 4) { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    q = 0xffffffffu; if (LowerBound(e, e + 5, q, MapperPred(), 0) != e + 5) { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    if (LowerBound(e, e, q, MapperPred(), 0) != e) { std::printf("00b8f1c0_TEST FAIL\n"); return 1; }
    std::printf("00b8f1c0_TEST PASS\n");
    return 0;
}