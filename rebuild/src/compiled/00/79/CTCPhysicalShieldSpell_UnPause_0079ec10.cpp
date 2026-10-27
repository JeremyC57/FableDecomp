// CTCPhysicalShieldSpell::UnPause() -- resume a paused physical shield: restart the looping sound for the spell level (player-flagged for a player owner) and replay the shield effects on the owner
// Leaf: ?UnPause@CTCPhysicalShieldSpell@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079EC10, 96 bytes, plain `ret`.
// Identity: Ego_r.pdb `?UnPause@CTCPhysicalShieldSpell@@QAEXXZ`. The donor lays the TU out ... Pause, PlayShieldEffects,
//   CancelForCutscene, FinalCleanup, FrameUpdate, UnPause, ~dtor, OnKill, OnRemove; retail has the landed FrameUpdate
//   0x0079EB30 right before this body and OnKill / OnRemove right after (0x0079EC70 / 0x0079EC80). The body is the mirror
//   of the landed Pause 0x0079E3A0 (`Pausable && Paused` guard, clears Paused, restarts what Pause stopped). Callers
//   0x0072C054 / 0x0072C1E8 (next to the CTCBerserk Pause/UnPause calls). Catalog row was the placeholder
//   `_gapscan::sub_0079ec10`.
// Callees: CDefStringTable::GetString(long) const @ 0x009D49B0 on the global table 0x013CA828 (Ego_r.pdb
//   `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z`), CTCPhysicalShieldSpell::PlaySoundW(const CCharString&, bool)
//   @ 0x0079DE00 (Ego_r.pdb `?PlaySoundW@CTCPhysicalShieldSpell@@IAEKABVCCharString@@_N@Z`: the windows.h PlaySound macro
//   renamed it; protected), ~CCharString @ 0x0099EAE0, CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase*) @
//   0x0079E420 (Ego_r.pdb `?PlayShieldEffects@CTCPhysicalShieldSpell@@IAEXPAVCThingCreatureBase@@@Z`, protected).
// Layout: as the landed ResumeForCutscene 0x0079E520: owner +0x04, PDef +0x0c, SpellLevel +0x10, LoopingSound +0x40,
//   Pausable +0x44, Paused +0x45; CSpecialAbilitiesPhysicalShieldDef::LoopingSound[] at +0x50; CThing +0x10 type byte (2 = player).
// Shape: the ResumeForCutscene shape (owner read into edi first, flag cleared after the loads, `sete cl` pushed as the bool,
//   the handle stored before the string's destructor), then the owner passed to PlayShieldEffects: the string is a named
//   local in its own block, so its destructor runs before the PlayShieldEffects call.
class CCharString { public:
    void* PStringData;                      // +0 the shared string block
    ~CCharString();                         // 0x0099EAE0
};

class CDefStringTable {
public:
    CCharString GetString(long index) const;   // 0x009D49B0
};

extern CDefStringTable g_DefStringTable;    // 0x013CA828

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char Type;                     // +0x10 (2 = player)
};
class CThingCreatureBase : public CThing { public:
};

class CSpecialAbilitiesPhysicalShieldDef { public:
    unsigned char _pad00[0x50];
    long          LoopingSound[4];          // +0x50 CDefString
};

class CTCPhysicalShieldSpell { public:
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
    void UnPause();
protected:
    unsigned long PlaySoundW(const CCharString& sound, bool player);   // 0x0079DE00
    void PlayShieldEffects(CThingCreatureBase* creature);           // 0x0079E420
};

void CTCPhysicalShieldSpell::UnPause()
{
    if (Pausable && Paused) {
        CThingCreatureBase* owner = static_cast<CThingCreatureBase*>(ParentThing);
        Paused = false;
        {
            CCharString sound = g_DefStringTable.GetString(PDef->LoopingSound[SpellLevel]);
            LoopingSound = PlaySoundW(sound, owner->Type == 2);
        }
        PlayShieldEffects(owner);
    }
}