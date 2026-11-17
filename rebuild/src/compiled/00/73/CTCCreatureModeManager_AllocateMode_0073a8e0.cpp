// CTCCreatureModeManager::AllocateMode(unsigned long modeType) -- pick one of the creature modes registered for the type at random and build it for the parent thing, wrapped in a new counted pointer
// Leaf: ?AllocateMode@CTCCreatureModeManager@@IAE?AV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A8E0, 200 bytes (`ret 8`: the hidden result pointer and the type).
// Callees: RegisteredModes (0x013BAD44) const UpperBound 0x0073D250 / const LowerBound 0x0073D210 (the inline CVectorMap::Count) and
//   non-const LowerBound 0x0073D0E0, CTCBase::PeekWorld 0x00686A80, CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890 (inline GFRandom),
//   the registered AllocFunc (`__fastcall`, the thing in ecx), operator new 0x00BFEA1A (the CCPPointerInfo block), DeleteData 0x007180B0.
// NF62 (2026-11-17): ported onto engine/CTCCreatureModeManager.h (REGISTRY + overlay ALLOCATEMODE) / CVectorMap.h (UPPERBOUND + overlay
//   COUNT) / CCountedPointer.h / CTCBase.h / CWorld.h / GFRandom.h (the local registry, count block, counted pointer, CTCBase and CWorld
//   models deleted).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_COUNT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE
#define FABLE_RETAIL_CWORLD
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/GFRandom.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureModeManager.h"

CCountedPointer<CTCCreatureModeManager::CModeBase> CTCCreatureModeManager::AllocateMode(unsigned long mode_type)
{
    long count = RegisteredModes.Count(mode_type);
    unsigned long index = GFRandom(count, PeekWorld().GetWorldSeed());
    CThing& thing = GetParentThing();
    return CCountedPointer<CModeBase>(RegisteredModes.LowerBound(mode_type)[index].second.AllocFunc(thing));
}
