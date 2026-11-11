// CUnprovenSoundWorld_00c1c460 -- the libpk object whose update 0x00C1C460 re-positions every playing positional source in listener
// space (new shared header, chunk 1023). Unproven owner: no PDB covers libpk and no vtable / RTTI is read; the landed callers bind
// the address three ways (CWorld::Refresh in 0x00C1C620 SetAsPaused, which reads the paused flag at +0x18,
// CTCInventoryMagic_00c1c6a0::Step, CTarget_00c10a90::Update), all descriptive. Members: the sound-event manager at +4
// (0x00C1C48A) and the update-enabled flag at +0x19 (0x00C1C47E); +0x18 is the paused flag of the SetAsPaused row.
#ifndef FABLE_LIBPK_CUNPROVENSOUNDWORLD_00C1C460_H
#define FABLE_LIBPK_CUNPROVENSOUNDWORLD_00C1C460_H
#define FABLE_RETAIL_LHAUDIO_LISTENER_SPACE
#include "libpk/LHAudioEventPosition.h"
#include "libpk/PKCProtectionInstance.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"

class CUnprovenSoundWorld_00c1c460 {
public:
    long Unmodelled00;
    CSoundEventManager* PManager;                                        // +0x04
    unsigned char Unmodelled08[0x10];
    bool Paused;                                                         // +0x18
    bool SourcePositionsEnabled;                                         // +0x19
    void UpdateSourcePositions();                                        // 0x00C1C460
};

extern PKCCriticalSection g_soundLock;                                   // 0x0143BAFC
C3DVector __fastcall SwapYZ(const C3DVector& pos);                       // 0x00C1BFB0 (landed)
void FABLE_FASTCALL InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& m);   // 0x00C1D1C0 (landed)
extern "C" void __stdcall CODA_SetSourcePosition(long source, const C3DVector* position);   // 0x00C33FD0
void __fastcall GlobalMethodTailThunk_00c33690();                        // 0x00C33690 (landed)
#endif
