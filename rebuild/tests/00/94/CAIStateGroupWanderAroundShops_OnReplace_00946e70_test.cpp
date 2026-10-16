// Behaviour test for 00946e70 (see src/00946e70.cpp): WanderAroundShops::OnReplace: leave a live shop, then choose a new idle; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

struct TShop { TThing* removed; void RemoveShopper(TThing* t) { removed = t; } };
struct TIdle { int chosen; void ChooseNewIdle() { ++chosen; } };
struct TGroup { TThing* creature; TPtr Shop;
    void OnReplace() {
        if (Alive(Shop)) { TShop* s; Shop.GetPItem()->GetInterface(&s, 0x53); s->RemoveShopper(creature); }
        if (creature->HasTC(0xD3)) ((TIdle*)creature->Find(0xD3))->ChooseNewIdle(); } };
int main() {
    TThing me, shop; TShop ts; ts.removed = 0; shop.Add(0x53, &ts); TIdle idle; idle.chosen = 0;
    TGroup g; g.creature = &me;
    g.OnReplace(); CHECK(ts.removed == 0 && idle.chosen == 0, "00946e70");
    me.Add(0xD3, &idle); g.Shop.SetPItem(&shop); g.OnReplace(); CHECK(ts.removed == &me && idle.chosen == 1, "00946e70");
    ts.removed = 0; shop.flags91 = 1; g.OnReplace(); CHECK(ts.removed == 0 && idle.chosen == 2, "00946e70");
    std::printf("00946e70_TEST PASS\n"); return 0; }