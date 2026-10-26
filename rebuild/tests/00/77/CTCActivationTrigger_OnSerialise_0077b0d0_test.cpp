// Behaviour test for CTCActivationTrigger::OnSerialise (0x0077B0D0).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

class CThing { public:
    u64 UID; short m_level; bool levelLess;
    bool IsLevelLess() { return levelLess; }
};
static int g_remapCalls = 0;
static bool g_lastSaving = false, g_lastLevelLess = false;
static long g_lastLevel = 0;
class CThingManager { public:
    u64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, u64 uid)
    { ++g_remapCalls; g_lastSaving = saving; g_lastLevelLess = levelLess; g_lastLevel = level; return uid + 0x100; }
};
static CThingManager g_tm;
static u64 g_loadedValue = 0;
static int g_transfers = 0;
class CPersistContext { public:
    long Mode;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, u64& value, const u64& defaultValue)
    {
        ++g_transfers;
        if (std::strcmp(name, "ReceptorUID") != 0 || defaultValue != 0) { g_transfers = 100; return; }
        if (!IsSaving()) value = g_loadedValue;
        else g_loadedValue = value;
    }
};
class CTCBase { public: void* vt; CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
class CTCActivationReceptorBase { public: CThing* ParentThing; };
class CTCActivationTrigger : public CTCBase {
public:
    CTCActivationReceptorBase* PTCReceptor; u64 ReceptorUID; bool Activated;
    CThingManager* GetThingManager() { return &g_tm; }
    void OnSerialise(CPersistContext& context, bool)
    {
        if (context.IsSaving()) {
            if (PTCReceptor == 0) {
                ReceptorUID = 0;
            } else {
                ReceptorUID = PTCReceptor->ParentThing->UID;
                ReceptorUID = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ReceptorUID);
            }
            context.Transfer("ReceptorUID", ReceptorUID, 0);
        } else {
            context.Transfer("ReceptorUID", ReceptorUID, 0);
            if (ReceptorUID > 0)
                ReceptorUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ReceptorUID);
        }
    }
};

int main()
{
    CThing owner; owner.UID = 1; owner.m_level = -3; owner.levelLess = true;
    CThing recThing; recThing.UID = ((u64)7 << 32) | 5; recThing.m_level = 0; recThing.levelLess = false;
    CTCActivationReceptorBase rec; rec.ParentThing = &recThing;
    CTCActivationTrigger t; t.ParentThing = &owner; t.PTCReceptor = &rec; t.ReceptorUID = 99; t.Activated = false;
    CPersistContext save; save.Mode = 3;
    bool ok = true;
    t.OnSerialise(save, false);
    ok = ok && t.ReceptorUID == recThing.UID + 0x100 && g_loadedValue == t.ReceptorUID && g_remapCalls == 1
        && g_lastSaving && g_lastLevelLess && g_lastLevel == -3 && g_transfers == 1;
    t.PTCReceptor = 0; save.Mode = 1;
    t.OnSerialise(save, false);
    ok = ok && t.ReceptorUID == 0 && g_loadedValue == 0 && g_remapCalls == 1 && g_transfers == 2;
    CPersistContext load; load.Mode = 2;
    g_loadedValue = 0; t.ReceptorUID = 55;
    t.OnSerialise(load, false);
    ok = ok && t.ReceptorUID == 0 && g_remapCalls == 1 && g_transfers == 3;
    g_loadedValue = (u64)1 << 40;
    t.OnSerialise(load, false);
    ok = ok && t.ReceptorUID == ((u64)1 << 40) + 0x100 && g_remapCalls == 2 && !g_lastSaving && g_transfers == 4;
    if (!ok) { std::printf("0077b0d0_TEST FAIL\n"); return 1; }
    std::printf("0077b0d0_TEST PASS\n");
    return 0;
}