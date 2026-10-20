// CWound::Update(CTCTextureDecal&) -- advance one wound a frame: once its total duration is reached mark it dead (unless it leaves a permanent scar) and stop after the first update; refresh the wound and scar texture/bump decals with the current strengths
// Leaf: ?Update@CWound@CTCWound@@
// Retail: Fable.exe @ 0x00757570, 206 bytes, `ret 4`. PDB ?Update@CWound@CTCWound@@QAEXAAVCTCTextureDecal@@@Z (CTCWound::CWound)
// Callees: CTCWound::CWound::GetWoundStrength() const 0x007572D0, GetScarStrength() const 0x00757340, UpdateDecal(
// CTCTextureDecal&, long&, long, long, float) 0x00757440. Float pool 0x0129BA3C = 0.0001f.
// CWound (68 bytes, PDB = retail): Desc (CEntry: Location +0, ReplacingTextureIndex +4, ReplacingBumpMapIndex +8,
// WoundTextureIndex +0xC, WoundBumpMapIndex +0x10, ScarTextureIndex +0x14, ScarBumpMapIndex +0x18, FadeToScarDuration
// +0x1C, TotalDuration +0x20, PermanentScarStrength +0x24), Duration +0x28, the four decal entry ids +0x2C..+0x38,
// MorphIndex +0x3C, Dead +0x40, HadUpdate +0x41.
// Shape: the two strength getters' bodies sit above (auto_inline off) so the index stays in edi across them; the unsigned
// Duration is pre-incremented against TotalDuration (`jb`); each decal call evaluates its strength
// argument first (right-to-left).
struct CTCTextureDecal;
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

struct CTCWound {
    struct CEntry {                                          // 40 bytes
        EBodyLocation Location;                              // +0x00
        long          ReplacingTextureIndex;                 // +0x04
        long          ReplacingBumpMapIndex;                 // +0x08
        long          WoundTextureIndex;                     // +0x0C
        long          WoundBumpMapIndex;                     // +0x10
        long          ScarTextureIndex;                      // +0x14
        long          ScarBumpMapIndex;                      // +0x18
        long          FadeToScarDuration;                    // +0x1C
        long          TotalDuration;                         // +0x20
        float         PermanentScarStrength;                 // +0x24
    };
    struct CWound {
        CEntry        Desc;                                  // +0x00
        unsigned long Duration;                              // +0x28
        long          WoundTextureDecalEntryID;              // +0x2C
        long          WoundBumpDecalEntryID;                 // +0x30
        long          ScarTextureDecalEntryID;               // +0x34
        long          ScarBumpDecalEntryID;                  // +0x38
        long          MorphIndex;                            // +0x3C
        bool          Dead;                                  // +0x40
        bool          HadUpdate;                             // +0x41
        float GetWoundStrength() const;                      // 0x007572D0
        float GetScarStrength() const;                       // 0x00757340
        void UpdateDecal(CTCTextureDecal& decals, long& entryID, long decal, long textureIndex, float strength);   // 0x00757440
        void Update(CTCTextureDecal& decals);
    };
};

// The landed bodies of 0x007572D0 / 0x00757340 (same TU, compiled before Update), defined above under auto_inline(off):
// VC7.1 then knows they do not write the wound and keeps each texture index in edi across the strength call.
#pragma auto_inline(off)
float CTCWound::CWound::GetWoundStrength() const
{
    if (Duration < (unsigned long)Desc.TotalDuration)
    {
        if (Desc.FadeToScarDuration > 0)
        {
            if (Duration < (unsigned long)Desc.FadeToScarDuration)
                return 1.0f - (float)Duration / Desc.FadeToScarDuration;
        }
        else
            return 1.0f - (float)Duration / Desc.TotalDuration;
    }
    return 0.0f;
}

float CTCWound::CWound::GetScarStrength() const
{
    if (Duration < (unsigned long)Desc.TotalDuration && Desc.FadeToScarDuration > 0)
    {
        if (Duration > (unsigned long)Desc.FadeToScarDuration)
        {
            float sinceFade = (float)(Duration - Desc.FadeToScarDuration);
            float scarTime = (float)(Desc.TotalDuration - Desc.FadeToScarDuration);
            float t = sinceFade / scarTime;
            return (1.0f - t) * (1.0f - Desc.PermanentScarStrength) + Desc.PermanentScarStrength;
        }
        return (float)Duration / Desc.FadeToScarDuration;
    }
    return Desc.PermanentScarStrength;
}
#pragma auto_inline(on)

void CTCWound::CWound::Update(CTCTextureDecal& decals)
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
