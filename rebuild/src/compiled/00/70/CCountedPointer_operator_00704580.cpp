// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = CScriptThing)
// Leaf: ??4?$CCountedPointer@VCScriptThing@@@@QAEAAV0@ABV0@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00704580. Callees: the count block's DeleteFunc (indirect), operator delete.
// Identity (two sources): FableWin.pdb name set ??4?$CCountedPointer@VCScriptThing@@@@QAEAAV0@ABV0@@Z; CScriptThing::PImp is
//   CCountedPointer<CScriptThing> (engine/CScriptThing.h) and the implicit CScriptThing assignment in 0x00CFE030 calls this address on
//   &sparkles.PImp (`add eax,4 ; push eax ; lea ecx,[esp+40h] ; call 0x00704580`); 20 landed callers bind this spelling (exe-scan probe).
//   The engine/CCountedPointer.h layout note already names 0x00704580 the CScriptThing operator=. Older labels named the T
//   CDiskFileWin32 (a propagated label); no landed row references the address with another T.
// NF122 (2026-11-20, q6278 / q6417 / q6471): was the free extern "C" __fastcall stand-in CCountedPtr_assign on local RefBlock /
//   CCountedPtr models (whose header comment named the wrong address 0x009A9BF0); re-authored as the in-class operator= of
//   engine/CCountedPointer.h, emitted as its own COMDAT (recipe of the landed 0x00754F10 / 0x00429C58).
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CScriptThing;
template CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);
