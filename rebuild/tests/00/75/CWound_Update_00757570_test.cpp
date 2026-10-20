// Behaviour test for CTCWound::CWound::Update 0x00757570: duration/dead handling and the four decal refreshes.
#include <cstdio>

struct CTCTextureDecal { int dummy; };
struct CEntry {
    long Location, ReplacingTextureIndex, ReplacingBumpMapIndex, WoundTextureIndex, WoundBumpMapIndex,
         ScarTextureIndex, ScarBumpMapIndex, FadeToScarDuration, TotalDuration;
    float PermanentScarStrength;
};
static int g_calls = 0; static long g_lastDecal = 0, g_lastTexture = 0; static float g_lastStrength = 0; static long* g_lastEntry = 0;
struct CWound {
    CEntry Desc; unsigned long Duration;
    long WoundTextureDecalEntryID, WoundBumpDecalEntryID, ScarTextureDecalEntryID, ScarBumpDecalEntryID, MorphIndex;
    bool Dead, HadUpdate;
    float GetWoundStrength() const { return 0.75f; }
    float GetScarStrength() const { return 0.25f; }
    void UpdateDecal(CTCTextureDecal&, long& entryID, long decal, long textureIndex, float strength)
    { ++g_calls; g_lastEntry = &entryID; g_lastDecal = decal; g_lastTexture = textureIndex; g_lastStrength = strength; }
    void Update(CTCTextureDecal& decals);
};

void CWound::Update(CTCTextureDecal& decals)
{
    if (++Duration >= (unsigned long)Desc.TotalDuration)
    {
        if (!(Desc.PermanentScarStrength > 0.0001f))
            Dead = true;
        if (HadUpdate)
            return;
    }
    if (Desc.WoundTextureIndex)
        UpdateDecal(decals, WoundTextureDecalEntryID, Desc.ReplacingTextureIndex, Desc.WoundTextureIndex, GetWoundStrength());
    if (Desc.WoundBumpMapIndex)
        UpdateDecal(decals, WoundBumpDecalEntryID, Desc.ReplacingBumpMapIndex, Desc.WoundBumpMapIndex, GetWoundStrength());
    if (Desc.ScarTextureIndex)
        UpdateDecal(decals, ScarTextureDecalEntryID, Desc.ReplacingTextureIndex, Desc.ScarTextureIndex, GetScarStrength());
    if (Desc.ScarBumpMapIndex)
        UpdateDecal(decals, ScarBumpDecalEntryID, Desc.ReplacingBumpMapIndex, Desc.ScarBumpMapIndex, GetScarStrength());
    HadUpdate = true;
}

int main()
{
    CTCTextureDecal decals; CWound w;
    w.Desc.Location = 0; w.Desc.ReplacingTextureIndex = 4; w.Desc.ReplacingBumpMapIndex = 8; w.Desc.WoundTextureIndex = 12;
    w.Desc.WoundBumpMapIndex = 0; w.Desc.ScarTextureIndex = 20; w.Desc.ScarBumpMapIndex = 24; w.Desc.FadeToScarDuration = 2;
    w.Desc.TotalDuration = 3; w.Desc.PermanentScarStrength = 0.0f; w.Duration = 0; w.Dead = false; w.HadUpdate = false;
    bool ok = true;
    w.Update(decals);                                  // Duration 1: three decals (wound bump index is 0)
    if (g_calls != 3 || g_lastEntry != &w.ScarBumpDecalEntryID || g_lastDecal != 8 || g_lastTexture != 24 || g_lastStrength != 0.25f || !w.HadUpdate || w.Dead) ok = false;
    w.Update(decals);                                  // Duration 2
    if (g_calls != 6) ok = false;
    w.Update(decals);                                  // Duration 3 >= Total: dead, already updated -> stop
    if (g_calls != 6 || !w.Dead || w.Duration != 3) ok = false;
    CWound v = w; v.Duration = 2; v.HadUpdate = false; v.Dead = false; v.Desc.PermanentScarStrength = 0.5f;
    v.Update(decals);                                  // reaches Total, permanent scar: not dead, first update runs
    if (v.Dead || g_calls != 9 || !v.HadUpdate) ok = false;
    if (!ok) { std::printf("00757570_TEST FAIL\n"); return 1; }
    std::printf("00757570_TEST PASS\n");
    return 0;
}