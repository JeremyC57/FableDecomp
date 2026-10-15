#include <cstdio>
struct CThing {
    bool persisted; long removed; int removes;
    void RemoveTC(long type) { removed = type; ++removes; }
    void SetAsGameStatePersisted(bool p) { persisted = p; }
};
enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1, WALL_MOUNT_EFFECT_HEAL = 2 };
struct CWallMountEffectsDef { EWallMountEffects Effect; };
struct CTCWallMountEffects {
    CThing* ParentThing; const CWallMountEffectsDef* PDef; bool ObjectWasGamePersistant;
    void OnTakenOffMount()
    {
        if (PDef->Effect == WALL_MOUNT_EFFECT_TELEPORT) ParentThing->RemoveTC(0xDD);
        if (PDef->Effect != WALL_MOUNT_EFFECT_NONE && !ObjectWasGamePersistant) ParentThing->SetAsGameStatePersisted(false);
    }
};
int main()
{
    CThing t; t.persisted = true; t.removed = 0; t.removes = 0;
    CWallMountEffectsDef d; CTCWallMountEffects e; e.ParentThing = &t; e.PDef = &d;
    d.Effect = WALL_MOUNT_EFFECT_TELEPORT; e.ObjectWasGamePersistant = false; e.OnTakenOffMount();
    if (t.removes != 1 || t.removed != 0xDD || t.persisted) { std::printf("007df440_TEST FAIL\n"); return 1; }
    t.persisted = true; e.ObjectWasGamePersistant = true; d.Effect = WALL_MOUNT_EFFECT_HEAL; e.OnTakenOffMount();
    if (t.removes != 1 || !t.persisted) { std::printf("007df440_TEST FAIL\n"); return 1; }
    d.Effect = WALL_MOUNT_EFFECT_NONE; e.ObjectWasGamePersistant = false; e.OnTakenOffMount();
    if (t.removes != 1 || !t.persisted) { std::printf("007df440_TEST FAIL\n"); return 1; }
    std::printf("007df440_TEST PASS\n");
    return 0;
}
