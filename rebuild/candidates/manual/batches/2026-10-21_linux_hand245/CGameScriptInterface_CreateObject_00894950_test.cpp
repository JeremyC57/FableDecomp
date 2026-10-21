// Behaviour test for 00894950 CGameScriptInterface::CreateObject(defName, position, angle, scriptName): orientation from
// C3DVector(angle, 0, 0), player 4, null script thing on failure, counted script-thing handle released correctly.
#include <cstdio>
struct Info { long refs; };
struct ScriptThing { Info* info; ScriptThing() : info(0) {} ScriptThing(const ScriptThing& o) : info(o.info) { if (info) ++info->refs; }
    ScriptThing& operator=(const ScriptThing& o) { if (o.info) ++o.info->refs; if (info) --info->refs; info = o.info; return *this; }
    ~ScriptThing() { if (info) --info->refs; } };
struct Vec { float x, y, z; Vec(float a, float b, float c) : x(a), y(b), z(c) {} };
static Info g_info = { 1 };
static float g_angleSeen; static long g_playerSeen;
static bool Create(bool ok, const Vec& euler, long player) { g_angleSeen = euler.x; g_playerSeen = player; return ok; }
static ScriptThing Thing() { ScriptThing t; t.info = &g_info; ++g_info.refs; return t; }
static ScriptThing CreateObject(bool ok, float angle)
{
    Vec euler(angle, 0.0f, 0.0f);
    bool object = Create(ok, euler, 4);
    ScriptThing result;
    if (object) result = Thing();
    return result;
}
int main()
{
    long before = g_info.refs;
    bool ok;
    {
        ScriptThing a = CreateObject(true, 1.5f);
        ScriptThing b = CreateObject(false, 2.0f);
        ok = a.info == &g_info && b.info == 0 && g_info.refs == before + 1 && g_angleSeen == 2.0f && g_playerSeen == 4;
    }
    ok = ok && g_info.refs == before;
    if (!ok) { std::printf("00894950_TEST FAIL\n"); return 1; }
    std::printf("00894950_TEST PASS\n");
    return 0;
}
