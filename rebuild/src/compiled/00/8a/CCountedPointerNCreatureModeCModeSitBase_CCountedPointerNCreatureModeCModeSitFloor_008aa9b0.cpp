// CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(const CCountedPointer<NCreatureMode::CModeSitFloor>&) -- the converting copy
//   constructor from the floor-sitting mode's counted pointer: copies Data and Info from the source and, when there is a count block,
//   takes a reference on it; returns this (engine/CCountedPointer.h member template `template <class U> CCountedPointer(const CCountedPointer<U>&)`).
// Leaf: ??$?0VCModeSitFloor@NCreatureMode@@@?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@ABV?$CCountedPointer@VCModeSitFloor@NCreatureMode@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA9B0, 25 bytes. Callees: none.
// Names (agentNF250, 2026-11-22, CODEXP27 L71): was the placeholder ??0?$CCountedPointer@VCUnprovenPointee_008aa9b0@@@@QAE@ABV0@@Z
//   (catalog CCountedPointer_CopyCtor_008aa9b0). FableWin.pdb S_PUB32 spells this converting instance; retail's only caller
//   0x009394F3 (CAIStateGroup_SitRoundCampfire::Update, landed 0x009392A0, which binds this spelling) converts its
//   CCountedPointer<CModeSitFloor> local into the CCreatureAction_SitDown argument. CModeSitFloor : CModeSitBase at offset 0
//   (engine/NCreatureMode_CModeSitFloor.h, RTTI), so the Data copy needs no adjustment: the same body as the plain copy constructor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

template CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(const CCountedPointer<NCreatureMode::CModeSitFloor>&);
