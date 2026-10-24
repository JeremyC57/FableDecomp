// Behaviour test for 0x006E0970 CTCDParticleEmitter::Create(name, position, force): no creation while the front-end
// definitions compile, none for an unknown (<= 0) template handle, otherwise the typed Create's result with the same
// position and force flag.
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CThing { int id; };

static bool g_bCompileFrontendDefinitions = false;
static CThing g_made;
static int g_calls, g_lastType; static bool g_lastForce; static const C3DVector* g_lastPos;

static long HandleFromName(const char* name) { return std::strcmp(name, "SPARK") == 0 ? 7 : 0; }
static CThing* CreateTyped(long type, const C3DVector& pos, bool force)
{
    ++g_calls; g_lastType = (int)type; g_lastPos = &pos; g_lastForce = force; return &g_made;
}
static CThing* CreateByName(const char* name, const C3DVector& pos, bool force)
{
    if (!g_bCompileFrontendDefinitions) {
        long id = HandleFromName(name);
        if (id > 0) return CreateTyped(id, pos, force);
    }
    return 0;
}

int main()
{
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    if (CreateByName("SPARK", p, true) != &g_made || g_lastType != 7 || g_lastPos != &p || !g_lastForce) { std::printf("006e0970_TEST FAIL\n"); return 1; }
    if (CreateByName("NOPE", p, false) != 0 || g_calls != 1) { std::printf("006e0970_TEST FAIL\n"); return 1; }
    g_bCompileFrontendDefinitions = true;
    if (CreateByName("SPARK", p, false) != 0 || g_calls != 1) { std::printf("006e0970_TEST FAIL\n"); return 1; }
    std::printf("006e0970_TEST PASS\n");
    return 0;
}