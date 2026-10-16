// CTCWound::CWound::GetWoundStrength() const -- fresh-wound strength: 1 fading to 0 over the fade-to-scar time (or the whole life)
// Retail 0x007572D0, 0x66 bytes, no callees. 0 once Duration reaches TotalDuration (unsigned compare); with a positive
// FadeToScarDuration the strength is 1 - Duration/Fade while Duration < Fade (0 after), otherwise 1 - Duration/Total.
// Duration is unsigned (fild + 2^32 pool fix-up 0x0122DCB4), the divisors are longs (`fidiv`); 1.0f pool 0x0122DED8,
// 0.0f pool 0x0122DEDC.
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

    float GetWoundStrength() const;
};

float CWound::GetWoundStrength() const
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