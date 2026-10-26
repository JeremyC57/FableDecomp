#include <cstdio>
#include <cstring>
static int g_strings = 0; static const char* g_lastName = 0;
class CCharString { public: const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) { ++g_strings; } ~CCharString() { --g_strings; } };
class CTCBase; class CTCParamsBase;
class CThing { public:
    bool persisted; int added; bool lastActivate;
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase*) { ++added; g_lastName = name.m_pData; lastActivate = activate; return 0; }
    bool IsGameStatePersisted() const { return persisted; }
    void SetAsGameStatePersisted(bool p) { persisted = p; }
};
enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1, WALL_MOUNT_EFFECT_HEAL = 2 };
class CWallMountEffectsDef { public: EWallMountEffects Effect; };
class CTCWallMountEffects { public:
    CThing* ParentThing; const CWallMountEffectsDef* PDef; bool ObjectWasGamePersistant;
    void OnPutOnMount()
    {
        if (PDef->Effect == WALL_MOUNT_EFFECT_TELEPORT) ParentThing->AddTC("CTCTeleporter", false, 0);
        if (PDef->Effect != WALL_MOUNT_EFFECT_NONE) { ObjectWasGamePersistant = ParentThing->IsGameStatePersisted(); ParentThing->SetAsGameStatePersisted(true); }
    }
};
int main()
{
    CThing t; t.persisted = false; t.added = 0; t.lastActivate = true;
    CWallMountEffectsDef d; CTCWallMountEffects e; e.ParentThing = &t; e.PDef = &d; e.ObjectWasGamePersistant = true;
    d.Effect = WALL_MOUNT_EFFECT_TELEPORT; e.OnPutOnMount();
    if (t.added != 1 || std::strcmp(g_lastName, "CTCTeleporter") != 0 || t.lastActivate || g_strings != 0 || !t.persisted || e.ObjectWasGamePersistant) { std::printf("007df3e0_TEST FAIL\n"); return 1; }
    d.Effect = WALL_MOUNT_EFFECT_HEAL; e.OnPutOnMount();
    if (t.added != 1 || !e.ObjectWasGamePersistant || !t.persisted) { std::printf("007df3e0_TEST FAIL\n"); return 1; }
    t.persisted = false; e.ObjectWasGamePersistant = true; d.Effect = WALL_MOUNT_EFFECT_NONE; e.OnPutOnMount();
    if (t.added != 1 || t.persisted || !e.ObjectWasGamePersistant) { std::printf("007df3e0_TEST FAIL\n"); return 1; }
    std::printf("007df3e0_TEST PASS\n");
    return 0;
}