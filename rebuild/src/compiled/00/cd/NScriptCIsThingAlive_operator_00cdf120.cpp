// NScript::CIsThingAlive::operator()() -- the script predicate: true while its thing is alive (Thing.IsAlive(), CScriptThing slot 75, a virtual tail call on the member)
// Leaf: ??RCIsThingAlive@NScript@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDF120, 11 bytes, thiscall: `add ecx,4 ; mov eax,[ecx] ; jmp [eax+12Ch]`.
// Identity (proven, two sources): (1) slot 0 of the predicate vtable 0x012C32F0 (no RTTI locator), whose slot 2 is the landed
//   ??_GCIsThingAlive@NScript@@UAEPAXI@Z 0x00CDF160 and slot 1 the 0x10-byte Clone 0x00CDF130; the same vtable is stored by the
//   landed ~CIsThingAlive 0x00E241B0 and by every SetPredicate(CIsThingAlive(Me)) site (0x00D6A360 ...); (2) Ego_r.pdb 0x00CC4A70 and
//   FableWin.pdb name slot 0 ??RCIsThingAlive@NScript@@UBE_NXZ (`bool operator()() const`, the CPredicateBase pure slot 0).
//   Catalog row "_gapscan::sub_00cdf120" carried no name.
// Callee: CScriptThing::IsAlive (virtual slot 75, +0x12C; ?IsAlive@CScriptThing@@UBE_NXZ 0x004AB130) on the member Thing at +4:
//   VC7.1 does not devirtualise a call on an embedded member object, so the call goes through its vtable as a tail jump.
// Model: the shared retail headers engine/CIsThingAlive.h (CPredicateBase, Thing +0x04, 0x10) and engine/CScriptThing.h (79 slots).
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CIsThingAlive.h"

bool NScript::CIsThingAlive::operator()() const
{
    return Thing.IsAlive();
}