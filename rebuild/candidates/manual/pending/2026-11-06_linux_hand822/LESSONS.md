# Chunk 822 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A final thread check after an `if (IsXbox()) {..hint loop..} else {..hint loop..}` that retail compiles as ONE call with its own pad
  is written at the end of BOTH arms (`if (T) return;` twice): VC7.1 cross-jumps the two into one call and keeps the hint-loop returns
  on their own shared pad; one check after the if / else merges the Xbox loop returns into the final pad (0x00DB86B0, md 91 -> 0).
- A nested inline dtor (predicate temp's ~CIsThingConscious -> member ~CScriptThing) that retail expands one level only is a threshold
  case: with an inline ~CScriptThing body and a __forceinline outer dtor, VC7.1 expands both levels; the exact source grades
  RELOCATION_MATCH under the diagnostic /d2inlT60 but NOT under T72/T66/T40 (the /d2inlT knob is non-monotonic) (0x00DAF080).
- Leaving a polymorphic predicate dtor implicit (header opt-in) drops the dtor's vtable store: retail's `mov reg,<vtable>` at the dtor
  site means the dtor is user-declared (0x00DAF080 b10, CIsThingConscious vtable 0x012C2FE8).
- A probe that writes one return as `if (!T) { rest }` (falls to the loop increment, not retail semantics) is a fast way to prove which
  single return site holds an extra expanded dtor pad: if bytes and size then match except that jne target, the residue is that one
  pad's inline decision (0x00DAC760: 5867v5867, only 0x00DADB4D's target off).
- The implicit MovieBase dtor in a 3.3 KB Main stays the COMDAT call even with the chunk-821 SAY do{}while(0), do-whiles round the
  whole block / whole if, and nested scopes; /d2inlS15 (not T72/T78/T90) expands it, and a reduced body holding the same block expands
  it: heat and caller size combine, so a park on "only under S15" is a caller-size gap, not a missing source lever (0x00D18DF0).
- `vector.size()` vs `end() - begin()` in an index argument: size() is retail's spelling (it leaves the dead `add ecx,begin` of the
  inlined member's this), but in 0x00DADF80 it now colours parent eax / size ecx (retail ecx / eax) and its extra inline bodies move a
  ~CScriptThing pad split; chunk 797 measured size() at size-exact, so a header moved since.

Review at landing:
- All four parked rows needed more than 4 grades (16 / 10 / 9 / 9 attempt lines in STATUS.tsv); 0x00DB86B0 took 3.
- 0x00EF7AB0 has seven spellings bound in reloc_bindings.tsv (placeholder `CScriptThingFunctor_00ef7ab0`, `CIsThing`,
  `CIsThingAlive@N_00ef7ab0`, `CIsThingConscious@@` global, `CIsThingStored@NScript`, `CIsThingAliveEndGame`): one retail address, one
  name -- the vtable 0x012C2FE8 store makes it `??1CIsThingConscious@NScript@@UAE@XZ`; check each caller row (a CIsThingStored /
  CIsThingAlive binding here may be a mis-modelled predicate class in that row) before respelling.
- 0x00DAEBF0 GetVillagerSpeechIndex still landed under a global `CNOVI_Villager` (chunk 797 NAME_FIXES unapplied).
