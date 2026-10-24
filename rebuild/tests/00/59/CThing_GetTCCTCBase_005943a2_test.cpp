// Behaviour test for 0x005943A2 CThing::GetTC<T>(ETCInterfaceType, T**) (model copy)
#include <cstdio>
struct Component { int tag; };
struct Entry { int type; Component* component; };
struct Thing {
    unsigned int bits[9];
    Entry* first; Entry* last;
    bool test(int pos) const { return (bits[pos >> 5] >> (pos & 31)) & 1; }
    const Entry* lower_bound(const int& key) const
    {
        const Entry* f = first; int n = int(last - first);
        while (n > 0) { int half = n / 2; if (f[half].type < key) { f += half + 1; n -= half + 1; } else n = half; }
        return f;
    }
    bool GetTC(int type, Component** out)
    {
        if (test(type)) {
            int key = type;
            const Entry* it = lower_bound(key);
            if (it == last || type < it->type) it = last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
int main()
{
    Component a, b, endc; a.tag = 1; b.tag = 2; endc.tag = 99;
    Entry e[3]; e[0].type = 4; e[0].component = &a; e[1].type = 0x46; e[1].component = &b; e[2].type = -1; e[2].component = &endc;
    Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    t.bits[0] |= 1u << 4; t.bits[0x46 >> 5] |= 1u << (0x46 & 31);
    t.first = e; t.last = e + 2;
    Component* out = 0;
    if (!t.GetTC(0x46, &out) || out != &b) { std::printf("005943a2_TEST FAIL\n"); return 1; }
    if (!t.GetTC(4, &out) || out != &a) { std::printf("005943a2_TEST FAIL\n"); return 1; }
    out = 0;
    if (t.GetTC(5, &out) || out != 0) { std::printf("005943a2_TEST FAIL\n"); return 1; }
    t.bits[0] |= 1u << 5;                    // bit set but no map entry: the end entry's value is handed out
    if (!t.GetTC(5, &out) || out != &endc) { std::printf("005943a2_TEST FAIL\n"); return 1; }
    std::printf("005943a2_TEST PASS\n");
    return 0;
}