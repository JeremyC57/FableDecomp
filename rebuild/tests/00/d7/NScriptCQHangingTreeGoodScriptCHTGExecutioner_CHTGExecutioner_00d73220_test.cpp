// behaviour test for 0x00D73220 CHTG_Executioner's constructor (model: base fields, parent, empty vector, master data)
#include <cstdio>
struct Thing { int id; };
struct Parent { void* Game; };
struct Entity {
    void* Game; Thing Me; Parent* ParentClass; void* PMasterData; void* s; void* f; void* e;
    Entity(Parent& parent, void* data, const Thing& me) : Game(parent.Game), Me(me), ParentClass(&parent), s(0), f(0), e(0) { PMasterData = data; }
};
int main()
{
    int game = 0, data = 0; Parent p = { &game }; Thing me = { 7 };
    Entity x(p, &data, me);
    int ok = x.Game == &game && x.Me.id == 7 && x.ParentClass == &p && x.PMasterData == &data && x.s == 0 && x.f == 0 && x.e == 0;
    if (!ok) { std::printf("00d73220_TEST FAIL\n"); return 1; }
    std::printf("00d73220_TEST PASS\n");
    return 0;
}