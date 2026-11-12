// CThing::SetCutscenePauseBehaviour(ECutsceneBehaviour behaviour) -- choose how the thing behaves during cutscenes: 0 default (neither flag), 1 paused during cutscenes, 2 explicitly not paused
// Leaf: ?SetCutscenePauseBehaviour@CThing@@QAEXW4ECutsceneBehaviour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C78A0, 66 bytes. No callees.
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?SetCutscenePauseBehaviour@CThing@@QAEXW4ECutsceneBehaviour@@@Z; the enum is
//   engine/ECutsceneBehaviour.h (DEFAULT 0, PAUSED 1, NOT_PAUSED 2). Was spelled with a long parameter.
// Shape: `sub eax,0 ; je ; dec eax ; je ; dec eax ; jne` is a three-case switch on the by-value enum.
// PausedDuringCutscenes is bit 7 of +0x92, NotPausedDuringCutscenes bit 0 of +0x93 (retail offsets; PDB +0x10
// higher). Cases 0 and 1 share the store of +0x92 and the `and byte ptr [..],0FEh` clear.
#include "engine/ECutsceneBehaviour.h"
class CThing { public:
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
    void SetCutscenePauseBehaviour(ECutsceneBehaviour behaviour);
};

void CThing::SetCutscenePauseBehaviour(ECutsceneBehaviour behaviour)
{
    switch (behaviour) {
    case CUTSCENE_BEHAVIOUR_DEFAULT:
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = false;
        break;
    case CUTSCENE_BEHAVIOUR_PAUSED:
        PausedDuringCutscenes = true;
        NotPausedDuringCutscenes = false;
        break;
    case CUTSCENE_BEHAVIOUR_NOT_PAUSED:
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = true;
        break;
    }
}