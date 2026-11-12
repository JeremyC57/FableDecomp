// NScript::CDanyalTestScriptsData::CDanyalTestScriptsData() -- base CBaseClassNonCopyable() (through the inline CScriptDataBase constructor) then this class's vtable 0x012C273C
// Leaf: ??0CDanyalTestScriptsData@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD46E0, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset vtable ; mov eax,esi ; pop esi ; ret
// Name fix (agentNF24, land1011de): was the propagated stand-in C3DAnimationUtil::C3DAnimationUtil (??0C3DAnimationUtil@@QAE@XZ). The
//   vtable it stores, 0x012C273C, is NScript::CDanyalTestScriptsData's (agentFAM08: the landed CDanyalTestScriptsData Alloc / scalar deleting destructor store or name
//   it; engine/CDanyalTestScriptsData.h); FableWin.pdb ??0CDanyalTestScriptsData@NScript@@QAE@XZ. Header opt-in FABLE_RETAIL_NSCRIPT_DATA_CTOR declares the ctor.
#define FABLE_RETAIL_NSCRIPT_DATA_CTOR
#include "engine/CDanyalTestScriptsData.h"

NScript::CDanyalTestScriptsData::CDanyalTestScriptsData()
{
}
