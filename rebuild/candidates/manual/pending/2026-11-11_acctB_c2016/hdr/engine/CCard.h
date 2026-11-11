#ifndef FABLE_ENGINE_CCARD_H
#define FABLE_ENGINE_CCARD_H
// CCard (chunk 2016, row 0x007D6870 CTCTavernGameCardPairs::ProcessInput): one playing card of the tavern card games. Ego_r.pdb layout
// (ghidra_out/struct_layouts_egor.tsv, 88 bytes, class key V: ??$_Random_shuffle@PAPAVCCard@@...), no vptr; the three positions are
// byte-packed C3DVectors (LocalPos +0x27), so natural layout reproduces every PDB offset. Retail evidence: 0x007D69E5 / 0x007D69EC read
// FaceDown +0x25 / Turning +0x24 (also the landed CTCTavernGameCardPairs::CardsMainGame 0x007D62D0), 0x007D6C85..0x007D6C96 LocalPos.X / .Y
// +0x27 / +0x2B with Width +0x4C / Height +0x50, 0x007D6ECC LocalPos.Z +0x2F.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CThingObject;
class CCard {
public:
    // Ego_r.pdb / FableWin.pdb ?Turn@CCard@@QAEXXZ, out of line in retail at 0x008F0620 (the bound spelling). Declaration only.
    void Turn();                                                         // 0x008F0620
    CThingObject* Thing;                                                 // +0x00
    long          Name;                                                  // +0x04
    long          Value;                                                 // +0x08
    float         InitialRotation;                                       // +0x0c
    float         FinalRotation;                                         // +0x10
    float         TurnSpeed;                                             // +0x14
    float         TurnFraction;                                          // +0x18
    float         MoveSpeed;                                             // +0x1c
    float         MoveFraction;                                          // +0x20
    bool          Turning;                                               // +0x24
    bool          FaceDown;                                              // +0x25
    bool          Moving;                                                // +0x26
    C3DVector     LocalPos;                                              // +0x27
    C3DVector     NewLocalPos;                                           // +0x33
    C3DVector     CurrentPos;                                            // +0x3f
    float         Width;                                                 // +0x4c
    float         Height;                                                // +0x50
    long          Index;                                                 // +0x54
};
FABLE_STATIC_ASSERT(offsetof(CCard, Turning) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CCard, LocalPos) == 0x27);
FABLE_STATIC_ASSERT(offsetof(CCard, Width) == 0x4c);
FABLE_STATIC_ASSERT(sizeof(CCard) == 0x58);
#endif
