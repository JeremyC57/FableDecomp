// CTCBerserk::SetLevel() -- load the berserk tuning for the current spell level from the def: the duration in frames (rounded up) and the damage, scale and rumble parameters
// Grade: DIFFER(ours v retail) -- lane gap: retail rounds with a bare fistp inside a frame-pointer body (inline-asm float-to-long helper in the original ceil); plain C++ gives __ftol2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BF00, length 187 bytes (+ int3 padding). No calls.
// Globals: the frames-per-second counter 0x01375550.
// Layout: Ego_r.pdb CTCBerserk with the retail 12-byte DependentThings vector: Timer +0x0C, MaxIncreaseInStrength +0x18,
// BerserkDamageToMe +0x24, BerserkDamageToEnemy +0x28, BerserkScale +0x2C, RumbleLevel +0x30, RumbleSmoothness +0x34,
// RumbleAttack +0x38, RumbleDecay +0x3C, SpellLevel +0x44, PDef +0x68 (PDB +0x6C; engine/CTCBerserk.h has the PDB offsets,
// so the class is modelled locally); the def's per-level vectors are read through their first pointer.
// Shape: retail keeps a frame pointer and rounds with a bare `fistp` (the inline-asm float-to-long helper of the original
// ceil: `n = round(t + 0.5f); if (t == n - 1.0f) --n;`); written here in plain C++.
struct CSpecialAbilitiesBerserkDef {
    unsigned char _pad00[0x7c];
    float* Duration;                            // +0x7c vector<float> first
    unsigned char _pad80[0x88 - 0x80];
    float* DamageToEnemy;                       // +0x88
    unsigned char _pad8c[0x98 - 0x8c];
    float* DamageToMe;                          // +0x98
    unsigned char _pad9c[0xa4 - 0x9c];
    float* MaxIncreaseInStrength;               // +0xa4
    unsigned char _pada8[0xb0 - 0xa8];
    float* Scale;                               // +0xb0
    unsigned char _padb4[0xf0 - 0xb4];
    float* RumbleLevel;                         // +0xf0
    unsigned char _padf4[0xfc - 0xf4];
    float* RumbleSmoothness;                    // +0xfc
    unsigned char _pad100[0x108 - 0x100];
    float* RumbleAttack;                        // +0x108
    unsigned char _pad10c[0x114 - 0x10c];
    float* RumbleDecay;                         // +0x114
};
extern long g_FramesPerSecond;                  // 0x01375550
class CTCBerserk {
public:
    virtual void v00();
    unsigned char _base04[8];                   // CTCBase
    long  Timer;                                // +0x0c
    long  DarkenEnvTimer;                       // +0x10
    float StrengthPercentage;                   // +0x14
    float MaxIncreaseInStrength;                // +0x18
    float ReleasedAnimationTimer;               // +0x1c
    long  AnimTimer;                            // +0x20
    float BerserkDamageToMe;                    // +0x24
    float BerserkDamageToEnemy;                 // +0x28
    float BerserkScale;                         // +0x2c
    float RumbleLevel;                          // +0x30
    float RumbleSmoothness;                     // +0x34
    float RumbleAttack;                         // +0x38
    float RumbleDecay;                          // +0x3c
    unsigned long BerserkingLoopingSound;       // +0x40
    long  SpellLevel;                           // +0x44
    unsigned char _pad48[0x68 - 0x48];
    const CSpecialAbilitiesBerserkDef* PDef;    // +0x68
    void SetLevel();
};

void CTCBerserk::SetLevel()
{
    float frames = g_FramesPerSecond * PDef->Duration[SpellLevel];
    float half = 0.5f;
    float one = 1.0f;
    long timer = (long)(frames + half);
    if (frames == timer - one)
        --timer;
    Timer = timer;
    const CSpecialAbilitiesBerserkDef* def = PDef;
    long level = SpellLevel;
    BerserkDamageToMe = def->DamageToMe[level];
    BerserkDamageToEnemy = def->DamageToEnemy[level];
    MaxIncreaseInStrength = def->MaxIncreaseInStrength[level];
    BerserkScale = def->Scale[level];
    RumbleLevel = def->RumbleLevel[level];
    RumbleSmoothness = def->RumbleSmoothness[level];
    RumbleAttack = def->RumbleAttack[level];
    RumbleDecay = def->RumbleDecay[level];
}
