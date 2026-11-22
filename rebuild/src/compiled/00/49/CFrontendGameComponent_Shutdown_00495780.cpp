// CFrontendGameComponent::Shutdown() -- release the frontend font and temporary graphics bank and empty the save and world name lists
// Leaf: ?Shutdown@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe @ 0x00495780, 162 bytes, plain `ret`. Catalog label C3DMeshStripifier::~C3DMeshStripifier is wrong: the
// three callers 0x00496D40 / 0x00496DB7 / 0x00496E78 are the exit paths of CFrontendGameComponent::Run (vtable 0x01238768
// slot 2, 0x00496CF0) before it hands over to the next component, and the offsets are CFrontendGameComponent members.
// Name: Ego_r.pdb 0x0049E380 + FableWin.pdb 0x01A41790 ?Shutdown@CFrontendGameComponent@@AAEXXZ (private).
// Header port (NF233 / NF234, 2026-11-22, q12639 / q12650): engine/CFrontendGameComponent.h (the SHUTDOWN layout opt-in: PTempGraphics
//   +0x1DFC, MainFont +0x1E04, ProfileNames / SaveNames / WorldNames STLport vectors, SaveToLoad), engine/CCountedPointer.h, stlport/
//   fable_containers.h (clear() = erase(begin(), end()), the erase kept out of line by FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE as in
//   0x0042EBB6) and fable_string.h. Was a row-local CWideString / CCPPointerInfo / CCountedPointer / allocator-less std::vector /
//   CFrontendGameComponent model; the erase binder 0x00412450 now takes the STLport spelling (2 -> 1), the self leaf the PDB's private AAE
//   (was QAE; binder 0x00496CF0 moves with it).
// Body: MainFont.Reset(0) then PTempGraphics.Reset(0), expanded: the Data-first FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA release followed by
//   Data = 0, which dead-store elimination turns into retail's Info-then-Data stores (0x004957B9 / 0x004957BF ...).
// Fidelity: the CCountedPointer size-body opt-ins are scaffolding, not retail: this /O2 TU expands both Resets, and VC7.1 still emits
//   Reset / ReleaseData <CGraphicDataBank> and <const CFontBank> as pick-any COMDATs; the opt-ins compile them under optimize("s", on),
//   as their owner rows do (0x004194D1 / 0x0042FC13 / 0x00419108 / 0x0042F948), so each copy is its owner's body. DELETEDATA_DECL_ONLY
//   keeps the T-incomplete DeleteData copies out (owners 0x00419036 / 0x0042F864 are the virtual delete).
// Opt: /O2 (16-aligned entry).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SHUTDOWN
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CGameComponent.h"
#include "engine/CFrontendGameComponent.h"

void CFrontendGameComponent::Shutdown()
{
    MainFont.Reset(0);
    PTempGraphics.Reset(0);
    SaveNames.clear();
    WorldNames.clear();
}
