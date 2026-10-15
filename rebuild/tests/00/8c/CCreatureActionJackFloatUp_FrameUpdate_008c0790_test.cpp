// Behaviour test for 0x008C0790 (see src/008c0790.cpp).
#include <cstdio>
#include <cstring>

// Interface table model shared by the GetInterface-based rows: presence bits + sorted {type, component} entries.
struct CEntry { int type; void* component; };
struct CThing {
    unsigned int presentBits[9];
    CEntry entries[4];
    int count;
    unsigned char flags91;                  // bit 0: dead
    unsigned char state10;
    void* cached5c;                         // the cached driver component
    unsigned char driverFlags28;
    CThing() { std::memset(presentBits, 0, sizeof(presentBits)); count = 0; flags91 = 0; state10 = 0; cached5c = 0; driverFlags28 = 0; }
    void Add(int type, void* c) { presentBits[type >> 5] |= 1u << (type & 31); entries[count].type = type; entries[count].component = c; ++count; }
    template<class T> bool GetInterface(T** out, int type)
    {
        if (presentBits[type >> 5] & (1u << (type & 31))) {
            CEntry* it = entries;
            CEntry* last = entries + count;
            while (it != last && it->type < type) ++it;
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
inline bool IS_THING_ALIVE(CThing* p) { return p != 0 && !(p->flags91 & 1); }
static int g_log[32];
static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { if (n != g_n) return false; for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
static void Reset() { g_n = 0; }

#define CHECK(c) do { if (!(c)) { std::printf("008c0790_TEST FAIL\n"); return 1; } } while (0)

struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static long g_frameRate = 10;
struct CAction { C3DVector pos; float FloatHeight, FloatSpeed, StartHeight; bool Moving;
    CAction() : pos(1.0f, 2.0f, 3.0f) {}
    void BaseFrameUpdate() { Log(1); }
    void FrameUpdate()
    {
        BaseFrameUpdate();
        if (Moving) {
            C3DVector rise(0.0f, 0.0f, FloatSpeed / (float)g_frameRate);
            const C3DVector& p = pos;
            C3DVector target(p.X, p.Y, p.Z + rise.Z);
            float limit = FloatHeight + StartHeight;
            if (target.Z > limit) target.Z = limit;
            pos = target;
        }
    } };
int main()
{
    CAction a; a.FloatHeight = 2.0f; a.StartHeight = 3.0f; a.FloatSpeed = 5.0f; a.Moving = true;
    a.FrameUpdate(); CHECK(a.pos.X == 1.0f && a.pos.Y == 2.0f && a.pos.Z == 3.5f);
    for (int i = 0; i < 10; ++i) a.FrameUpdate();
    CHECK(a.pos.Z == 5.0f);                                   // clamped to StartHeight + FloatHeight
    a.Moving = false; a.pos.Z = 0.0f; a.FrameUpdate(); CHECK(a.pos.Z == 0.0f);
    std::printf("008c0790_TEST PASS\n");
    return 0;
}