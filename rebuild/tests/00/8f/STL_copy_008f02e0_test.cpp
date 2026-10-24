// Behaviour test for 0x008F02E0 _STL::__copy<Coin*>: element-wise assignment in order, intelligent pointer re-set.
#include <cstdio>

static int g_setCalls = 0;

struct CIntelligentPointer {
    void* item;
    CIntelligentPointer() : item(0) {}
    void* GetPItem() const { return item; }
    void  SetPItem(void* p) { item = p; ++g_setCalls; }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};

struct Coin {
    float Pos[2], Vel[2], IMass, APos, AVel, AIMass;
    bool  Owned;
    CIntelligentPointer Thing;
    float Radius;
    int   Owner;
    unsigned int Sound;
};

static Coin* __copy(Coin* first, Coin* last, Coin* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    Coin c[6];
    int things[6];
    for (int i = 0; i < 6; ++i) {
        c[i].Pos[0] = (float)i; c[i].Owned = (i & 1) != 0; c[i].Thing.item = &things[i];
        c[i].Radius = 0.5f * i; c[i].Owner = i; c[i].Sound = 100u + i;
    }
    int fail = 0;
    g_setCalls = 0;
    Coin* end = __copy(c + 2, c + 6, c);            // erase the first two: shift four down
    if (end != c + 4 || g_setCalls != 4) fail = 1;
    if (c[0].Owner != 2 || c[3].Owner != 5 || c[1].Thing.item != &things[3] || !c[1].Owned || c[2].Sound != 104u) fail = 1;
    float d = c[3].Pos[0] - 5.0f;
    if (d > 0.001f || d < -0.001f) fail = 1;
    if (__copy(c, c, c + 1) != c + 1 || g_setCalls != 4) fail = 1;   // empty range
    if (fail) { std::printf("008f02e0_TEST FAIL\n"); return 1; }
    std::printf("008f02e0_TEST PASS\n");
    return 0;
}