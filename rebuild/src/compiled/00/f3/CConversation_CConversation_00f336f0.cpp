// CConversation::CConversation() -- the def base, the CConversation vtable, then the four empty CCharString vectors
// (Speaker, Dialogue, Animation, AnimLoop: twelve zero dwords +0x2C..+0x58); Lines is left unset.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F336F0, length 56 bytes (64 with padding).
// Name: FableWin.pdb ??0CConversation@@QAE@XZ; TransferVectorOfSubComponents<CConversation> 0x00F334E0 builds its resize default
// element with it (RELOCATION_MATCH, chunk 1018), and the stride / member offsets are Ego_r.pdb CConversation's (0x5C,
// engine/CConversation.h). The vtable stored is 0x012E5FF0.
// Callee: ??0CDefClassBase@@AAE@XZ 0x0042D9E0 (FableWin.pdb, private): CSubDefClassBase's ctor is expanded around it, its own
// vtable store dead under CConversation's (overlay opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL /
// FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE).
// Leaf: ??0CConversation
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#include "engine/CConversation.h"

CConversation::CConversation()
{
}
