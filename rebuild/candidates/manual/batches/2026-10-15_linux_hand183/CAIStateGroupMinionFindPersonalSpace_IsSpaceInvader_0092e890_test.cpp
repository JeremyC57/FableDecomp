// Behaviour test for 0092e890 (see src/0092e890.cpp); plain C++98, member access only.
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
// FindPersonalSpace::IsSpaceInvader: live, motionless (|dp| <= 1e-4 per axis), not in creature mode 8 or 9
#include <math.h>
struct TMotion { float pos[3]; float last[3]; };
struct TModes { long mode; bool HasMode(long m) { return mode == m; } };
static bool IsSpaceInvader(TThing* t, TMotion* motion) {
    if (t != 0 && !(t->flags91 & 1)) {
        if ((float)fabs(motion->pos[0] - motion->last[0]) <= 1e-4f && (float)fabs(motion->pos[1] - motion->last[1]) <= 1e-4f &&
            (float)fabs(motion->pos[2] - motion->last[2]) <= 1e-4f && t->HasTC(0x31)) {
            TModes* m = (TModes*)t->Find(0x31);
            if (!m->HasMode(8) && !m->HasMode(9)) return true;
        }
    }
    return false; }
int main() {
    TThing t; TModes m; m.mode = 0; t.Add(0x31, &m);
    TMotion mo; for (int i = 0; i < 3; ++i) { mo.pos[i] = 1.0f; mo.last[i] = 1.0f; }
    CHECK(IsSpaceInvader(&t, &mo), "0092e890");
    m.mode = 9; CHECK(!IsSpaceInvader(&t, &mo), "0092e890");
    m.mode = 0; mo.pos[2] = 2.0f; CHECK(!IsSpaceInvader(&t, &mo), "0092e890");
    CHECK(!IsSpaceInvader(0, &mo), "0092e890");
    std::printf("0092e890_TEST PASS\n"); return 0; }
