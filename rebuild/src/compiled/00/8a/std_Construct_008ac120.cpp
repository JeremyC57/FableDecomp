// std::_Construct<CScriptThing, CScriptThing>(CScriptThing* p, const CScriptThing& value) -- STLport placement copy: if p is not null, construct a copy of value there (the implicit CScriptThing copy: vtable, then the PImp counted pointer with its count bumped)
// Leaf: ??$_Construct@VCScriptThing@@V1@@std@@YIXPAVCScriptThing@@ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AC120, __fastcall (p in ecx, value in edx).
// NF123 (2026-11-20, q4397): re-spelled from the stand-in ?Cons_val@@YIXPAUSelfNode@@PAUSrcNode@@@Z (local SelfNode / SrcNode models and a
//   cast vtable address) to the STLport 4.6 _construct.h template on the shared engine/CScriptThing.h, the spelling the landed
//   0x00DEFB10 binds (owner decision 2026-11-03 D2: ??$_Construct@VCScriptThing@@V1@@std@@YIXPAVCScriptThing@@ABV1@@Z). The copy is
//   the implicit CScriptThing copy constructor (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY): CBaseClass vtable store 0x01238C8C, then the
//   CCountedPointer PImp copy (object +4, count block +8, `++*count` when non-null). The catalog's _Cons_val label is the Dinkumware
//   name; retail is STLport (EXTRA_RULES).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"

namespace std {
template void __fastcall _Construct<CScriptThing, CScriptThing>(CScriptThing* p, const CScriptThing& value);
}
