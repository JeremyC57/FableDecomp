// Behaviour test for 009239c0 (see src/009239c0.cpp); plain C++98, member access only.
#include <cstdio>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    TThing() : count(0), flags91(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
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
inline bool AliveT(const TThing* t) { return t != 0 && !(t->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)
// ServeFood::OnReplace: release the place, drop/destroy live carried food and notify the table, reset indices
struct TTable { bool Occupied; long released; long served; };
struct TCarry { TThing* food; };
static int kills = 0;
struct TGroup { TThing* creature; TThing* PTable; long TaskIndex; long PlaceIndex;
    void OnReplace() {
        if (PTable != 0) {
            TTable* t = 0; PTable->GetInterface(&t, 0xb8); t->released = PlaceIndex; t->Occupied = false;
            TCarry* c = 0; creature->GetInterface(&c, 0x46); TThing* food = c->food;
            if (food != 0 && !(food->flags91 & 1)) { c->food = 0; ++kills; t->served = TaskIndex; }
            TaskIndex = -1; PlaceIndex = -1; PTable = 0; } } };
int main() {
    TThing me, table, stew; TTable t; t.Occupied = true; t.released = 0; t.served = 0; table.Add(0xb8, &t);
    TCarry c; c.food = &stew; me.Add(0x46, &c);
    TGroup g; g.creature = &me; g.PTable = &table; g.TaskIndex = 3; g.PlaceIndex = 5;
    g.OnReplace(); CHECK(!t.Occupied && t.released == 5 && t.served == 3 && kills == 1 && c.food == 0, "009239c0");
    CHECK(g.TaskIndex == -1 && g.PlaceIndex == -1 && g.PTable == 0, "009239c0");
    g.OnReplace(); CHECK(kills == 1, "009239c0");
    std::printf("009239c0_TEST PASS\n"); return 0; }