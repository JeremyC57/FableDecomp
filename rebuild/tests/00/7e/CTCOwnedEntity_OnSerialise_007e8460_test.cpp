// Behaviour test for CTCOwnedEntity::OnSerialise (0x007E8460).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 UID64;
#else
typedef unsigned long long UID64;
#endif

class CThing { public: short m_level; bool LevelLess; bool IsLevelLess() { return LevelLess; } };
class CThingManager { public:
    int Calls; bool LastSaving;
    UID64 RemapUIDForSerialisation(bool saving, bool, long level, UID64 uid)
    { ++Calls; LastSaving = saving; return saving ? uid + 1000 + level : uid - 1000 - level; }
};
static CThingManager g_mgr;
class CPersistContext { public:
    long Mode;
    // a tiny "file": key -> value
    long Version; UID64 Owner; bool Switch; int Count;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& v, const bool&) { ++Count; if (std::strcmp(name, "SwitchableNavigationTCAdded") == 0) { if (IsSaving()) Switch = v; else v = Switch; } }
    void Transfer(const char* name, long& v, const long&) { ++Count; if (std::strcmp(name, "VersionNumber") == 0) { if (IsSaving()) Version = v; else v = Version; } }
    void Transfer(const char* name, UID64& v, const UID64&) { ++Count; if (std::strcmp(name, "OwnerUID") == 0) { if (IsSaving()) Owner = v; else v = Owner; } }
};
class CTCOwnedEntity { public:
    CThing* ParentThing;
    bool SwitchableNavigationTCAdded;
    UID64 OwnerUID;
    CThingManager* GetThingManager() { return &g_mgr; }
    void OnSerialise(CPersistContext& context, bool)
    {
        context.Transfer("SwitchableNavigationTCAdded", SwitchableNavigationTCAdded, false);
        if (context.IsSaving()) {
            long version = 1;
            context.Transfer("VersionNumber", version, 0L);
            UID64 uid = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, OwnerUID);
            context.Transfer("OwnerUID", uid, 0);
        } else {
            long version;
            context.Transfer("VersionNumber", version, 0L);
            context.Transfer("OwnerUID", OwnerUID, 0);
            if (version == 1 && OwnerUID != 0)
                OwnerUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, OwnerUID);
        }
    }
};

int main()
{
    CThing thing; thing.m_level = 3; thing.LevelLess = false;
    CTCOwnedEntity e; e.ParentThing = &thing; e.SwitchableNavigationTCAdded = true; e.OwnerUID = 500;
    CPersistContext file; std::memset(&file, 0, sizeof(file)); file.Mode = 1;
    e.OnSerialise(file, false);
    bool ok = file.Count == 3 && file.Version == 1 && file.Owner == 1503 && file.Switch && e.OwnerUID == 500 && g_mgr.LastSaving;
    CTCOwnedEntity l; l.ParentThing = &thing; l.SwitchableNavigationTCAdded = false; l.OwnerUID = 0;
    file.Mode = 2; file.Count = 0;
    l.OnSerialise(file, true);
    ok = ok && file.Count == 3 && l.SwitchableNavigationTCAdded && l.OwnerUID == 500 && !g_mgr.LastSaving && g_mgr.Calls == 2;
    file.Owner = 0; l.OnSerialise(file, true);           // a null owner is not remapped
    ok = ok && l.OwnerUID == 0 && g_mgr.Calls == 2;
    if (!ok) { std::printf("007e8460_TEST FAIL\n"); return 1; }
    std::printf("007e8460_TEST PASS\n");
    return 0;
}