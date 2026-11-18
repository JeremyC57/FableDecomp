// CConversation::CConversation(const CConversation& other) -- copy constructor: the def base copied out of line, own vtable, Lines, then the four CCharString vectors copied out of line
// Leaf: ??0CConversation@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54CA0, length 79 bytes (+ int3 padding).
// Identity: FableWin.pdb ??0CConversation@@QAE@ABV0@@Z; engine/CConversation.h (Ego_r.pdb layout at retail offsets: Lines +0x28, Speaker /
//   Dialogue / Animation / AnimLoop vector<CCharString> +0x2C..+0x50). Vtable 0x012E5FF0.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C (through the inline CSubDefClassBase copy), the STLport
//   vector<CCharString, allocator<CCharString> > copy constructor @ 0x007ACEA0 (x4; q11134).
// NF87: moved off the row-local CDefClassBase_Retail / CRetailVector_CCharString models onto engine/CConversation.h, so 0x007ACEA0 binds the
//   STLport vector copy-ctor spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#include "engine/CConversation.h"

CConversation::CConversation(const CConversation& other)
    : CSubDefClassBase(other)
    , Lines(other.Lines)
    , Speaker(other.Speaker)
    , Dialogue(other.Dialogue)
    , Animation(other.Animation)
    , AnimLoop(other.AnimLoop)
{
}
