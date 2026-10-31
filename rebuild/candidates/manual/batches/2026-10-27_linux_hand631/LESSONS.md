# Chunk 631 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Spilled struct-copy scalars keep 8-byte pair slots** (0x00A57960): copy points into `C2DVector` locals with a memberwise
  user copy ctor, `C2DVector p2 = Point2;`. VC7.1 scalarises each copy. Only the members that spill get a frame word, but at
  their offset inside the struct's 8-byte slot, which gives retail's gapped 0x20 frame. Plain float locals pack the frame,
  and an implicit (trivial) copy moves both words.
- **<math.h>'s `float pow(float, float)` is not the same as a hand-written float helper** (0x00C59C60): `m = pow(2.0f, x);`
  through the real header overload put `pop edi` between the last `_CIpow` call and its `fstp`, as retail does.
  `(float)pow(2.0, x)` and `inline float PowF(float, float) { return (float)pow(...); }` store first. Use the real CRT
  header form for float math calls.
- **The shared-false-block rule also holds for a last test that returns a value** (0x00C56D70, extends GOTCHAS b51/00bfc600):
  `if (Play() != 0) return false; return true;` keeps the merged false block after the FIRST test, and VC still folds the tail
  to `neg eax ; sbb al,al ; inc al`. `return hr == 0 ? true : false` moves the block after the last test. Goto, nested-if,
  result-variable and inline-helper forms are inert.
- **A raw float pointer re-keys x87 operand order per value** (0x00A5D000): reading only the old translation x through
  `const float* s = &source.m[0][0]; ... s[3]` gave retail's source-first order in rows 1 and 3, while row 2 stayed m-first.
  Pointer-reading the whole translation flips row 2 as well. The k-lever (0..7 unused locals) is inert in this call-free body.
- **A same-TU noinline stand-in for the search call caches the vector start for the freeing dtor** (0x00725B70, as the landed
  sibling 0x00726BE0): it fixed a 205v201 size residue. Write the stand-in as an out-of-class `__declspec(noinline)` template
  member definition. An in-class body is inlined even under `#pragma auto_inline(off)`. What is left is frame-slot reuse.
- **Respell from the PDB before tuning registers** (0x00943C20, 0x00AEB600): the member and callee types changed (PTask
  CIntelligentPointer<CVillageTaskCarryCrate>, GetBestCrateTaskForThing(const CThing&), InterpolateTo(const&, float), const
  members). In both rows the register residue stayed, but the names are now PDB-exact.

Review at landing:
- Rows that took more than 4 grades: 00725b70 (7 probes), 00a5d000 (k-sweep + 12 probes), 00c56d70 (8), 00943c20 (7),
  00aeb600 (5), 00c351b0 (6). Each probe changed one thing; sweeps ran in parallel with distinct QTAGs.
- reloc_bindings.tsv holds stale spellings for several addresses in this chunk (NAME_FIXES.tsv). Two need landed files
  changed:
  - std_foreach_00726660 instantiates the wrong functor (CApplyAdditionalImpulseIfInCombatAndTooClose over CThing**).
  - The landed CAIStateGroup_* sources spell 0x007EE540 as the free `GFGetPVillageInterface` __fastcall. The PDB has
    CTCVillageMember::GetPVillage() const.
- 0x00725F10 GetAllThingsInRoughArea has three different bound spellings, none of them the PDB template-member form (void,
  const). The 00725b70 differ uses the template member. The exact retail template arguments (vector<CThingCreatureBase*> vs
  Ego_r's vector<const CThing*>) are unproven.
- 0x00C351B0 binding `?Start@CVirtualPlayer@@QAEXJPAUSEventLevel@@M@Z`: the parameter types are descriptive and contradict
  the body (see NOTES). Decide the spelling before this row lands.
- 0x012BEEA0 should be the SDK GUID name `_DSPROPSETID_EAX20_BufferProperties` (NAME_FIXES). The 00c56d70 src still uses the
  bound `g_eaxSourcePropertySetGuid_012beea0` spelling, so landing does not depend on the respell.
