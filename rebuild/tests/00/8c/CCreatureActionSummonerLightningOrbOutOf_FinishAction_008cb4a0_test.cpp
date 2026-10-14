#include <cstdio>
struct Modes { unsigned long has; int removed; Modes() : has(0), removed(0) {} bool HasMode(unsigned long m) { return has == m; } void RemoveMode(unsigned long m) { if (m == has) { ++removed; has = 0; } } };
struct Entry { long type; Modes* comp; };
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
struct Act : Base { void FinishAction() { Base::FinishAction(); Modes* m; if (PCreature.GetPItem()->GetInterface(&m, 0x31)) { if (m->HasMode(90)) m->RemoveMode(90); } } };
int main()
{
    Thing t; Modes m; Entry e[2] = { { 0x10, 0 }, { 0x31, &m } }; t.map.first = e; t.map.last = e + 2;
    for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    Act a; a.PCreature.p = &t;
    m.has = 90; a.FinishAction();           // bit clear: nothing
    t.bits[1] = 1UL << 17; a.FinishAction();   // present with mode 90: removed
    m.has = 5; a.FinishAction();            // other mode: untouched
    if (g_base != 3 || m.removed != 1 || m.has != 5) { std::printf("008cb4a0_TEST FAIL\n"); return 1; }
    std::printf("008cb4a0_TEST PASS\n");
    return 0;
}