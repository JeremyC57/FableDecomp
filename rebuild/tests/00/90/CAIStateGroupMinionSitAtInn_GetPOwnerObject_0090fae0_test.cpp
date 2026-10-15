#include <cstdio>
// MinionSitAtInn::GetPOwnerObject: nearest village building with property 0x20 from the creature's position
struct Village { void* inn; long askedProperty; void* FindNearest(const float* pos, long prop) { askedProperty = prop; return pos ? inn : 0; } };
struct Group {
    bool hasVillager; Village* village; float pos[3];
    bool GetPOwnerObject(void** out)
    {
        if (hasVillager) { Village* v = village; if (v != 0) { void* inn = v->FindNearest(pos, 0x20); if (inn != 0) { *out = inn; return true; } } }
        return false;
    }
};
int main()
{
    int innObj; Village v; v.inn = &innObj; v.askedProperty = 0; Group g; g.hasVillager = true; g.village = &v; void* out = 0;
    if (!g.GetPOwnerObject(&out) || out != &innObj || v.askedProperty != 0x20) { std::printf("0090fae0_TEST FAIL\n"); return 1; }
    out = 0; v.inn = 0; if (g.GetPOwnerObject(&out) || out != 0) { std::printf("0090fae0_TEST FAIL\n"); return 1; }
    v.inn = &innObj; g.village = 0; if (g.GetPOwnerObject(&out)) { std::printf("0090fae0_TEST FAIL\n"); return 1; }
    g.village = &v; g.hasVillager = false; if (g.GetPOwnerObject(&out)) { std::printf("0090fae0_TEST FAIL\n"); return 1; }
    std::printf("0090fae0_TEST PASS\n"); return 0;
}