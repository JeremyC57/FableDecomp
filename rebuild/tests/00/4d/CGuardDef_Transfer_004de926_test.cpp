// behaviour test for 0x004DE926 CGuardDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    long BribeCostWeaponOut;
    long BribeCostTrespass;
    long BribeCostVandalism;
    long BribeCostShoplifter;
    long BribeCostPickPocket;
    long BribeCostAssault;
    long BribeCostGuardAssault;
    long BribeCostGBH;
    long BribeCostGuardGBH;
    long BribeCostMurder;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.BribeCostWeaponOut);
    c.Transfer(d.BribeCostTrespass);
    c.Transfer(d.BribeCostVandalism);
    c.Transfer(d.BribeCostShoplifter);
    c.Transfer(d.BribeCostPickPocket);
    c.Transfer(d.BribeCostAssault);
    c.Transfer(d.BribeCostGuardAssault);
    c.Transfer(d.BribeCostGBH);
    c.Transfer(d.BribeCostGuardGBH);
    c.Transfer(d.BribeCostMurder);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[10] = { &d.BribeCostWeaponOut, &d.BribeCostTrespass, &d.BribeCostVandalism, &d.BribeCostShoplifter, &d.BribeCostPickPocket, &d.BribeCostAssault, &d.BribeCostGuardAssault, &d.BribeCostGBH, &d.BribeCostGuardGBH, &d.BribeCostMurder };
    if (save.count != 10u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 10u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.BribeCostWeaponOut, 0, sizeof(e.BribeCostWeaponOut));
    std::memset(&e.BribeCostTrespass, 0, sizeof(e.BribeCostTrespass));
    std::memset(&e.BribeCostVandalism, 0, sizeof(e.BribeCostVandalism));
    std::memset(&e.BribeCostShoplifter, 0, sizeof(e.BribeCostShoplifter));
    std::memset(&e.BribeCostPickPocket, 0, sizeof(e.BribeCostPickPocket));
    std::memset(&e.BribeCostAssault, 0, sizeof(e.BribeCostAssault));
    std::memset(&e.BribeCostGuardAssault, 0, sizeof(e.BribeCostGuardAssault));
    std::memset(&e.BribeCostGBH, 0, sizeof(e.BribeCostGBH));
    std::memset(&e.BribeCostGuardGBH, 0, sizeof(e.BribeCostGuardGBH));
    std::memset(&e.BribeCostMurder, 0, sizeof(e.BribeCostMurder));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004de926_TEST FAIL %d\n", fail); return 1; }
    std::printf("004de926_TEST PASS\n");
    return 0;
}