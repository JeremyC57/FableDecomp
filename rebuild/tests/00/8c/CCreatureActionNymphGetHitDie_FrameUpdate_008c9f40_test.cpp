// Behaviour test for 0x008C9F40 (see src/008c9f40.cpp).
#include <cstdio>
#include <cstring>

// Interface table model shared by the GetInterface-based rows: presence bits + sorted {type, component} entries.
struct CEntry { int type; void* component; };
class CThing { public:
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

#define CHECK(c) do { if (!(c)) { std::printf("008c9f40_TEST FAIL\n"); return 1; } } while (0)

class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CWorldMap { public: float ground; float GetHeightZBelowPos(const C3DVector* p) { return ground + p->X * 0.0f; } };
struct CAction { C3DVector pos; float progress; float HeightOnInit; CWorldMap map;
    CAction() : pos(4.0f, 5.0f, 9.0f) {}
    void BaseFrameUpdate() { Log(1); }
    void FrameUpdate()
    {
        BaseFrameUpdate();
        C3DVector position = pos;
        float height = HeightOnInit * (1.0f - progress);
        position.Z = map.GetHeightZBelowPos(&position) + height;
        pos = position;
    } };
int main()
{
    CAction a; a.map.ground = 1.0f; a.HeightOnInit = 4.0f; a.progress = 0.25f;
    a.FrameUpdate(); CHECK(a.pos.X == 4.0f && a.pos.Y == 5.0f && a.pos.Z == 4.0f);
    a.progress = 1.0f; a.FrameUpdate(); CHECK(a.pos.Z == 1.0f);
    std::printf("008c9f40_TEST PASS\n");
    return 0;
}