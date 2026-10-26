// Behaviour test for CTCCameraPointScripted::OnSerialseLeaf (0x0075D320): load raises the set flags, LookPosEnd defaults
// to LookPos, the UID is remapped in place on load and only transferred when non-zero on save.
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_floatCalls = 0, g_uidCalls = 0;
static u64 g_lastUidSaved = 0;
static bool g_lastRemapSaving = false;

class CThing { public: short m_level; bool IsLevelLess() { return m_level == 0; } };
class CThingManager { public:
    u64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, u64 uid)
    { g_lastRemapSaving = saving; return saving ? uid + 100 + level + (levelLess ? 1 : 0) : uid * 2; }
};
static CThingManager g_mgr;
class CPersistContext { public:
    long Mode;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, float& value, const float& def) { ++g_floatCalls; if (!IsSaving()) value = def; }
    void Transfer(const char*, u64& value, const u64& def) { ++g_uidCalls; if (IsSaving()) g_lastUidSaved = value; else value = def + 7; }
};
class C3DVector { public: float X, Y, Z; };
class CTCCameraPointScripted { public:
    CThing* ParentThing;
    C3DVector StartPos, EndPos, LookPos, LookPosEnd;
    u64 ThingUID;
    float TransitionTime;
    bool LookDirSet, StartPosSet, EndPosSet;
    CThingManager* GetThingManager() { return &g_mgr; }
    void OnSerialseLeaf(CPersistContext& context)
    {
        if (!context.IsSaving()) { LookDirSet = true; StartPosSet = true; EndPosSet = true; }
        context.Transfer("LookDirection.X", LookPos.X, 0.0f);
        context.Transfer("LookDirection.Y", LookPos.Y, 0.0f);
        context.Transfer("LookDirection.Z", LookPos.Z, 0.0f);
        context.Transfer("LookDirectionEnd.X", LookPosEnd.X, LookPos.X);
        context.Transfer("LookDirectionEnd.Y", LookPosEnd.Y, LookPos.Y);
        context.Transfer("LookDirectionEnd.Z", LookPosEnd.Z, LookPos.Z);
        context.Transfer("StartPos.X", StartPos.X, 0.0f);
        context.Transfer("StartPos.Y", StartPos.Y, 0.0f);
        context.Transfer("StartPos.Z", StartPos.Z, 0.0f);
        context.Transfer("EndPos.X", EndPos.X, 0.0f);
        context.Transfer("EndPos.Y", EndPos.Y, 0.0f);
        context.Transfer("EndPos.Z", EndPos.Z, 0.0f);
        context.Transfer("TransitionTime", TransitionTime, 0.0f);
        if (context.IsSaving()) {
            if (ThingUID != 0) {
                u64 uid = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ThingUID);
                context.Transfer("ThingUID", uid, 0);
            }
        } else {
            context.Transfer("ThingUID", ThingUID, 0);
            if (ThingUID != 0)
                ThingUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ThingUID);
        }
    }
};

int main()
{
    CThing thing; thing.m_level = 3;
    CTCCameraPointScripted cp;
    std::memset(&cp, 0, sizeof(cp));
    cp.ParentThing = &thing;
    CPersistContext load; load.Mode = 2;
    cp.OnSerialseLeaf(load);
    if (!cp.LookDirSet || !cp.StartPosSet || !cp.EndPosSet || g_floatCalls != 13 || g_uidCalls != 1
        || cp.ThingUID != 14 || g_lastRemapSaving) { std::printf("0075d320_TEST FAIL\n"); return 1; }
    CTCCameraPointScripted s;
    std::memset(&s, 0, sizeof(s));
    s.ParentThing = &thing;
    CPersistContext save; save.Mode = 1;
    s.OnSerialseLeaf(save);
    if (s.LookDirSet || g_uidCalls != 1) { std::printf("0075d320_TEST FAIL\n"); return 1; }
    s.ThingUID = 5;
    save.Mode = 3;
    s.OnSerialseLeaf(save);
    if (g_uidCalls != 2 || g_lastUidSaved != 108 || !g_lastRemapSaving || s.ThingUID != 5) { std::printf("0075d320_TEST FAIL\n"); return 1; }
    std::printf("0075d320_TEST PASS\n");
    return 0;
}