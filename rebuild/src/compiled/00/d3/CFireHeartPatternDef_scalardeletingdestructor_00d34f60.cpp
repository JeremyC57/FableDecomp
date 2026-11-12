// CFireHeartPatternDef::`scalar deleting destructor'(unsigned int flags) -- ~CFireHeartPatternDef() out of line, then free this
// when bit 0 of flags is set; returns this
// Leaf: ??_GCFireHeartPatternDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34F60, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012CCC50, the vtable CFireHeartPatternDef::CFireHeartPatternDef 0x00F33DC0 stores (slots 18 / 19 the
//   landed CFireHeartPatternDef::Transfer 0x00D35070 / Copy 0x00D34FB0); the destructor it calls is the landed
//   CFireHeartPatternDef::~CFireHeartPatternDef 0x00D34F80 (frees PadActive's storage). Ego_r.pdb / FableWin.pdb name the class's
//   ??1 / Transfer / Copy; ??_G is the deleting destructor of the vtable.
// Shape: `new` of the class with the constructor defined inline is this TU's emitter of the vtable and so of ??_G (descriptive
//   emitter name, not graded).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CFireHeartPatternDef.h"

inline CFireHeartPatternDef::CFireHeartPatternDef()
{
}
CFireHeartPatternDef* NewFireHeartPatternDef()
{
    return new CFireHeartPatternDef;
}
