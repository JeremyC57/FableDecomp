#include <cstdio>
struct CTCCameraPointDefinitionBase { bool CutInto; bool CutOutOf; };
struct CThing {
    bool dead; bool hasPoint; CTCCameraPointDefinitionBase* point;
    bool IsDead() const { return dead; }
    bool PeekTC(const CTCCameraPointDefinitionBase** out, long type) const
    {
        if (type == 0x3f && hasPoint) { *out = point; return true; }
        return false;
    }
};
struct CScriptModuleBase {
    CThing* camPoint; long CutOutOverride;
    CThing* GetPCameraPoint() const { return camPoint; }
    bool ResetCameraOnEnd() const
    {
        if (CutOutOverride != -1)
            return CutOutOverride == 1;
        if (GetPCameraPoint() && !GetPCameraPoint()->IsDead()) {
            const CTCCameraPointDefinitionBase* point;
            if (GetPCameraPoint()->PeekTC(&point, 0x3f) && point->CutOutOf)
                return true;
        }
        return false;
    }
};
int main()
{
    CTCCameraPointDefinitionBase def; def.CutInto = false; def.CutOutOf = true;
    CThing t; t.dead = false; t.hasPoint = true; t.point = &def;
    CScriptModuleBase m; m.camPoint = &t; m.CutOutOverride = 1;
    int bad = 0;
    if (!m.ResetCameraOnEnd()) bad = 1;
    m.CutOutOverride = 0; if (m.ResetCameraOnEnd()) bad = 2;
    m.CutOutOverride = -1; if (!m.ResetCameraOnEnd()) bad = 3;
    def.CutOutOf = false; if (m.ResetCameraOnEnd()) bad = 4;
    def.CutOutOf = true; t.dead = true; if (m.ResetCameraOnEnd()) bad = 5;
    t.dead = false; t.hasPoint = false; if (m.ResetCameraOnEnd()) bad = 6;
    m.camPoint = 0; if (m.ResetCameraOnEnd()) bad = 7;
    if (bad) { std::printf("00853fb0_TEST FAIL\n"); return 1; }
    std::printf("00853fb0_TEST PASS\n");
    return 0;
}