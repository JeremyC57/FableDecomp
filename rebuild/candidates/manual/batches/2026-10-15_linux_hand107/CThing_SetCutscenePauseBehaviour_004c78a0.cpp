// CThing::SetCutscenePauseBehaviour(long behaviour) -- choose how the thing behaves during cutscenes: 0 default (neither flag), 1 paused during cutscenes, 2 explicitly not paused
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C78A0, 66 bytes. No callees.
// Shape: `sub eax,0 ; je ; dec eax ; je ; dec eax ; jne` is a three-case switch on the by-value enum (a long).
// PausedDuringCutscenes is bit 7 of +0x92, NotPausedDuringCutscenes bit 0 of +0x93 (retail offsets; PDB +0x10
// higher). Cases 0 and 1 share the store of +0x92 and the `and byte ptr [..],0FEh` clear.
struct CThing {
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x92 - 0x04];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; // +0x92 bit 0 (PDB +0xa2)
    unsigned char Drawable : 1;                      // bit 1
    unsigned char Dying : 1;                         // bit 2
    unsigned char ForceFrameUpdate : 1;              // bit 3
    unsigned char LevelPersistent : 1;               // bit 4
    unsigned char GameStatePersisted : 1;            // bit 5
    unsigned char PersistOnUnloadWhenInLimbo : 1;    // bit 6
    unsigned char PausedDuringCutscenes : 1;         // bit 7
    unsigned char NotPausedDuringCutscenes : 1;      // +0x93 bit 0
    void SetCutscenePauseBehaviour(long behaviour);
};

void CThing::SetCutscenePauseBehaviour(long behaviour)
{
    switch (behaviour) {
    case 0:
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = false;
        break;
    case 1:
        PausedDuringCutscenes = true;
        NotPausedDuringCutscenes = false;
        break;
    case 2:
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = true;
        break;
    }
}
