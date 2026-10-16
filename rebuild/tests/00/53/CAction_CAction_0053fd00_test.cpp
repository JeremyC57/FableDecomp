// Behaviour test for 0053fd00 (see src/0053fd00.cpp): CAction::~CAction: owned elements deleted, both lists freed; plain C++98, member access only.
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

static int g_deleted = 0, g_nodesFreed = 0;
struct TElem { long ownership; virtual ~TElem() { ++g_deleted; } };
template<class T> struct TList { struct N { N* next; N* prev; T value; }; N* head;
    TList() { head = new N; head->next = head; head->prev = head; }
    void Push(T v) { N* n = new N; n->value = v; n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
    void clear() { N* n = head->next; while (n != head) { N* d = n; n = n->next; delete d; ++g_nodesFreed; } head->next = head; head->prev = head; }
    ~TList() { clear(); if (head) { delete head; ++g_nodesFreed; } } };
struct TAction { long u; TList<TElem*> elements; TList<void*> pending;
    ~TAction() { for (TList<TElem*>::N* n = elements.head->next; n != elements.head; n = n->next) if (n->value->ownership != 1) delete n->value; } };
int main() {
    TElem keep; keep.ownership = 1;
    {
        TAction* a = new TAction; TElem* own = new TElem; own->ownership = 0;
        a->elements.Push(own); a->elements.Push(&keep); a->pending.Push(0); a->pending.Push(0); a->pending.Push(0);
        delete a;
    }
    CHECK(g_deleted == 1 && g_nodesFreed == 7, "0053fd00");
    std::printf("0053fd00_TEST PASS\n"); return 0; }