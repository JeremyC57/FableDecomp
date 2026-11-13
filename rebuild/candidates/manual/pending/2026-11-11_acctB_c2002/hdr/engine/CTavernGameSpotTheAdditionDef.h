#ifndef FABLE_ENGINE_CTAVERNGAMESPOTTHEADDITIONDEF_H
#define FABLE_ENGINE_CTAVERNGAMESPOTTHEADDITIONDEF_H
// New shared header (w22 agent2002, row 0x007D8570 CTCTavernGameSpotTheAddition::ProcessInput): the spot-the-addition tavern-game
// definition. RTTI .?AVCTavernGameSpotTheAdditionDef@@ : CSubDefClassBase. Members: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) in
// order; Knicknacks / ItemsPerRound are STLport 4.6 vectors (12 bytes in retail, 16 in Ego_r), so every member after the first one
// sits 4 (then 8) bytes below its PDB offset. The same model as the landed FrameUpdate 0x007D8E20's local class (TimeToMoveCover +0x6c,
// KnickKnackRadius +0x5c, CoverWidth +0x30 ...); ProcessInput 0x007D8570 reads ItemsPerRound +0x44/+0x48, ImpulseScale +0x60,
// HighlightWidth +0x84, HighlightColour +0x88, HighlightColourError +0x8c. StatGetTypeName is inline (GetSubDef<CTavernGameSpotTheAdditionDef>
// 0x007D9A80 builds the literal in place). Needs the retail def model (FABLE_RETAIL_DEF_CLASS_BASE) and stlport/fable_containers.h.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CRGBColour.h"

class CTavernGameSpotTheAdditionDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernGameSpotTheAdditionDef", -1); }   // (inline)
    long  Pointer;                      // +0x28
    long  Cover;                        // +0x2c
    float CoverWidth;                   // +0x30
    float CoverLength;                  // +0x34
    std::vector<long> Knicknacks;       // +0x38
    std::vector<long> ItemsPerRound;    // +0x44
    float JoystickScale;                // +0x50
    float TimeToThink;                  // +0x54
    float MaxCursorHeight;              // +0x58
    float KnickKnackRadius;             // +0x5c
    float ImpulseScale;                 // +0x60
    long  PointerModel;                 // +0x64
    float PointerHeightOffset;          // +0x68
    float TimeToMoveCover;              // +0x6c
    float TimeToPauseCover;             // +0x70
    float MaxCoverHeight;               // +0x74
    float MinimumKnickKnackGap;         // +0x78
    float LengthMovementScale;          // +0x7c
    float HeightMovementScale;          // +0x80
    float HighlightWidth;               // +0x84
    CRGBColour HighlightColour;         // +0x88
    CRGBColour HighlightColourError;    // +0x8c
};
FABLE_STATIC_ASSERT(offsetof(CTavernGameSpotTheAdditionDef, ItemsPerRound) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTavernGameSpotTheAdditionDef, ImpulseScale) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTavernGameSpotTheAdditionDef, HighlightColourError) == 0x8c);
#endif
