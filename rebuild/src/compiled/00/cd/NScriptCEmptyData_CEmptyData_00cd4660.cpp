// NScript::CEmptyData::CEmptyData() -- base CBaseClassNonCopyable() (through the inline CScriptDataBase constructor) then this class's vtable 0x012C2724
// Leaf: ??0CEmptyData@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4660, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset vtable ; mov eax,esi ; pop esi ; ret
// Name fix (agentNF24, land1011de): was the propagated stand-in C3DAnimationUtil::C3DAnimationUtil (??0C3DAnimationUtil@@QAE@XZ). The
//   vtable it stores, 0x012C2724, is NScript::CEmptyData's (agentFAM08: the landed CEmptyData Alloc / scalar deleting destructor store or name
//   it; engine/CEmptyData.h); FableWin.pdb ??0CEmptyData@NScript@@QAE@XZ. Header opt-in FABLE_RETAIL_NSCRIPT_DATA_CTOR declares the ctor.
#define FABLE_RETAIL_NSCRIPT_DATA_CTOR
#include "engine/CEmptyData.h"

NScript::CEmptyData::CEmptyData()
{
}
