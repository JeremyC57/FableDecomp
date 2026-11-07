# Chunk 837 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00EFA4F0) A level-script Main wrapped in `while (!IsActiveThreadTerminating()) { ...; NewScriptFrame(); }` expands the implicit
  MovieBase dtor at all 7 exits (one merged tail); the chunk 813/828 form `for (;; Game->NewScriptFrame()) { if (T) return; ... }`
  emits the same rotated loop but drops it to 4 expanded sites (retail 3), with the later exits and the scope end calling 0x006E7B80.
  `if (T) return; do { ... } while (!T);` grades like the while form.
- (0x00EFA4F0) Without the declared-only ~CCountedPointer<MovieBase> spec the PImp release inlines and only ONE movie dtor site expands;
  with it all expand: the spec changes the per-site cost, and /d2inlS (any value) does not move this split; /d2inlT is a cliff (<=75
  default, >=80 everything expands).
- (0x00EF4B40) An STB (CScriptGameResourceObjectScriptedThingBase) ctor that retail expands at one site and calls (0x007E72A0) at the
  other is out of reach: implicit and row-local plain-inline ctors expand at both (and cost an ebp push / 4 frame bytes), OUTOFLINE_CTOR
  calls at both but restores the frame, ebx zero register and ebp shrink-wrap exactly (113 -> 13 lines).
- (0x00DDAE40) An expanded `CScriptThing::GetAngleXY()` feeding a teleport argument keeps its fstp right after the virtual call under
  every inline spelling; retail sinks it below the next GetPos null test. Same-instruction scheduling residue, park.
- (0x00D23DC0) `class CScriptThing;` must precede `template <> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(...)`
  (declared-only 0x00704580) or the row is SRC_FAIL; with it, a re-assigned function-scope CScriptThing matches.
- (all) q.py's work dir is per address: two grades of the same address at once corrupt each other's .obj (objdump CalledProcessError);
  set QTAG for parallel diagnostics of one address.
- (0x00DDAE40) The CEntityScriptBinding flags argument of a level-script binding was 0 here (`[edi+0x18]=0`, `[edi+0x14]=1` is
  ClassAllocValid): read it from the bytes, do not copy it from a sibling.

Review at landing:
- 0x00D3A4A0, 0x00DDAE40, 0x00EFA4F0 took more than 10 grades (parked).
- Catalog / label names under CQ_FireHeartScript::CFH_Prisoner (0x00D3A9B0, 0x00D3AB00, 0x00D3ABE0, 0x00D3ACE0) and reloc_bindings
  spellings of 0x0099F100 / 0x01375748 are wrong (NAME_FIXES.tsv).
- engine/CQ_PrisonRaceScript.h MakeRacerComment signature (header debt).
