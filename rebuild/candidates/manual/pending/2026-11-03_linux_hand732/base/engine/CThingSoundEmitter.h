#ifndef FABLE_ENGINE_CTHINGSOUNDEMITTER_H
#define FABLE_ENGINE_CTHINGSOUNDEMITTER_H
#include "engine/CThing.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// AttachedToThing, PAttachedThing, ContinuePlayingAfterAttachedThingDeath, Pos; complete tail through+b8, native allocation527340.
class CThingSoundEmitter:public CThing {
protected:
    unsigned char _pad_0x98[4]; // Native-only pointer, ctor8367c9; absent from all three donor PDBs, identity unresolved.
    bool AttachedToThing; // +9c, ctor8367d3 and writes836981/8369af
    unsigned char PAttachedThing[8]; // +9d CIntelligentPointer<const CThing>; ctor8367bb,8367df
    bool ContinuePlayingAfterAttachedThingDeath; // +a5, ctor8367e5
    unsigned char Pos[12]; // +a6 C3DVector; native8369c2 takes this exact unaligned address
    unsigned char _pad_0xb2[6];
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingSoundEmitter)==0xb8);
#endif
