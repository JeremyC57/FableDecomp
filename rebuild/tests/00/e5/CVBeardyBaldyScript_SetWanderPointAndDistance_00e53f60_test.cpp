// Behaviour test for CV_BeardyBaldyScript::SetWanderPointAndDistance (0x00E53F60): the four interface calls in order,
// each with a by-value copy of the thing (refcount bumped per copy) and the def's wander distances.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
struct CCPPointerInfo { long RefCount; };
struct CScriptThing {
    void* Data;
    CCPPointerInfo* Info;
    CScriptThing(void* d, CCPPointerInfo* i) : Data(d), Info(i) {}
    CScriptThing(const CScriptThing& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CScriptThing() {}                      // callee-owned copies: leak-count observable by the test
};
struct CGameScriptInterface {
    virtual void SetWanderCentrePoint(CScriptThing thing, C3DVector point) = 0;
    virtual void SetWanderMinDistance(CScriptThing thing, float distance) = 0;
    virtual void SetWanderMaxDistance(CScriptThing thing, float distance) = 0;
    virtual void SetScriptingStateGroup(CScriptThing thing, long group) = 0;
    virtual ~CGameScriptInterface() {}
};
struct CScriptDef { float WanderMinDistance, WanderMaxDistance; };
static CScriptDef g_def = { 2.5f, 9.0f };
static CScriptDef* g_pScriptDef = &g_def;
struct CV_BeardyBaldyScript {
    CGameScriptInterface* GSI;
    void SetWanderPointAndDistance(const CScriptThing& thing, const C3DVector& point)
    {
        GSI->SetWanderCentrePoint(thing, point);
        GSI->SetWanderMinDistance(thing, g_pScriptDef->WanderMinDistance);
        GSI->SetWanderMaxDistance(thing, g_pScriptDef->WanderMaxDistance);
        GSI->SetScriptingStateGroup(thing, 4);
    }
};
static int g_log[8]; static int g_n = 0; static float g_f[4]; static void* g_data[4]; static long g_group = 0;
struct Mock : CGameScriptInterface {
    void SetWanderCentrePoint(CScriptThing t, C3DVector p) { g_data[g_n] = t.Data; g_log[g_n++] = 1; g_f[0] = p.X + p.Y + p.Z; }
    void SetWanderMinDistance(CScriptThing t, float d) { g_data[g_n] = t.Data; g_log[g_n++] = 2; g_f[1] = d; }
    void SetWanderMaxDistance(CScriptThing t, float d) { g_data[g_n] = t.Data; g_log[g_n++] = 3; g_f[2] = d; }
    void SetScriptingStateGroup(CScriptThing t, long g) { g_data[g_n] = t.Data; g_log[g_n++] = 4; g_group = g; }
};
int main()
{
    Mock m; CV_BeardyBaldyScript s; s.GSI = &m;
    int obj = 0; CCPPointerInfo info = { 1 };
    CScriptThing thing(&obj, &info);
    C3DVector p(1.0f, 2.0f, 3.0f);
    s.SetWanderPointAndDistance(thing, p);
    bool ok = g_n == 4 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[3] == 4;
    float sum = 6.0f, mn = 2.5f, mx = 9.0f;
    ok = ok && g_f[0] == sum && g_f[1] == mn && g_f[2] == mx && g_group == 4 && info.RefCount == 5;
    for (int i = 0; i < 4; ++i) ok = ok && g_data[i] == &obj;
    if (!ok) { std::printf("00e53f60_TEST FAIL\n"); return 1; }
    std::printf("00e53f60_TEST PASS\n");
    return 0;
}