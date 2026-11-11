// NUISystem::CMouseCursor::GetPosition() const -- the cursor position: the sprite position, copied out by value
// Leaf: GetPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C5D0, 34 bytes (+ int3 padding; the 48-byte catalog row includes 14 of it),
//   `ret 4` (__fastcall, hidden C2DVector result pointer returned in eax).
// Label: the catalog name "CPersistTraits<EInputKey>::TransferIn" is propagated and wrong: the only reference is slot 16
//   (+0x40) of the NUISystem::CMouseCursor vtable 0x0124C22C, the slot landed UI code calls GetPosition() const
//   (C2DVector by hidden pointer, e.g. 0x00544520); the base slot is NUISystem::CSprite::GetPosition @ 0x0052E890
//   (returns the C2DVector at +0x34).
// Callees: NUISystem::CComponent::GetPosition() const @ 0x0052E890 (a qualified, non-virtual call; agentNF19: Ego_r ?GetPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ, the CComponent body CSprite inherits).
// Shape: the base result lands in a named local (its own frame slot), then the result is copied out member-wise through
//   C2DVector's user copy constructor (X via fld/fstp, Y through a GPR). Speed-optimised (default) TU.
// agentNF19: the class models now come from the shared headers (NUISystem::CMouseCursor : CSprite : CChangingStateComponent :
//   CComponent); the override is the opt-in FABLE_RETAIL_CMOUSECURSOR_GETPOSITION declaration.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMOUSECURSOR_GETPOSITION
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/NUISystem_CMouseCursor.h"

namespace NUISystem {
C2DVector CMouseCursor::GetPosition() const
{
    C2DVector position = CComponent::GetPosition();
    return position;
}
}
