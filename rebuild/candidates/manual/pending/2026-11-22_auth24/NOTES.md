# 0x00839420 NThingHelpers::CreateEffectOnHit -- NOTES (AUTH24, 2026-11-22; continues AUTH23's p2)
**Grade:** RELOCATION_MATCH at stock flags, with a ruling needed on two open Ego_d deviations (lines 207 / 208) and one Inferred
device (the dead-arm assert), as 0x0066B370 was put to the owner; (`00839420.cpp`), behaviour test PASS (`00839420_test.cpp`), vland dry-run with strict headers
WINS 1 (`land/vland.log`). Overlay `hdr/` (two add-only opt-ins), `hdr.diff`, proofs in `hdr/README.md`.
Grades: AUTH23 3 (r0, p1, p2), AUTH24 5 (p4, p6b, final, final 2, final 3 = comment-only regrade). LOG.md has every pre-registration.

## Route (each step measured; LOG.md)
1. p3 = p2 ported onto the shared headers: byte-neutral (ours v ours, only relocation spellings changed).
2. p4 = p3 + the visible GetSubDef body (`FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY`, `..._SETDYNAMICCAST_REREAD`, CCreatureDef's inline
   StatGetTypeName): pcreature_def's release becomes `dec [ecx+4]` in place (0x0083968F) and the pmaterial_def / pcreature_def frame words
   swap to retail's. DIFFER(1308v1308), 6 masked lines: the squared-magnitude product order at 0x00839736 (ours Z, Y, X; retail X, Z, Y).
3. The product order is c2's commutative-operand sort (GOTCHAS 466). Read with the c2 probes (`../c2w/`, the 2026-11-20_c2_symbol_order
   kit; `fm2.py`, `symlog.py` + `lpool.py`): key = (3 * (symbol number << 5) + 0x30) & 0xFFFF, descending. The field symbols of the
   `point_of_hit - lod_centre` temporary were X #534 (made by the inliner with the C3DVector constructor), Y #679, Z #681 (made by the tree
   optimiser, in IL order, after every other vector's fields): keys c870 / fed0 / ff90 -> Z, Y, X. No existing GetSquaredMagnitude / operator- /
   constructor opt-in moves it (mini TUs: the order follows the numbers; one experimental per-product helper spelling did move it, to Y, X, Z). Retail's order needs Y / Z at least four
   numbers later, where the 16-bit key wraps (#683 -> 0050).
4. Ego_d line 129 is 13 B (ours 6) and its `if` at line 125 is 17 B (ours 13): `GFDebugPDynamicCast(pthing_hitting, (const
   CThingCreatureBase*)0)` measures exactly 13 / 17 (VC10 /Od). The debug cast carries one ASSERT (GFDebugPDynamicCast.h, wfB3); with its
   release dead arm (`FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT` + `FABLE_RETAIL_ASSERT_DEAD_ARM`, both existing opt-ins) the numbers become
   X #535, Y #712, Z #714 -> keys c8d0 / 0b30 / 0bf0 -> X, Z, Y. The cast alone (p6a) moves nothing. p6b: RELOCATION_MATCH.
5. Line 87: the draft's separate `pthing_hitting = 0;` was never needed. Ego_d's one statement builds the identical leaf (lf.py diff
   empty), zero store at 0x00839430 included. Not its cause: tcd_particle_emitter's initialiser (mini/tcd0.cpp: only 0x008398C2 goes),
   the assert opt-ins (mini/noarm.cpp: still there). Cause not identified; the text is Ego_d's.

## Ego_d conformance (VC10 /Od line sizes of the final text, od/fin_nothrow_od10.txt, against egod_lines.txt)
Every line 86..214 has Ego_d's size except:
- 151 / 166: Ego_d 77 / 52, ours 97 / 72 with today's CDefPointer.h; exactly 77 / 52 when `IsNull()` / `Get()` are declared `throw()`
  (od/hdrn, evidence only: the 20 B are the EH state stores and the temporary's saved address). **Needs header claim** (CDefPointer.h);
  no byte of this row depends on it (retail's function has no EH frame).
- 207: Ego_d 481 B, ours 15 (the GetTCDriver call). 466 B more is the size of one expanded debug ASSERT, so Ego_d's line holds the call
  and a debug check; retail still makes the call (0x008398C6), so the release statement is the call. Its debug text is not recovered.
- 208: Ego_d 40 B, ours 42. `NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "DIRECTION")` (FableWin.pdb, shared header) measures 40.
  c2 does not expand that helper here (inline probe: sz 25, c1 14 = one over the always-inline bound 13; scored 182 at f 0.012 against
  T 74), which leaves a call retail does not have, so the row spells the helper's body `CCRC::Calc(0, "DIRECTION", 9)` as the landed
  siblings 0x008F7950 / 0x007F2DC0 do. Retail has no out-of-line copy of that helper anywhere (no CCRC::Calc caller with a strlen loop
  in front of it), so the shared helper's cost is probably one too high: **needs header claim**, not this row's to change.

## Pick-any (pk.py on final.obj, final.pk.full.txt; leaf relocations: first_witnesses.tsv)
11 owned copies EQ: GetSubDef<CCreatureDef> 162 B EQ 0x00485815 (device below; its literal `??_C@_0N@IFBGKOGI@CCreatureDef` is
0x01231D3C = "CCreatureDef"), CArray<pair<unsigned long, C3DVector> >::push_back 70 B EQ 0x0076B120, IS_THING_ALIVE x2, HasTC,
bitset::test, GetTCDriver<CTCDParticleEmitter>, GetPRegionDef, GetString, operator CCharString, placement new.
1 NEQ: `vector<pair<unsigned long, C3DVector> >::_M_insert_overflow` 222 v 0x0076ADA0. Not leaf-bound (push_back calls it), bound at
HEAD, and the HEAD object of the landed 0x0076B120 defines the same differing copy (mini/head_0076b120.cpp): existing debt, queued.
The landed sibling's `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE` is NOT used: it makes CArray::push_back a 5-byte forwarder (v 70).

## Header changes (overlay hdr/, both add-only, off by default)
- engine/CMaterialDef.h `FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS`: the three declarations in their PDB spellings.
- engine/CDefinitionManager.h `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY`: the 0x0066B370 device for T = CCreatureDef
  (scaffolding). Conditions: (1) with the macro removed the leaf is identical (p7 v p8, lf.py) and only that COMDAT changes (182 -> 162;
  sorted pk lists otherwise equal); (2) the owner row 0x00485815 sets `optimize("y", off)` / `("s", on)` itself; (3) the block ends with
  `optimize("", on)` and says so; this row has no optimize pragma.

## Names (two sources each)
- Function, parameters, locals: Ego_d.pdb S_BPREL32 / block records (egod_locals.txt), Ego_r.pdb + FableWin.pdb symbol names.
- CMaterialDef methods: FableWin.pdb mangled names (ghidra_out/_fablewin_api.tsv) + Ego_r.pdb proc records (codex27 Ego_r.symbols.txt
  S_GPROC32 `CMaterialDef::GetBlendedSwordHitEffect`; Ego_r name set 00913d40 / 00913e70 for the two GetRandom*).
- GFDebugPDynamicCast: FableWin.pdb COMDATs (GFDebugPDynamicCast.h) + Ego_d line 129's size. GetPRegionDef: FableWin.pdb + Ego_d type record.
- No descriptive name in the row. `(ETCInterfaceType)132` is now TCI_BERSERK (engine/ETCInterfaceType.h); `GetTypeFlags() & 8` stays a
  literal as in the landed rows (no named enumerator in the shared headers). NAME_FIXES.tsv: the two landed CMaterialDef spellings; the
  -1.0f literal at 0x0122DEE0 is a split to queue (two spellings for one address), not a proven fix.

## Margin and recheck triggers (the match depends on symbol numbers)
With the dead arm: X #535 (key c8d0), Y #712 (0b30), Z #714 (0bf0). Retail's order holds while Y and Z stay at or above #683 (the key
wrap) and below about #1218 (where Z's key would pass X's), and X stays where it is: Y has 29 numbers of room downwards. Without the arm
Y / Z are #679 / #681, four short. Any change to the inline helpers this row expands can move them: recheck this row when
GFDebugPDynamicCast.h, fable_assert.h, C3DVector.h, GFWorldDistance.h, CDefPointer.h, CDefinitionManager.h (GetSubDef body), CThing.h,
CIntelligentPointer.h, CHitParameters.h, ThingAlive.h, CRegion.h, CMainGameComponent.h, CCreatureDef.h or stlport/fable_containers.h
(bitset::test under HasTC) change.
## Caveat for landing
vland with LANDVERIFY_STRICT_RELOCS=1 fails (WINS 0, land/vland_strict_relocs.log) on one RELOC_NAME warning only: 0x0129BA3C is labelled
?g_NormaliseEpsilon@@3MB while the row (and reloc_bindings) name ?DELTA_NOTIONAL_ZERO@@3MB. With strict headers only: WINS 1.
pk.py NO-OWNER check: none of the NO-OWNER symbols has a reloc_bindings target (looked up by name), so no copy went unchecked.
