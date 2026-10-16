// CTCWound::CWound::GetScarStrength() const -- scar strength: rises 0..1 over the fade, then relaxes towards PermanentScarStrength
// Retail 0x00757340, 0x79 bytes, no callees. PermanentScarStrength (+0x24) once Duration reaches TotalDuration or when there
// is no fade; Duration/Fade while Duration <= Fade; afterwards t = (Duration-Fade)/(Total-Fade) and the result is
// (1 - t) * (1 - Permanent) + Permanent. Duration is unsigned (fild + 2^32 fix-up, pool 0x0122DCB4); 1.0f pool 0x0122DED8.
// Layout (PDB CTCWound::CWound, 0x44 bytes): Desc = CWoundMorphs::CEntry (40 bytes: Location +0x00, replacing/wound/scar
// texture and bump indices +0x04..+0x18, FadeToScarDuration +0x1C, TotalDuration +0x20, PermanentScarStrength +0x24),
// Duration (ulong) +0x28, decal entry IDs +0x2C..+0x38, MorphIndex +0x3C, Dead +0x40, HadUpdate +0x41.
struct CWoundMorphsEntry
{
    long  Location;                    // +0x00 EBodyLocation
    long  ReplacingTextureIndex;       // +0x04
    long  ReplacingBumpMapIndex;       // +0x08
    long  WoundTextureIndex;           // +0x0C
    long  WoundBumpMapIndex;           // +0x10
    long  ScarTextureIndex;            // +0x14
    long  ScarBumpMapIndex;            // +0x18
    long  FadeToScarDuration;          // +0x1C
    long  TotalDuration;               // +0x20
    float PermanentScarStrength;       // +0x24
};

class CWound
{
public:
    CWoundMorphsEntry Desc;            // +0x00
    unsigned long     Duration;        // +0x28
    long              WoundTextureDecalEntryID;  // +0x2C
    long              WoundBumpDecalEntryID;     // +0x30
    long              ScarTextureDecalEntryID;   // +0x34
    long              ScarBumpDecalEntryID;      // +0x38
    long              MorphIndex;                // +0x3C
    bool              Dead;                      // +0x40
    bool              HadUpdate;                 // +0x41

    float GetScarStrength() const;
};

float CWound::GetScarStrength() const
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