// CTCPhysicalShieldSpell::ResumeForCutscene() -- after a cutscene cancelled the shield's loop, restart the looping sound for the current spell level (player-flagged when the owner is a player)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E520, 81 bytes.
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on the global table 0x013CA828 (CCharString by value into the `push ecx`
//   frame slot), CTCPhysicalShieldSpell::PlaySound @ 0x0079DE00 (landed as "CTCBulletTime::PlaySoundW", whose header already
//   suspects this class), CCharString::~CCharString @ 0x0099EAE0.
// Layout (PDB CTCPhysicalShieldSpell; no engine header): +0x04 the owner thing, PDef +0x0c (CDefPointer, one word), SpellLevel
//   +0x10 (1-based), LoopingSound +0x40, CutSceneCancelled +0x46. CSpecialAbilitiesPhysicalShieldDef::LoopingSound[4] at +0x54
//   (`[ecx+eax*4+50h]` = LoopingSound[SpellLevel - 1]). CThing +0x10 = the type byte (2 = player).
// Shape: the owner is a named local taken first (edi, read before the GetString call); the string is a named local; the
//   flag is cleared after the level/def/owner loads; the player test is `sete cl`
//   pushed as the bool; the sound handle is stored before the string's destructor runs.
// Leaf: ResumeForCutscene
struct CCharString {
    void* PStringData;                      // +0 the shared string block
    ~CCharString();                         // 0x0099EAE0
};

struct CDefStringTable {
    CCharString GetString(long id);         // 0x009D49B0
};

extern CDefStringTable g_DefStringTable;    // 0x013CA828

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char Type;                     // +0x10 (2 = player)
};

struct CSpecialAbilitiesPhysicalShieldDef {
    unsigned char _pad00[0x54];
    long          LoopingSound[4];          // +0x54 CDefString
};

struct CTCPhysicalShieldSpell {
    void*                                     __vftable;           // +0x00
    CThing*                                   ParentThing;         // +0x04
    unsigned char                             _pad08[0x4];         // +0x08
    const CSpecialAbilitiesPhysicalShieldDef* PDef;                // +0x0c CDefPointer
    long                                      SpellLevel;          // +0x10
    long                                      ShieldFadeOutTimer;  // +0x14
    long                                      CurShieldFadeOutTimer; // +0x18
    long                                      DurationTimer;       // +0x1c
    float                                     ShieldHealth;        // +0x20
    float                                     CurWobbleTimer;      // +0x24
    float                                     WobbleDelta;         // +0x28
    unsigned long                             ShieldColor;         // +0x2c CRGBColour
    unsigned char                             ShieldEffect[8];     // +0x30 CIntelligentPointer<CThing>
    unsigned char                             SwirlyEffect[8];     // +0x38 CIntelligentPointer<CThing>
    unsigned long                             LoopingSound;        // +0x40
    bool                                      Pausable;            // +0x44
    bool                                      Paused;              // +0x45
    bool                                      CutSceneCancelled;   // +0x46
    unsigned long PlaySound(const CCharString& sound, bool player);   // 0x0079DE00
    void ResumeForCutscene();
};

void CTCPhysicalShieldSpell::ResumeForCutscene()
{
    if (CutSceneCancelled) {
        CThing* owner = ParentThing;
        CutSceneCancelled = false;
        CCharString sound = g_DefStringTable.GetString(PDef->LoopingSound[SpellLevel - 1]);
        LoopingSound = PlaySound(sound, owner->Type == 2);
    }
}