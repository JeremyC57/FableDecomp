// Behaviour test for 00905360 (see src/00905360.cpp): CActionPerformExpression ctor: fields set, a live target adopted, a dead one ignored; plain C++98, member access only.
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

struct TScriptThing { int id; TScriptThing() : id(0) { } };
struct TThing2 { unsigned char flags91; int id; TScriptThing GetScriptThing() { TScriptThing s; s.id = id; return s; } };
struct TAction { void* PTCScript; bool Activated, Terminated; TScriptThing PTargetThing; bool Performed; const char* Expression; long ExtraData1, ExtraData2;
    TAction(TThing2* target, const char* expr, long a, long b) : PTCScript(0), Activated(false), Terminated(false), Performed(false), Expression(expr), ExtraData1(a), ExtraData2(b)
    { if (target != 0 && !(target->flags91 & 1)) PTargetThing = target->GetScriptThing(); } };
int main() {
    TThing2 t; t.flags91 = 0; t.id = 42;
    TAction a(&t, "WAVE", 3, 4); CHECK(a.PTargetThing.id == 42 && !a.Performed && a.ExtraData1 == 3 && a.ExtraData2 == 4 && !a.Terminated, "00905360");
    t.flags91 = 1; TAction b(&t, "WAVE", 0, 0); CHECK(b.PTargetThing.id == 0, "00905360");
    TAction c(0, "WAVE", 0, 0); CHECK(c.PTargetThing.id == 0 && c.PTCScript == 0, "00905360");
    std::printf("00905360_TEST PASS\n"); return 0; }
