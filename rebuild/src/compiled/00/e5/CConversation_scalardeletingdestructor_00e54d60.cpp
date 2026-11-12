// CConversation::`scalar deleting destructor'(unsigned int flags) -- ~CConversation() out of line, then free this when bit 0 of
// flags is set; returns this
// Leaf: ??_GCConversation@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54D60, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012E5FF0, the vtable the landed CConversation::CConversation 0x00F336F0 (FableWin.pdb
//   ??0CConversation@@QAE@XZ) and 0x00E54CA0 store (slots 18 / 19 the landed CConversation::Transfer 0x00E579E0 / Copy
//   0x00E54D10); it calls the landed CConversation::~CConversation 0x00E54D80. FableWin.pdb ??_GCConversation@@UAEPAXI@Z.
// Shape: `new` of the class with the constructor defined inline is this TU's emitter of the vtable and so of ??_G (descriptive
//   emitter name, not graded).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CConversation.h"

inline CConversation::CConversation()
{
}
CConversation* NewConversation()
{
    return new CConversation;
}
