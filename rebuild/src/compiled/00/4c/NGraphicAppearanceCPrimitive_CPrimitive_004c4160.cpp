// NGraphicAppearance::CPrimitive::~CPrimitive() -- destroy the CEnginePrimitiveHandle PrimitiveHandle at +4: tell the primitive it is losing its last handle, drop the counted Data pointer, then the CBaseClass base
// Leaf: ??1CPrimitive@NGraphicAppearance@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4160, length 94 bytes. Callees: the primitive's virtual slot 3 (+0x0C, with
// the handle) and slot 1 (+0x04, Destroy when the count reaches zero), CBaseClass::~CBaseClass @ 0x0099A2E0 (tail jump).
// the owner's destructor collapses to `add ecx,4 ; jmp ??1CEnginePrimitiveHandle`); the last-handle test is an inline
// `IsUnique()` (`Object != 0 && Object->RefCount == 1`) and the call goes through `operator->`, which keeps the object
// in eax and moves it to ecx (a direct member test loads it into ecx); `Data = 0` is the release plus a second
// unconditional zero store, and the member destructor releases a third time; the out-of-line CBaseClass destructor
// becomes the tail jump.
// Naming (agentNF31, 2026-11-12): was SPrimitiveHandleHolder_004c4160::~SPrimitiveHandleHolder_004c4160 (named by effect) over
//   local handle models. FableWin.pdb 0x01D23DB0 + Ego_r.pdb 0x0042A230 ??1CPrimitive@NGraphicAppearance@@QAE@XZ; the 16-byte
//   NGraphicAppearance::CPrimitive (Ego_r: Type +0, PrimitiveHandle +4, BankIndex +0xc; engine/NGraphicAppearance_CPrimitive.h)
//   destroys exactly the handle at +4 and nothing else; the landed caller RemovePrimitive 0x004BF920 binds this symbol.
//   Handle and counted pointer from the shared headers (FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR: inline Clear then the member's release).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#include "engine/NGraphicAppearance_CPrimitive.h"

NGraphicAppearance::CPrimitive::~CPrimitive()
{
}
