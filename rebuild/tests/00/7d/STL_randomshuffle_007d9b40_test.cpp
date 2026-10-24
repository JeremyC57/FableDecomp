// Behaviour test for 0x007D9B40 random_shuffle<KnickKnack*>: element i swaps with first + rand() % (i + 1), whole
// 16-byte records move together; an empty or single-element range is untouched.
#include <cstdio>

static int g_seq[8]; static int g_pos;
int TestRand() { return g_seq[g_pos++]; }

struct C3DVector {
    float X, Y, Z;
};
class CThingObject { public: int id; };
struct KnickKnack {
    C3DVector     Pos;
    CThingObject* Thing;
};

template <class T> void random_shuffle(T* first, T* last)
{
    if (first == last)
        return;
    for (T* i = first + 1; i != last; ++i) {
        T* b = first + TestRand() % ((i - first) + 1);
        T tmp = *i;
        *i = *b;
        *b = tmp;
    }
}

int main()
{
    CThingObject t[4];
    KnickKnack k[4];
    for (int i = 0; i < 4; ++i) { t[i].id = i; k[i].Thing = &t[i]; k[i].Pos.X = (float)i; k[i].Pos.Y = 10.0f + i; k[i].Pos.Z = 20.0f + i; }
    g_seq[0] = 2; g_seq[1] = 4; g_seq[2] = 9; g_pos = 0;
    random_shuffle(k, k + 4);                 // [A B C D] -> [B A C D] -> [B C A D] -> [B D A C]
    int want[4] = { 1, 3, 0, 2 };
    bool ok = g_pos == 3;
    for (int i = 0; i < 4 && ok; ++i)
        ok = k[i].Thing->id == want[i] && k[i].Pos.X == (float)want[i] && k[i].Pos.Y == 10.0f + want[i] && k[i].Pos.Z == 20.0f + want[i];
    g_pos = 0;
    random_shuffle(k, k);
    random_shuffle(k, k + 1);
    ok = ok && g_pos == 0 && k[0].Thing->id == 1;
    if (!ok) { std::printf("007d9b40_TEST FAIL\n"); return 1; }
    std::printf("007d9b40_TEST PASS\n");
    return 0;
}