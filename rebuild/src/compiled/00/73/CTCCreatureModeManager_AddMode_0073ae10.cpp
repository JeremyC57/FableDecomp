// CTCCreatureModeManager::AddMode(long modeType) -- allocate a mode of the given type and add it (by-value counted pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE10, length 92 bytes (+4 int3). Called by SetupDefaultMode
// (0x0073AF60) with the def's DefaultCreatureMode.
// Callees: CTCCreatureModeManager::AllocateMode(long) @ 0x0073A8E0 (returns CCountedPointer<CModeBase> through the hidden
// pointer = the 8-byte frame local), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0 (by-value
// argument, destroyed by the callee), operator delete @ 0x00BFE9BC.
// Shape: the by-value argument is copy-constructed in the argument area (`sub esp,8 ; mov eax,esp`, the count block
// add-ref'd with `inc [esi]`); the bool result rides in bl across the local's destructor. The count block {references,
// __fastcall destroy function, object} is the CCountedPointer layout of the landed Reset family (0x00836580): the release
// is `--references == 0` -> destroy(object) -> delete block, with the block pointer cached in esi from the copy.
// AllocateMode gets a same-TU `auto_inline(off)` stand-in body that writes only the returned pointer: VC7.1 then knows
//   the local's address (the hidden return slot) does not escape, so AddMode cannot change it and the count block stays
//   in esi across the call, as retail (an extern declaration re-reads the slot after AddMode).
// Stand-in body (the real AllocateMode is out of line in retail): it writes only the returned pointer, so VC7.1
// knows the local's address does not escape the call.
// Leaf: ?AddMode@CTCCreatureModeManager@@QAE_NK@Z
// NF70 (round 7, land1117fu q10741): moved onto engine/CTCCreatureModeManager.h + CCountedPointer.h (NF62's 0x0073A8E0 define block): the
//   callee is the protected Ego_r.pdb ?AllocateMode@CTCCreatureModeManager@@IAE?AV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@K@Z
//   (was the local CModePointer / SCountBlock model with a same-TU stand-in AllocateMode body over two made-up callees), and the row itself is
//   FableWin.pdb ?AddMode@CTCCreatureModeManager@@QAE_NK@Z (unsigned long mode type; was _NJ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_COUNT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE_BODY
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

bool CTCCreatureModeManager::AddMode(unsigned long modeType)
{
    CCountedPointer<CModeBase> mode = AllocateMode(modeType);
    return AddMode(mode);
}
