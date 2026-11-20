// CDisplayManager::CCreateDeviceParameters::CCreateDeviceParameters(const CCreateDeviceParameters&) -- memberwise copy
// Leaf: ??0CCreateDeviceParameters@CDisplayManager@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C11A0, length 73 bytes. No callees.
// Identity: Ego_r.pdb 0x00B26590 / FableWin.pdb 0x0301CCB0 ??0CCreateDeviceParameters@CDisplayManager@@QAE@ABU01@@Z (the nested parameter
//   block of engine/CDisplayManager.h); its one retail caller is CDisplayManager::ProcessLostDevice 0x009C00C0 (call at 0x009C011E).
// Shape: the compiler-generated copy constructor. CCreateDeviceParameters declares no copy constructor (FableWin.pdb lists only its
//   default constructor besides this one), but its C2DExtentsI member has a user copy constructor (FableWin.pdb ??0C2DExtentsI@@QAE@ABV0@@Z),
//   so the implicit copy is memberwise (`this` to eax, every member through edx, the bool as one byte) and is emitted out of line as a
//   pick-any COMDAT where a caller does not expand it (ProcessLostDevice; ChangePresentationInterval / SetDisplayMode / SetExclusiveMode
//   expand it).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#include <new>
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"

// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): an unreferenced anonymous-namespace function whose copy makes this TU emit
//   the implicit constructor as the pick-any COMDAT every includer emits.
namespace {
void ReferenceCreateDeviceParametersCopy(const CDisplayManager::CCreateDeviceParameters& parameters, CDisplayManager::CCreateDeviceParameters* out)
{
    new (out) CDisplayManager::CCreateDeviceParameters(parameters);
}
}
