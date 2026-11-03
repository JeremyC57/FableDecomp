// CCodaObjectBase::~CCodaObjectBase() -- GameCODA 2.10 root class destructor: nothing to release, MSVC still re-installs the
//   root vptr (`mov dword ptr [ecx], 0x012B5868 ; ret`).
// Leaf: ??1CCodaObjectBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C830, 7 bytes.
// Name (owner decision 2026-11-03 D3): the root (vtable 0x012B5868) is GameCODA's CCodaObjectBase (pass 11 CPKObject; Codex pass 12
//   CodaEnvironment.h); the EH unwind funclets 0x012163D0 / 0x012163F0 call this destructor by that name. Earlier label: a generic
//   "CBase" / SetMemberConst_00c3c830 tiny-body shape.
#include "engine/CCodaObjectBase.h"

CCodaObjectBase::~CCodaObjectBase()
{
}
