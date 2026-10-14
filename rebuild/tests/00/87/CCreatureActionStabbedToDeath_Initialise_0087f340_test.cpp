#include <cstdio>
struct Comp { unsigned char pad[0x0c]; unsigned long flags; int sets; Comp() : flags(0), sets(0) {} void SetFlags(unsigned long f) { flags = f; ++sets; } };
struct Entry { long type; Comp* comp; };
struct Map { Entry* first; Entry* last; Entry* LowerBound(const long* t) { Entry* e = first; while (e != last && e->type < *t) ++e; return e; } };
struct Thing {
    unsigned char pad[0x20]; unsigned long bits[9]; Map map;
    template<class T> bool GetInterface(T** out, long type) {
        if (bits[type >> 5] & (1UL << (type & 31))) { long k = type; Entry* it = map.LowerBound(&k); if (it == map.last || type < it->type) it = map.last; *out = (T*)it->comp; return true; }
        return false; }
};
static int g_base = 0;
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; };
struct Act : Base { void Initialise() { Base::Initialise(); Comp* c; if (PCreature.GetPItem()->GetInterface(&c, 9)) { unsigned long f = c->flags; if (f & 4) f ^= 4; c->SetFlags(f); } } };
int main()
{
    Thing t; Comp c; Entry e[2] = { { 3, 0 }, { 9, &c } }; t.map.first = e; t.map.last = e + 2;
    for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    Act a; a.PCreature.p = &t;
    c.flags = 0xf; a.Initialise();           // bit clear: nothing
    if (c.sets != 0) { std::printf("0087f340_TEST FAIL\n"); return 1; }
    t.bits[0] = 1UL << 9; a.Initialise();    // present: bit 2 cleared
    if (c.sets != 1 || c.flags != 0xb) { std::printf("0087f340_TEST FAIL\n"); return 1; }
    a.Initialise();                          // already clear: unchanged
    if (c.sets != 2 || c.flags != 0xb || g_base != 3) { std::printf("0087f340_TEST FAIL\n"); return 1; }
    std::printf("0087f340_TEST PASS\n");
    return 0;
}