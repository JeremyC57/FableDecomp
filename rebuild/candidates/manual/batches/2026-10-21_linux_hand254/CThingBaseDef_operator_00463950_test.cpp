// Behaviour test for CThingBaseDef::operator= (0x00463950): the parent assignment and the component vector copy run
// (the copy only for distinct objects), every one-bit flag and every trailing field is copied, *this is returned.
#include <cstdio>

static int g_parentCalls = 0, g_copyCalls = 0;

struct CParentDefClassBase {
    unsigned char _body[0x39];
    CParentDefClassBase& operator=(const CParentDefClassBase&) { ++g_parentCalls; return *this; }
};

struct CThingComponentSetEntry;
struct CSmallVectorComponentEntries {
    CThingComponentSetEntry* Start;
    signed char Size;
    void Copy(const CSmallVectorComponentEntries& other) { ++g_copyCalls; Start = other.Start; Size = other.Size; }
    CSmallVectorComponentEntries& operator=(const CSmallVectorComponentEntries& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};

struct CThingComponentSet { CSmallVectorComponentEntries Entries; };
struct CDefString { long Index; };

struct CThingBaseDef : CParentDefClassBase {
    CThingComponentSet Components;
    bool AvailableInEditor : 1; bool AlwaysFrameUpdate : 1; bool StartKillLocked : 1; bool LevelLess : 1;
    bool IsDrawable : 1; bool IsDrawableInEditor : 1; bool ForceNoSerialise : 1; bool InMapSearches : 1;
    bool IsLoadableGlobal : 1; bool GameStatePersisted : 1; bool CanComeBetweenCameraAndObservedThing : 1;
    bool DrawWeaponTrails : 1; bool DrawProjectileWeaponTrails : 1; bool IsVulnerableToMelee : 1; bool Damageable : 1;
    bool AlwaysIncludeInObjectStrikeScans : 1;
    bool DisableOcclusionTesting : 1; bool DrawAfterWater : 1; bool ForceRenderedLastFrameOverride : 1;
    bool AddToComboMultiplierOnHit : 1; bool GiveHeroStatChangesOnBeingHit : 1;
    long GroupDef; float RenderFadeDistance; long PersistenceFlags; float Health; float MinHealth;
    float MeshHeightOffset; float MeshHeight; float MeshRadius; float ApproxMaxMeshHeight;
    CDefString DefaultScriptName; CDefString DefaultScriptData;
};

int main()
{
    CThingBaseDef a, b;
    a.AvailableInEditor = true; a.AlwaysFrameUpdate = false; a.StartKillLocked = true; a.LevelLess = false;
    a.IsDrawable = true; a.IsDrawableInEditor = true; a.ForceNoSerialise = false; a.InMapSearches = true;
    a.IsLoadableGlobal = false; a.GameStatePersisted = true; a.CanComeBetweenCameraAndObservedThing = true;
    a.DrawWeaponTrails = false; a.DrawProjectileWeaponTrails = true; a.IsVulnerableToMelee = false; a.Damageable = true;
    a.AlwaysIncludeInObjectStrikeScans = true; a.DisableOcclusionTesting = true; a.DrawAfterWater = false;
    a.ForceRenderedLastFrameOverride = true; a.AddToComboMultiplierOnHit = false; a.GiveHeroStatChangesOnBeingHit = true;
    a.GroupDef = 7; a.RenderFadeDistance = 50.0f; a.PersistenceFlags = 3; a.Health = 100.0f; a.MinHealth = 1.0f;
    a.MeshHeightOffset = 0.5f; a.MeshHeight = 2.0f; a.MeshRadius = 0.75f; a.ApproxMaxMeshHeight = 2.5f;
    a.DefaultScriptName.Index = 11; a.DefaultScriptData.Index = 12;
    a.Components.Entries.Start = 0; a.Components.Entries.Size = 4;
    b.AvailableInEditor = false; b.AlwaysFrameUpdate = true; b.StartKillLocked = false; b.LevelLess = true;
    b.IsDrawable = false; b.IsDrawableInEditor = false; b.ForceNoSerialise = true; b.InMapSearches = false;
    b.IsLoadableGlobal = true; b.GameStatePersisted = false; b.CanComeBetweenCameraAndObservedThing = false;
    b.DrawWeaponTrails = true; b.DrawProjectileWeaponTrails = false; b.IsVulnerableToMelee = true; b.Damageable = false;
    b.AlwaysIncludeInObjectStrikeScans = false; b.DisableOcclusionTesting = false; b.DrawAfterWater = true;
    b.ForceRenderedLastFrameOverride = false; b.AddToComboMultiplierOnHit = true; b.GiveHeroStatChangesOnBeingHit = false;
    b.Components.Entries.Size = 0;
    CThingBaseDef* r = &(b = a);
    int ok = r == &b && g_parentCalls == 1 && g_copyCalls == 1 && b.Components.Entries.Size == 4;
    ok = ok && b.AvailableInEditor && !b.AlwaysFrameUpdate && b.StartKillLocked && !b.LevelLess && b.IsDrawable
         && b.IsDrawableInEditor && !b.ForceNoSerialise && b.InMapSearches && !b.IsLoadableGlobal && b.GameStatePersisted
         && b.CanComeBetweenCameraAndObservedThing && !b.DrawWeaponTrails && b.DrawProjectileWeaponTrails
         && !b.IsVulnerableToMelee && b.Damageable && b.AlwaysIncludeInObjectStrikeScans && b.DisableOcclusionTesting
         && !b.DrawAfterWater && b.ForceRenderedLastFrameOverride && !b.AddToComboMultiplierOnHit
         && b.GiveHeroStatChangesOnBeingHit;
    ok = ok && b.GroupDef == 7 && b.RenderFadeDistance == 50.0f && b.PersistenceFlags == 3 && b.Health == 100.0f
         && b.MinHealth == 1.0f && b.MeshHeightOffset == 0.5f && b.MeshHeight == 2.0f && b.MeshRadius == 0.75f
         && b.ApproxMaxMeshHeight == 2.5f && b.DefaultScriptName.Index == 11 && b.DefaultScriptData.Index == 12;
    b = b;                                                       // self-assignment skips the vector copy
    ok = ok && g_copyCalls == 1 && g_parentCalls == 2;
    if (!ok) { std::printf("00463950_TEST FAIL\n"); return 1; }
    std::printf("00463950_TEST PASS\n");
    return 0;
}
