// NScript::Cjoss_test_scriptsData::Cjoss_test_scriptsData() -- base CBaseClassNonCopyable() (through the inline CScriptDataBase constructor) then this class's vtable 0x012C2730
// Leaf: ??0Cjoss_test_scriptsData@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD46A0, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset vtable ; mov eax,esi ; pop esi ; ret
// Name fix (agentNF24, land1011de): was the propagated stand-in C3DAnimationUtil::C3DAnimationUtil (??0C3DAnimationUtil@@QAE@XZ). The
//   vtable it stores, 0x012C2730, is NScript::Cjoss_test_scriptsData's (agentFAM08: the landed Cjoss_test_scriptsData Alloc / scalar deleting destructor store or name
//   it; engine/Cjoss_test_scriptsData.h); FableWin.pdb ??0Cjoss_test_scriptsData@NScript@@QAE@XZ. Header opt-in FABLE_RETAIL_NSCRIPT_DATA_CTOR declares the ctor.
#define FABLE_RETAIL_NSCRIPT_DATA_CTOR
#include "engine/Cjoss_test_scriptsData.h"

NScript::Cjoss_test_scriptsData::Cjoss_test_scriptsData()
{
}
