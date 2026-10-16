// Behaviour test for 0093c6d0 (see src/0093c6d0.cpp); plain C++98, member access only.
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
// StealthSeekEnemy::SetWhatsThatAnimation: finish unless the animation completed, still runnable and target unmoved
static long result = 2; static bool runnable = true; static int finished = 0; static float eps = 1e-4f;
struct TPerceive { float p[3]; };
struct TGroup { float last[3];
    void Run(TPerceive* per) {
        if (result == 2 && runnable) {
            float d0 = per->p[0] - last[0], d1 = per->p[1] - last[1], d2 = per->p[2] - last[2];
            if (!(d0 * d0 + d1 * d1 + d2 * d2 > eps * eps)) return; }
        ++finished; } };
int main() {
    TGroup g; TPerceive per; for (int i = 0; i < 3; ++i) { g.last[i] = 5.0f; per.p[i] = 5.0f; }
    g.Run(&per); CHECK(finished == 0, "0093c6d0");
    per.p[1] = 6.0f; g.Run(&per); CHECK(finished == 1, "0093c6d0");
    per.p[1] = 5.0f; result = 1; g.Run(&per); CHECK(finished == 2, "0093c6d0");
    std::printf("0093c6d0_TEST PASS\n"); return 0; }
