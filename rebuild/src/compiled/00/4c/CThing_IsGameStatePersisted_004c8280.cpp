// CThing::IsGameStatePersisted() const -- whether this thing's state is saved with the game (never for co-op spirits)
// Leaf: IsGameStatePersisted@CThing
// Retail: Fable.exe @ 0x004C8280, 22 bytes; Ego_r.pdb ?IsGameStatePersisted@CThing@@QBE_NXZ.
// The first test is HasTC(0xc7): bit 199 of UsingComponents is bit 7 of byte +0x38 (`test al,al; jns`); 0xc7 is the
// interface type CTCCoopSpirit's slot-21 getter returns.
// Layout: retail CThing: UsingComponents +0x20 as in Ego_r.pdb; the flag bytes sit 0x10 lower (GameStatePersisted +0x92 bit 5,
// PDB +0xa2 bit 5; Dead +0x91 bit 0 as in the hero rows).
enum { TC_COOP_SPIRIT = 0xc7 };                          // CTCCoopSpirit interface type (vtable slot 21 returns 0xc7)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    unsigned char _pad44[0x92 - 0x44];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;     // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;                // +0x92 bit 5
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    bool IsGameStatePersisted() const;
};
bool CThing::IsGameStatePersisted() const
{
    if (HasTC(TC_COOP_SPIRIT))
        return false;
    return GameStatePersisted;
}