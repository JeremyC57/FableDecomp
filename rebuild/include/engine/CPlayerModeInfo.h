#ifndef FABLE_ENGINE_CPLAYERMODEINFO_H
#define FABLE_ENGINE_CPLAYERMODEINFO_H
#include "rebuild_abi.h"
#include "engine/EPlayerMode.h"
class CPlayer;
class CProcessedInput;
// Both original PDBs: class32, two public16-byte general member-function pointers.
// It precedes CPlayer's complete definition, retaining the original unresolved inheritance representation.
class CPlayerModeInfo {
public:
    void (CPlayer::*InitFunc)(EPlayerMode,CProcessedInput*);
    void (CPlayer::*UninitFunc)(EPlayerMode,CProcessedInput*);
};
FABLE_STATIC_ASSERT(sizeof(CPlayerModeInfo)==32);
#endif
