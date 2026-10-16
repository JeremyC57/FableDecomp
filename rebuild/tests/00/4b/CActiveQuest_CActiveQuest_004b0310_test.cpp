// Behaviour test for 004b0310 (see src/004b0310.cpp): CActiveQuest ctor: counted pointers adopt non-null objects, fields set; plain C++98, member access only.
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

struct TBlock { long refs; void* obj; };
struct TCP { void* obj; TBlock* blk; TCP(void* o) : obj(o) { if (o) { blk = new TBlock; blk->refs = 1; blk->obj = obj; } else blk = 0; } ~TCP() { delete blk; } };
struct TQuest { long ID; void* info; TCP script; bool persistent; bool loaded; TCP data; const char* name;
    TQuest(long id, const char* n, void* i, void* s, void* d, bool l) : ID(id), info(i), script(s), persistent(true), loaded(l), data(d), name(n) { } };
int main() {
    int s, d; TQuest q(5, "Q", 0, &s, &d, true);
    CHECK(q.ID == 5 && q.persistent && q.loaded && q.script.blk && q.script.blk->refs == 1 && q.script.blk->obj == &s && q.data.blk->obj == &d, "004b0310");
    TQuest r(6, "R", 0, 0, 0, false); CHECK(r.script.blk == 0 && r.data.blk == 0 && !r.loaded, "004b0310");
    std::printf("004b0310_TEST PASS\n"); return 0; }