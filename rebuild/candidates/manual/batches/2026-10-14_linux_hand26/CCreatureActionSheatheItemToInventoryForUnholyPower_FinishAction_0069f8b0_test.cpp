#include <cstdio>
struct Comp { unsigned char pad[0x18]; unsigned char flag; long cancelled; Comp() : flag(0), cancelled(0) {} void CancelAbilities() { ++cancelled; } };
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
struct Base { virtual ~Base() {} virtual void FinishAction() { ++g_base; } Ptr PCreature; };
struct Act : Base { void FinishAction() { Base::FinishAction(); Comp* c; if (PCreature.GetPItem()->GetInterface(&c, 0x86)) { if (!c->flag) c->CancelAbilities(); } } };
int main()
{
    Thing t; Comp c; Entry e[2] = { { 0x10, 0 }, { 0x86, &c } }; t.map.first = e; t.map.last = e + 2;
    for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    Act a; a.PCreature.p = &t;
    a.FinishAction();                       // bit clear: nothing
    t.bits[0x86 >> 5] = 1UL << (0x86 & 31); a.FinishAction();   // present, flag clear: cancelled
    c.flag = 1; a.FinishAction();           // flag set: nothing
    if (g_base != 3 || c.cancelled != 1) { std::printf("0069f8b0_TEST FAIL\n"); return 1; }
    std::printf("0069f8b0_TEST PASS\n");
    return 0;
}
