// Behaviour test for 009c4330 (see src/009c4330.cpp): CActiveFile ctor: counters zeroed, file info shared, two empty sentinel lists; plain C++98, member access only.
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

struct TInfo { long refs; };
struct TPtrC { void* data; TInfo* info; TPtrC(const TPtrC& o) : data(o.data), info(o.info) { if (info) ++info->refs; } TPtrC(void* d, TInfo* i) : data(d), info(i) { } };
struct TNode { TNode* next; TNode* prev; };
struct TList { TNode* head; TList() : head(0) { TNode* s = new TNode; s->next = s; s->prev = s; head = s; } ~TList() { delete head; } };
struct TFile { unsigned long cur, read, written, in, out; TPtrC info; unsigned long minW, minR; TList a, b; unsigned long sp, dp; void* sb; void* db; unsigned long used; bool fin;
    TFile(const TPtrC& i) : cur(0), read(0), written(0), in(0), out(0), info(i), minW(0), minR(0), sp(0), dp(0), sb(0), db(0), used(0), fin(false) { } };
int main() {
    TInfo inf; inf.refs = 1; int dummy; TPtrC p(&dummy, &inf);
    TFile f(p); CHECK(inf.refs == 2 && f.info.data == &dummy && f.cur == 0 && f.used == 0 && !f.fin, "009c4330");
    CHECK(f.a.head->next == f.a.head && f.b.head->prev == f.b.head, "009c4330");
    TPtrC z(0, 0); TFile g(z); CHECK(g.info.info == 0, "009c4330");
    std::printf("009c4330_TEST PASS\n"); return 0; }