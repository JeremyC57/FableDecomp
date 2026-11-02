// CAIStateGroupBase::CAIStateGroupBase() -- CBaseClass(), own vtable, then the Name string built out of line
// Leaf: ??0CAIStateGroupBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD255, 26 bytes, thiscall, `ret`.
// Identity: stores vtable 0x01288C60 (RTTI .?AVCAIStateGroupBase@@ : CBaseClass); Ego_r.pdb / FableWin.pdb ??0CAIStateGroupBase@@QAE@XZ;
//   the base ctor of every CAIStateGroup_* ctor (chunk 703 NAME_FIXES).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (engine/CBaseClass.h), CCharString::CCharString 0x0099E4B0 (??0CCharString@@QAE@XZ).
// Layout: engine/CAIStateGroupBase.h's retail branch (Ego_r.pdb members: PBrain +0x04, ScriptAIPriority +0x08, Name +0x0C,
//   Terminated / Priority / Enabled +0x10..+0x12; 0x14 bytes).
// Shape: user default ctor, empty: CBaseClass(), the vtable store, then Name's default ctor; the other members stay uninitialised.
// History: generated in a chunk batch ("One of 16 bodies of this shape") over CBase_0099a2d0 / CMember_0099e4b0 with the truncated Leaf
//   ??0CAIStateGroupBase@; re-authored on the header in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

CAIStateGroupBase::CAIStateGroupBase()
{
}
