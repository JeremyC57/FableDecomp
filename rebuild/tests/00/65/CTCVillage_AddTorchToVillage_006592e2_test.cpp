// Behaviour test for CTCVillage::AddTorchToVillage (0x006592E2): the torch is appended and its village-member component
// pointed at the village.
#include <cstdio>
struct Village;
struct Member { Village* village; };
struct Torch { Member member; };
struct Village {
    Torch* torches[4]; int n;
    void AddTorchToVillage(Torch* t) { torches[n++] = t; Member* m = 0; m = &t->member; m->village = this; }
};
int main()
{
    Village v; v.n = 0; Torch t; t.member.village = 0;
    v.AddTorchToVillage(&t);
    if (v.n != 1 || v.torches[0] != &t || t.member.village != &v) { std::printf("006592e2_TEST FAIL\n"); return 1; }
    std::printf("006592e2_TEST PASS\n");
    return 0;
}