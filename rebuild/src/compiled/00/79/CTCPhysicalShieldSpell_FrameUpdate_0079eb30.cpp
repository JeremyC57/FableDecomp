// CTCPhysicalShieldSpell::FrameUpdate() -- per frame: count the duration down and redraw the shield unless paused; once the duration has run out, stop it being pausable and, when the fade-out timer expires, play the end sound and END effect and remove the shield component (0x93) from its owner
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079EB30, 209 bytes (plain `ret`; PDB ?FrameUpdate@CTCPhysicalShieldSpell@@UAEXXZ).
// Callees: CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase*) @ 0x0079E420, CDefStringTable::GetString(long) @ 0x009D49B0
//   on g_DefStringTable @ 0x013CA828, PlaySound(const CCharString&, bool) @ 0x0079DE00 (PDB PlaySoundW), CCharString(const char*,
//   long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, PlayEffect @ 0x0079DEB0, CThing::RemoveTC(long) @ 0x004C9840.
// Layout: Ego_r.pdb CTCPhysicalShieldSpell (PDef +0x0c, CurShieldFadeOutTimer +0x18, DurationTimer +0x1c, Pausable/Paused
//   +0x44/+0x45); the def's EndSound CDefString at +0x60 (PDB +0x64, def bodies sit 4 bytes low); CThing +0x10 type byte (2 = player).
// Shape: the tail is the landed CancelMove 0x0079E300 verbatim (sound string in its own block, the "" type string declared
//   before the "END" bone name so it sits one word higher, both destroyed before RemoveTC); the owner is read once into edi at entry.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
enum ETCInterfaceType { };
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char Type;                         // +0x10 (2 = player)
    void RemoveTC(ETCInterfaceType type);                   // 0x004C9840
};
class CThingCreatureBase : public CThing { public: };
class CSpecialAbilitiesPhysicalShieldDef { public:
    unsigned char _pad00[0x60];
    long EndSound;                              // +0x60 CDefString id (PDB +0x64)
};
class CTCPhysicalShieldSpell { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    const CSpecialAbilitiesPhysicalShieldDef* PDef;   // +0x0c CDefPointer
    long    SpellLevel;                         // +0x10
    long    ShieldFadeOutTimer;                 // +0x14
    long    CurShieldFadeOutTimer;              // +0x18
    long    DurationTimer;                      // +0x1c
    unsigned char _pad20[0x44 - 0x20];
    bool    Pausable;                           // +0x44
    bool    Paused;                             // +0x45
    unsigned long PlaySound(const CCharString& sound, bool player);   // 0x0079DE00
    void PlayEffect(long effect, const CCharString& boneName, const CCharString& type, CThing* target, long flags);   // 0x0079DEB0
    void PlayShieldEffects(CThingCreatureBase* creature);   // 0x0079E420
    void FrameUpdate();
};

void CTCPhysicalShieldSpell::FrameUpdate()
{
    CThing* owner = ParentThing;
    if (!Paused) {
        if (DurationTimer > 0)
            --DurationTimer;
        PlayShieldEffects((CThingCreatureBase*)owner);
    }
    if (DurationTimer == 0) {
        Pausable = false;
        if (--CurShieldFadeOutTimer <= 0) {
            {
                CCharString sound = g_DefStringTable.GetString(PDef->EndSound);
                PlaySound(sound, owner->Type == 2);
            }
            {
                CCharString type("", -1);
                CCharString boneName("END", -1);
                PlayEffect(0, boneName, type, owner, 8);
            }
            owner->RemoveTC((ETCInterfaceType)(0x93));
        }
    }
}