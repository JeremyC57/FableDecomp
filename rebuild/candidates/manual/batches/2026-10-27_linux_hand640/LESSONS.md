# Chunk 640 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A matrix written through the inline `float& operator()(int,int)` keeps its integer stores in source order**
  (0x00AA3AF0). VC7.1 hoists constant and integer stores to named union members (E34 = pos.Z, the 0,0,0 last row,
  E44 = 1.0f) up past the x87 stores (to just after E11). The FableWin inline accessor `??RCMatrix4x4@@QAEAAMHH@Z` over a
  `float M[4][4]` view keeps them after E33, as retail does. Try this on any matrix row whose residue is "integer stores
  hoisted". Chained zeros, reversed order and a named position local do not move them.
- **The shared NScript headers fix the callee-saved register for &Me** (0x00DF48F0). The headers are CEntityScriptBase.h,
  CScriptThing.h, CGameScriptInterface.h and CIsThingConscious.h : CPredicateBase.
  - A local CM_Experience model with a flat CScriptThing put &Me in ebx; the header model gives edi, as retail does.
  - Define the predicate's ctor/dtor inline after the include (retail expands them).
  - Do NOT define `~CScriptThing` inline (agent636).
  - A `GetThingWithScriptName(CCharString("X", -1))` temporary is built after the vtable load. The implicit conversion from
    `"X"` builds the string first.
- **engine/CDefPointer.h's `Assign` loads the source pointee once** (0x00688D40). Retail's inline `operator=` re-reads
  `other.Object` after the release (`mov [this+30h],0 ; mov eax,[edi]`), so `Object = other.Object` is a separate read.
  Both release shapes (short in operator=, long in the temporary's dtor) come out right from the header's `Release()`.
- **VC7.1's commutative x87 operand choice depends on the whole function, not on the source order** (probe, 0x00C1D1C0).
  An isolated `m.E23 * m.E12` and `m.E12 * m.E23` both load E23 first. The same product inside the full determinant loads
  E12 first. An isolated `E13 * E32` loads E32 first, but E13 first in the determinant. Reference vs `this` bases also flip
  some pairs. Park "operand order" rows fast (third confirmation after agent634/635).
- **An LTCG two-pointer member keeps its PDB identity as a `__fastcall` member** (0x00C1D1C0). `void __fastcall
  CMatrix3x4::SetInverse(const CMatrix3x4&)` gives this=ecx and the argument in edx with a plain `ret`. It mangles
  `QIAE`, not the PDB's `QAE` (a review item, not a match).
- Register-colouring ties confirmed again: count/index (0x004FEC80) and this/def-temporary (0x00688D40) survived PDB
  respelling, local order and accessor forms.

Review at landing:
- Grades per row this session (one lever each):
  - 00688d40: 5, then 4 for 00697cc0 (above the brief's 4, within its ~6);
  - 004fec80, 00c1d1c0, 00be1c10, 00f36a30: 3 each;
  - 00986ed0, 00a22e10: 2 each;
  - the matches: 00aa3af0 2, 00df48f0 3.
- Respell callers per NAME_FIXES.tsv:
  - 0x00697CC0 is private (AAE).
  - 0x00663340 returns `CDefPointer<const CHeroCombatDef>`.
  - 0x00A14440 is AccurateNormalise. GOTCHAS chunks 160/181 say "C3DVector::Normalise"; the body (fsqrt + divide) and the
    Ego_r out-of-line set say AccurateNormalise. The lander should settle this line.
- 00df48f0 uses the established `pGlobalScriptDef` (0x0143E90C) spelling. Ego_r.pdb names it
  `NGlobalScriptDefs::pGlobalScriptDef` (a CDefPointer<CScriptDef>). The 100+ landed users share the spelling.
- 00aa3af0's `float M[4][4]` union view is a descriptive member name. The union's second member is not in
  struct_layouts_egor.tsv.
- The 00c1d1c0 differ is a `__fastcall` member (QIAE) standing in for the PDB thiscall `?SetInverse@CMatrix3x4@@QAEXABV1@@Z`
  under the LTCG convention.
- Local models next to existing headers:
  - 004fec80 differ: CWorldMap (engine/CWorldMap.h flattens the vptr);
  - 00697cc0 differ: CGameCameraManager (the header flattens the vptr and Camera);
  - 00688d40 differ: CDefPointer (see header debt in NOTES.md).
