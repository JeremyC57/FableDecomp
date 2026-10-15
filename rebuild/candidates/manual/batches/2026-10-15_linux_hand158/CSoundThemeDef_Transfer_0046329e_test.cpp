// Behaviour test for 0x0046329E: transfer order of the sound theme members.
#include <cstdio>
struct Ctx { void* order[16]; int n; const char* mapName; int mapDefault;
  void T(void* p) { order[n++] = p; }
  void TransferMap(const char* name, void* m, const int& d) { mapName = name; mapDefault = d; order[n++] = m; } };
struct Def { unsigned char colour[4]; int atmos, music; float gain, range, gran; int map[3]; bool a, b, c, d;
  void Transfer(Ctx& x) { x.T(&atmos); x.T(&music); x.T(colour); x.T(&gain); x.T(&range); x.T(&gran); x.TransferMap("AtmosGroupMap", map, 0);
    x.T(&a); x.T(&c); x.T(&b); x.T(&d); } };
int main()
{
    Ctx x; x.n = 0; Def d; d.Transfer(x);
    bool ok = x.n == 11 && x.order[0] == &d.atmos && x.order[2] == d.colour && x.order[6] == d.map && x.order[8] == &d.c && x.order[9] == &d.b && x.mapDefault == 0;
    if (!ok) { std::printf("0046329e_TEST FAIL\n"); return 1; }
    std::printf("0046329e_TEST PASS\n");
    return 0;
}
