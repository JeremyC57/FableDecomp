// CScriptDataBase::CScriptDataBase() -- base CBaseClassNonCopyable() then this class's vtable 0x012C270C
// Leaf: ??0CScriptDataBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4500, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset vtable ; mov eax,esi ; pop esi ; ret
// Name fix (agentNF24, land1011de): was the propagated stand-in C3DAnimationUtil::C3DAnimationUtil (??0C3DAnimationUtil@@QAE@XZ). The
//   vtable it stores, 0x012C270C, is the abstract CScriptDataBase's (agentFAM08: slot 0 ??_GCScriptDataBase@@UAEPAXI@Z 0x00CD4530, slots
//   1/2 _purecall); FableWin.pdb ??0CScriptDataBase@@QAE@XZ; engine/CScriptDataBase.h opt-in FABLE_RETAIL_CSCRIPTDATABASE_CTOR.
#define FABLE_RETAIL_CSCRIPTDATABASE_CTOR
#include "engine/CScriptDataBase.h"

CScriptDataBase::CScriptDataBase()
{
}
