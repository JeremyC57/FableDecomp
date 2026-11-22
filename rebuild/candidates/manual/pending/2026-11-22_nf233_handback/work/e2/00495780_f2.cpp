// CFrontendGameComponent::Shutdown() -- release the frontend font and temporary graphics bank and empty the save and world name lists
// Leaf: ?Shutdown@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe @ 0x00495780, 162 bytes, plain `ret`. Catalog label C3DMeshStripifier::~C3DMeshStripifier is wrong: the
// three callers 0x00496D40 / 0x00496DB7 / 0x00496E78 are the exit paths of CFrontendGameComponent::Run (vtable 0x01238768
// slot 2, 0x00496CF0) before it hands over to the next component, and the offsets are CFrontendGameComponent members.
// Name: Ego_r.pdb 0x0049E380 + FableWin.pdb 0x01A41790 ?Shutdown@CFrontendGameComponent@@AAEXXZ (private).
// Header port (NF233, 2026-11-22, q12639): engine/CFrontendGameComponent.h (the NF233 SHUTDOWN layout opt-in: PTempGraphics +0x1DFC,
//   MainFont +0x1E04, ProfileNames / SaveNames / WorldNames STLport vectors, SaveToLoad), engine/CCountedPointer.h (default ReleaseData:
//   --RefCount, at zero DeleteFunc(Data) and operator delete 0x00BFE9BC; Info = 0, Data = 0), stlport/fable_containers.h (clear() =
//   erase(begin(), end()), the erase kept out of line by FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE as in 0x0042EBB6) and fable_string.h.
//   Was a row-local CWideString / CCPPointerInfo / CCountedPointer / allocator-less std::vector / CFrontendGameComponent model; the erase
//   binder 0x00412450 now takes the STLport spelling (2 -> 1), the self leaf the PDB's private AAE (was QAE; binder 0x00496CF0 moves too).
// Opt: /O2 (16-aligned entry).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY
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
