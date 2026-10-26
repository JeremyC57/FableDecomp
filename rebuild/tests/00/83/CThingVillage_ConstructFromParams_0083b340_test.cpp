// Behaviour test for CThingVillage::ConstructFromParams (0x0083B340): forwards to the game-object base with an empty name.
#include <cstdio>
#include <cstring>
class C3DVector { public: float x, y, z; };
class CThingPhysical;
static int g_live = 0;
class CCharString { public:
    const char* text;
    CCharString(const char* t, long = -1) : text(t) { ++g_live; }
    ~CCharString() { --g_live; }
};
static long g_def, g_player; static unsigned long g_flags; static const C3DVector* g_pos;
static bool g_nameEmpty, g_last, g_result = true; static void* g_c1; static void* g_c2; static int g_liveInside;
class CThingGameObject {
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long flags, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8)
    {
        g_def = defIndex; g_pos = &pos; g_player = player; g_flags = flags; g_nameEmpty = std::strlen(scriptName.text) == 0;
        g_c1 = creator; g_c2 = originalCreator; g_last = a8; g_liveInside = g_live;
        return g_result;
    }
};
class CThingVillage : public CThingGameObject {
public:
    bool Construct(long defIndex, const C3DVector& pos, long player)
    {
        bool ok;
        {
            CCharString name("");
            ok = CThingGameObject::ConstructFromParams(defIndex, pos, player, 0, name, 0, 0, true);
        }
        return ok;
    }
};
int main()
{
    CThingVillage v; C3DVector p = { 0, 0, 0 };
    if (!v.Construct(7, p, 3) || g_def != 7 || g_pos != &p || g_player != 3 || g_flags != 0 || !g_nameEmpty || g_c1 || g_c2 || !g_last || g_liveInside != 1 || g_live != 0) { std::printf("0083b340_TEST FAIL\n"); return 1; }
    g_result = false;
    if (v.Construct(7, p, 3) || g_live != 0) { std::printf("0083b340_TEST FAIL\n"); return 1; }
    std::printf("0083b340_TEST PASS\n");
    return 0;
}