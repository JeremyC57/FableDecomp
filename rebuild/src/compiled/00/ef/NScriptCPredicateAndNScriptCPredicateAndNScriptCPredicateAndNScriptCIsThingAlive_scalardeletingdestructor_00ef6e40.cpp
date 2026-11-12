// NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>::`scalar deleting destructor'(unsigned int) -- destroy a three-level And of four CIsThingAlive predicates and free it when bit 0 is set
// Leaf: ??_G?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF6E40, 90 bytes (+6 int3 padding), `__fastcall`, `ret 4`.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x4), operator delete @ 0x00BFE9BC.
// Vtables: outer And 0x012F6780, middle And 0x012F6628, inner And 0x012E1B70, the shared CPredicate base 0x012C2FF4;
//   CIsThingAlive is 16 bytes (vtable 0x012C32F0 + a 12-byte CScriptThing, see landed 0x00E6EA30).
// Layout: each CPredicate_And<A,B> is a CPredicate (vptr) followed by its two operands by value, so the nested left
//   operand's vptr sits 4 bytes after its parent's: outer vptr +0x0, middle +0x4, inner +0x8; the four leaves are at
//   +0x0C, +0x1C (inner), +0x2C (middle) and +0x3C (outer). The catalog's `vector deleting destructor` label is the
//   scalar ??_G (no array loop).
// Name fix (agentNF24, land1011de): the local NScript::CPredicate<CScriptThing> / CIsThingAlive / CPredicate_And / NScript::CScriptThing
//   stand-ins replaced by the shared engine/CPredicateBase.h, CIsThingAlive.h, CPredicate_And.h and CScriptThing.h: the base vtable
//   0x012C2FF4 now binds ??_7CPredicateBase@NScript@@6B@ (slot 2 the landed ??_GCPredicateBase@NScript@@UAEPAXI@Z 0x00CDDB40; was
//   ??_7?$CPredicate@UCScriptThing@NScript@@@NScript@@6B@); the And destructor is inline (opt-in FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR).
// Shape: each And declares a destructor (its own vtable store), destroying the right operand, then the left one (whose
//   inline dtor stores its vtable at +4 / +8 first); the three CPredicate base dtors (user-declared, inline) reset the
//   vptrs innermost first, and VC7.1 merges them into one `mov eax,0x012C2FF4` and three stores (+8, +4, +0).
void __cdecl operator delete(void* block);               // 0x00BFE9BC
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR
#include "engine/CScriptThing.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"

template class NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
    NScript::CIsThingAlive>, NScript::CIsThingAlive>;
