// NScript::CalextestData::CalextestData() -- base CBaseClassNonCopyable() (through the inline CScriptDataBase constructor) then this class's vtable 0x012C2718
// Leaf: ??0CalextestData@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4620, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset vtable ; mov eax,esi ; pop esi ; ret
// Name fix (agentNF24, land1011de): was the propagated stand-in C3DAnimationUtil::C3DAnimationUtil (??0C3DAnimationUtil@@QAE@XZ). The
//   vtable it stores, 0x012C2718, is NScript::CalextestData's (agentFAM08: the landed CalextestData Alloc / scalar deleting destructor store or name
//   it; engine/CalextestData.h); FableWin.pdb ??0CalextestData@NScript@@QAE@XZ. Header opt-in FABLE_RETAIL_NSCRIPT_DATA_CTOR declares the ctor.
#define FABLE_RETAIL_NSCRIPT_DATA_CTOR
#include "engine/CalextestData.h"

NScript::CalextestData::CalextestData()
{
}
