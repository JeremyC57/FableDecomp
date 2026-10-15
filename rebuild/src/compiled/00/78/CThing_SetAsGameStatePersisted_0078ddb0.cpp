// CThing::SetAsGameStatePersisted(bool persisted) -- write the game-state-persisted flag (bit 5 of the flag byte at +0x15c)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078DDB0, 28 bytes. No callees.
// Identity: the catalog label is propagated from CThing (whose GameStatePersisted bit is +0x92 bit 5); this body
// writes bit 5 of +0x15c, i.e. the same-named bitfield of a larger derived class. Modelled locally.
// Shape: a one-bit `unsigned char` bitfield store from a bool argument is the classic
// `shl al,5 ; xor al,dl ; and al,20h ; xor dl,al` merge.
struct CThing {
    unsigned char _pad000[0x15c];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; // +0x15c bit 0
    unsigned char Drawable : 1;                      // bit 1
    unsigned char Dying : 1;                         // bit 2
    unsigned char ForceFrameUpdate : 1;              // bit 3
    unsigned char LevelPersistent : 1;               // bit 4
    unsigned char GameStatePersisted : 1;            // bit 5
    unsigned char PersistOnUnloadWhenInLimbo : 1;    // bit 6
    unsigned char PausedDuringCutscenes : 1;         // bit 7
    void SetAsGameStatePersisted(bool persisted);
};

void CThing::SetAsGameStatePersisted(bool persisted)
{
    GameStatePersisted = persisted;
}