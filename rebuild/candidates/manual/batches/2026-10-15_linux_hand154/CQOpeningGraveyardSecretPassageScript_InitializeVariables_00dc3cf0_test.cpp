// Behaviour test for 0x00dc3cf0 (CQ_OpeningGraveyardSecretPassageScript::InitializeVariables): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Data { bool flag11c; };
struct OGSP { Data* data; bool f[13]; char hero[4][32]; bool Lieutenant; long Friendlies; bool Visitors; bool Skeleton[4];
    void Init() { std::strcpy(hero[0], "OBJECT_CEREMONIAL_SWORD"); std::strcpy(hero[1], "OBJECT_CEREMONIAL_SHIELD"); std::strcpy(hero[2], "OBJECT_CEREMONIAL_ARMOUR"); std::strcpy(hero[3], "OBJECT_CEREMONIAL_HELMET");
        for (int i = 0; i < 13; ++i) f[i] = false; Lieutenant = false; Friendlies = 0; Visitors = false; data->flag11c = false; } };

int main()
{
    Data d; d.flag11c = true; OGSP o; o.data = &d; for (int i = 0; i < 13; ++i) o.f[i] = true; o.Lieutenant = o.Visitors = true; o.Friendlies = 3; for (int i = 0; i < 4; ++i) o.Skeleton[i] = true;
    o.Init();
    for (int i = 0; i < 13; ++i) CHECK(!o.f[i]); CHECK(!o.Lieutenant && !o.Visitors && o.Friendlies == 0 && !d.flag11c);
    for (int i = 0; i < 4; ++i) CHECK(o.Skeleton[i]);   // SkeletonStuff is left alone
    CHECK(std::strcmp(o.hero[0], "OBJECT_CEREMONIAL_SWORD") == 0 && std::strcmp(o.hero[3], "OBJECT_CEREMONIAL_HELMET") == 0);
    if (g_fail) { std::printf("00dc3cf0_TEST FAIL\n"); return 1; }
    std::printf("00dc3cf0_TEST PASS\n");
    return 0;
}
