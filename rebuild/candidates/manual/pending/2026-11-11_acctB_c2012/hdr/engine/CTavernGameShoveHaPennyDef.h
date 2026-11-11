#ifndef FABLE_ENGINE_CTAVERNGAMESHOVEHAPENNYDEF_H
#define FABLE_ENGINE_CTAVERNGAMESHOVEHAPENNYDEF_H
// New shared header (w22 agent2007, row 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput): the shove-ha'penny definition and its
// region sub-definition (the generated flattened copy sits in engine/_quarantine/). RTTI .?AVCTavernGameShoveHaPennyDef@@ :
// CTavernGameDef and .?AVCShoveHaPennyRegionDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb in order;
// CTavernGameDef is 0x1A4 in retail (engine/CTavernGameDef.h) and Regions is a 12-byte STLport vector, so Regions sits at +0x1A4
// (PDB +0x1A8; landed CalcScores 0x007CF860 reads +0x1A4 / +0x1A8) and everything after it 8 below its PDB offset (sizeof 0x200 as
// the generator's retail container layout computes): PositionCoinSpeedPC +0x1BC, PositionCoinMinimum +0x1C0, PositionCoinMaximum
// +0x1C4 (ProcessInput 0x007CFD28 / 0x007CFD3E / 0x007CFD68), PositionCoinDepth +0x1C8 (landed NewRound 0x007CF816),
// JoystickScalePCX / PCY / JoystickTranslateY +0x1DC / +0x1E0 / +0x1E4, AverageJoystickMagAbove +0x1EC, AverageDrift +0x1F0
// (ProcessInput). StatGetTypeName is inline (GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 builds the literal 0x01243D54 in place).
// CShoveHaPennyRegionDef: Start +0x28, Score +0x2C, sizeof 0x30 (CalcScores' stride). Needs FABLE_RETAIL_DEF_CLASS_BASE (+ the
// CTavernGameDef chain) and stlport/fable_containers.h.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CTavernGameDef.h"

class CShoveHaPennyRegionDef : public CSubDefClassBase {
public:
    float Start;                                    // +0x28
    float Score;                                    // +0x2c
};

class CTavernGameShoveHaPennyDef : public CTavernGameDef {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernGameShoveHaPennyDef", -1); }   // (inline)
    std::vector<CShoveHaPennyRegionDef> Regions;    // +0x1a4
    long  BoardDefIndex;                            // +0x1b0
    long  ArrowDefIndex;                            // +0x1b4
    float PositionCoinSpeedXbox;                    // +0x1b8
    float PositionCoinSpeedPC;                      // +0x1bc
    float PositionCoinMinimum;                      // +0x1c0
    float PositionCoinMaximum;                      // +0x1c4
    float PositionCoinDepth;                        // +0x1c8
    float ImpulseScale;                             // +0x1cc
    float TargetDistanceScale;                      // +0x1d0
    float JoystickScaleXboxX;                       // +0x1d4
    float JoystickScaleXboxY;                       // +0x1d8
    float JoystickScalePCX;                         // +0x1dc
    float JoystickScalePCY;                         // +0x1e0
    float JoystickTranslateY;                       // +0x1e4
    float JoystickMagBelow;                         // +0x1e8
    float AverageJoystickMagAbove;                  // +0x1ec
    float AverageDrift;                             // +0x1f0
    float MaxPower;                                 // +0x1f4
    float RestSpeed;                                // +0x1f8
    long  Rounds;                                   // +0x1fc
};
FABLE_STATIC_ASSERT(offsetof(CTavernGameShoveHaPennyDef, Regions) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CTavernGameShoveHaPennyDef, PositionCoinSpeedPC) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CTavernGameShoveHaPennyDef, JoystickScalePCX) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CTavernGameShoveHaPennyDef, AverageDrift) == 0x1f0);
FABLE_STATIC_ASSERT(sizeof(CTavernGameShoveHaPennyDef) == 0x200);
#endif
