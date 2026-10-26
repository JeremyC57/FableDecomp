// behaviour test for 0x004E77C8 CCarryingDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x34),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
struct Blob12 { unsigned char b[12]; };
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
    Blob12 AvailableCarrySlots;
    Blob4 OverriddenDummyObject;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.AvailableCarrySlots);
    c.Transfer(d.OverriddenDummyObject);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[2] = { &d.AvailableCarrySlots, &d.OverriddenDummyObject };
    if (save.count != 2u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 2u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.AvailableCarrySlots, 0, sizeof(e.AvailableCarrySlots));
    std::memset(&e.OverriddenDummyObject, 0, sizeof(e.OverriddenDummyObject));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004e77c8_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e77c8_TEST PASS\n");
    return 0;
}