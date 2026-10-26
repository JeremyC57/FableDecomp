#include <cstdio>
// behaviour test: the region exit saves a remapped copy of the connected entrance UID (0 stays 0, member untouched) and on load
// remaps a non-zero loaded UID in place
typedef unsigned long long u64;
class CPersistContext { public:
    long Mode; u64 stored; int transfers;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, bool& v, const bool& d) { ++transfers; if (!IsSaving()) v = d; }
    void Transfer(const char*, float& v, const float& d) { ++transfers; if (!IsSaving()) v = d; }
    void Transfer(const char*, u64& v, const u64&) { ++transfers; if (IsSaving()) stored = v; else v = stored; }
};
static u64 Remap(bool saving, u64 uid) { return saving ? uid + 1000 : uid - 1000; }
class CTCDRegionExit { public:
    bool Active; float Radius, MessageRadius; u64 EntranceConnectedToUID; bool ReversedOnMiniMap, HiddenOnMiniMap;
    void OnSerialise(CPersistContext& context)
    {
        context.Transfer("Active", Active, true);
        context.Transfer("Radius", Radius, 0.0f);
        context.Transfer("MessageRadius", MessageRadius, 0.0f);
        context.Transfer("ReversedOnMiniMap", ReversedOnMiniMap, false);
        context.Transfer("HiddenOnMiniMap", HiddenOnMiniMap, false);
        if (context.IsSaving()) {
            u64 uid = 0;
            if (EntranceConnectedToUID != 0)
                uid = Remap(true, EntranceConnectedToUID);
            context.Transfer("EntranceConnectedToUID", uid, 0);
        } else {
            context.Transfer("EntranceConnectedToUID", EntranceConnectedToUID, 0);
            if (EntranceConnectedToUID != 0)
                EntranceConnectedToUID = Remap(false, EntranceConnectedToUID);
        }
    }
};
int main()
{
    CPersistContext save = { 3, 99, 0 };
    CTCDRegionExit r = { false, 2.0f, 3.0f, 5, false, false };
    r.OnSerialise(save);
    bool ok = save.transfers == 6 && save.stored == 1005 && r.EntranceConnectedToUID == 5;
    r.EntranceConnectedToUID = 0; r.OnSerialise(save);
    ok = ok && save.stored == 0;
    CPersistContext load = { 0, 1007, 0 };
    r.OnSerialise(load);
    ok = ok && r.EntranceConnectedToUID == 7 && r.Active && r.Radius == 0.0f;
    load.stored = 0; r.OnSerialise(load);
    ok = ok && r.EntranceConnectedToUID == 0;
    if (!ok) { std::printf("00779430_TEST FAIL\n"); return 1; }
    std::printf("00779430_TEST PASS\n");
    return 0;
}