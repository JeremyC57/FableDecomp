// behaviour test for 0x004acab0 CThing::GetTC<CTCRegionDisplay>(out): when the thing's component bit 142 is set, look the interface type
// up in the sorted component map (lower bound) and store the component through out, returning true; otherwise false
// and out untouched
#include <cstdio>
struct Pair { int first; void* second; };
struct Thing {
    unsigned char bits[40]; Pair* start; Pair* finish;
    Pair* LowerBound(int key) { Pair* p = start; while (p != finish && p->first < key) ++p; return p; }
    bool GetTC(void** out)
    {
        if (bits[142 / 8] & (1 << (142 % 8))) {
            Pair* i = LowerBound(142);
            if (i == finish || 142 < i->first) i = finish;
            *out = i->second;
            return true;
        }
        return false;
    }
};
int main()
{
    int fails = 0;
    int comp = 0, other = 0;
    Pair map[3] = { { 1, &other }, { 142, &comp }, { 300, &other } };
    Thing t; for (int i = 0; i < 40; ++i) t.bits[i] = 0;
    t.start = map; t.finish = map + 3;
    void* out = &t;
    if (t.GetTC(&out) || out != &t) fails |= 1;
    t.bits[142 / 8] |= (unsigned char)(1 << (142 % 8));
    if (!t.GetTC(&out) || out != &comp) fails |= 2;
    if (fails) { std::printf("004acab0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004acab0_TEST PASS\n");
    return 0;
}