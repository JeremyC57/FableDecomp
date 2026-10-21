// CThing::IsLevelPersistent() const -- whether the thing survives its level: game-state persisted, or serialised by name, else the LevelPersistent bit
// Retry 10b: GameStatePersisted bound to a `const bool&` (a bit-field reference binds a bool TEMPORARY whose address is taken): retail's dead frame byte `mov [esp+7],al` and the separate `mov al,1` exit (`return true`); a plain bool local folds into a test.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C82A0, 104 bytes (+8 int3). __fastcall, returns bool in al.
// Callees: CCharString::operator!=(const char*) const 0x0099E960 (x2: "" 0x0122D70E, "NULL" 0x012393E4).
// Shape: a thing holding component 199 (bitset word 6 bit 7: `mov al,[esi+38h] ; test al,al ; jns`) is never level
//        persistent; GameStatePersisted (bit 5 of +0x92) bound to a const bool& first (`mov [esp+7],al`) returns true;
//        without KillOnLevelUnload (bit 2 of +0x93) a real SerialisationSectionName (not "" / "NULL") returns true;
//        otherwise the LevelPersistent bit (bit 4 of +0x92) as an unsigned char bitfield (`shr al,4 ; and al,1`).
// Layout: retail CThing = Ego_r.pdb - 0x10 here: UsingComponents +0x20, SerialisationSectionName +0x58, flag bytes +0x91..
//         +0x93 with the PDB bit order.
struct CCharString {
    void* PStringData;
    bool operator!=(const char* text) const;     // 0x0099E960
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];            // +0x20 bitset<274>
    unsigned char _pad44[0x58 - 0x44];
    CCharString SerialisationSectionName;        // +0x58 (PDB +0x68)
    unsigned char _pad5c[0x91 - 0x5c];
    unsigned char Dead : 1;                      // +0x91
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;  // +0x93
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char _bits93 : 4;
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    bool IsGameStatePersisted() const { return GameStatePersisted; }
    bool IsLevelPersistent() const;
};
bool CThing::IsLevelPersistent() const
{
    if (HasTC(199))
        return false;
    const bool& persistent = GameStatePersisted;
    if (persistent)
        return true;
    if (!KillOnLevelUnload && SerialisationSectionName != "" && SerialisationSectionName != "NULL")
        return true;
    return LevelPersistent;
}