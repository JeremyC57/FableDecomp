# 00b24850 (p25B, Account B, 2026-11-20): CEngine::SetDefaultRenderStates -- READY (two fitted devices, ruled)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00B24850 | CEngine::SetDefaultRenderStates() | 4344 | RELOCATION_MATCH (no flags) | PASS (7 targeted + 20,000 random runs) | 59 live, all killed; 10 device mutants reported separately |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, **no header overlay**): `RELOCATION_MATCH PASS`, with one warning:
`WARN RELOC_NAME: ?D3DTextureUVModeTable@CRenderStateManager@@0PAKA -> 01396dc8 labelled 'g_textureUVModeToD3D'`.
- The binding is right. The warning comes from a stale descriptive label; NAME_FIXES.tsv carries the fix (Ego_d.pdb carved public plus
  reloc_bindings.tsv at 0x009DA9F0; labels_codex_pass13 already has the PDB name).
- Headers: none changed. The CRenderStateManager.h EARLY_RETURN brace fix went in separately (main's fix commit). This row no longer uses
  EARLY_RETURN, so it does not depend on that fix.
- Purity: plain C++. No asm, no pragma, no flag.

## What the parked diagnosis missed
The draft was DIFFER(4344v4344), and its residue was not 3 instructions. Every one of the 38 inline SetRenderState expansions carries two
c2 tie orders, and the draft got both boundaries wrong. Per expansion, `b248sig.py` prints the load order (C = CurrentBookmarkMask
[+0x2814] first, B = BookmarkMask [+8] first) and the test operand order (s = `test esi,eax`, a = `test eax,esi`):

    retail  Cs x3, Ca x24, Cs x4 (from 27, 0x00B2543B), Bs x7 (from 31, 0x00B255F9)
    draft   a->s at 28, C->B at 31        Ego_d-shaped text   a->s at 28, no B

## Fitted devices (coordinator ruling 2026-11-20, under the owner's delegation)
Neither device emits a byte. Measured on this text; signatures in probe/, ablation/sigs.txt.

1. **Test order (a->s).** It moves only with one more dead, function-level, named scalar local: any type, any initialiser, any count from 1
   up (probe/matrices sigs_e e7, sigs_f f3, sigs_g).
   - The row uses `unsigned long version = GPDisplayManager->GetVertexShaderVersion();`. Retail calls 0x009BEDE0 at 0x00B258A7 and discards
     the result.
   - Without the local: DIFFER(4344v4344), 40 diff lines (ablation/ab_noversion, mutants/dead/rm_version_local).
   - The bytes prove one more named scalar. They do not prove its name, type or initialiser: `long version`, and
     `call; unsigned long version = 0;`, are both SAME_CODE.
   - Ego_d lists no such local. The name is descriptive.
   - c2 pool numbering: `version` is local-pool #5 (see the c2 probe below).
2. **Load order (C->B).** It moves only with the number of dead FABLE_RETAIL_ASSERT arms:
   - 0..4 arms: B never;
   - 5..13 arms: B from expansion 31 (matches, every k in the range: probe/window/sigs.txt);
   - 14..16 arms: B from expansion 7.
   - Arm position does not matter, and arm conditions are byte-neutral (SAME_CODE).
   - The row uses 5 arms, the bottom of the window: the four manager pointers it dereferences, plus `version >= 1100`. 1100 = vs_1_1, the
     threshold that the landed 0x00B3B030 / 0x00B33B50 test.
   - The count is fitted. The conditions are unconstrained by the bytes.

**What the arms change (c2 symbol-order kit probe, probe/c2symbols/: tools/il/symlog.py on the final text and on the text without the
five arms).**
- Without the arms the function allocates 7 fewer kind-7 symbols and 1 fewer kind-10 symbol, and 60 fewer kind-3 records.
- The local pool (kinds 4/5: locals and the inlined setters' this / state / value / mode / stage) gets the same 297 symbols in the same
  order either way, but from the 61st on (expansion 9's `_state`) every number is 64 higher with the arms (#480 -> #544 ... #1728 ->
  #1792). The pools draw fresh numbers 32 at a time from one shared counter, so the arms' kind-7 symbols make the local pool's later
  draws land two blocks further on.
- The kit's sort key holds the number as `num << 5` in a 16-bit hash, so a +64 shift moves which later expansions tie which way. That
  is the load order. It also explains why the count (not the position or the condition) matters, and why the window is wide.
- `version` is local-pool #5, ahead of uv_scale / uv_bias (#6, #7). It moves every later pool number by one, which is the test-order knob.

**Contradiction with the fable_assert.h note (for FINDINGS_LOG, header not edited).** The note, from 0x00BB3E80, says the arm count moves the
test/or-operand order, and that arms with no inline call in their condition leave the load order alone. In this row, arms with no inline
call move only the load order (through the +64 shift above), and the test order moves only with a dead named local (+1).

**Line 664 (the coordinator's blocking check 1).**
- Ego_d line 664 (34 bytes) is unidentified. It is not a store (14 bytes) and not call + compare + branch (18 bytes; Ego_d
  GetVertexBufferUsage line 769, egod_lines/).
- At /Od in VC7.1, this candidate's line is 13 bytes and each FABLE_RETAIL_ASSERT dead arm is 54 bytes (egod_lines/candidate_Od.cod).
- Ego_d and FableWin both have 847 bytes and no assert-sized line, so their debug revision of this function has no asserts.
- No-local forms graded on the Ego_d-shaped base with 5 arms (probe/noLocalForms) all stay at a->s 28, so none replaces `version`:
  - the call inside the assert's condition plus a bare call;
  - `if (call < 1100)` around a compiled-out macro (FABLE_RETAIL_ASSERT(false));
  - `if (call < 1100) {}`;
  - a bare call;
  - an empty inline function taking the version (its parameter does not count);
  - `if (call < 1100) DebugTakeText("..")`.
- /Od locals against Ego_d's:
  - ours: this, uv_scale, uv_bias, MAX_TEXTURE_STAGES, n, n, version;
  - Ego_d: this, MAX_TEXTURE_STAGES (const int), uv_bias, uv_scale (float), n, n (long, one per loop block).
  - The only surplus is `version`. Every other local has Ego_d's type and scope.
- FableWin callee list (pending/2026-11-19_fablewin_callees_for_B.tsv): 0x00B24850 / FableWin 0x02C9B640 is not in it, so no assert-handler
  count is available.

## Ladder and per-device ablation (ablation/, grades.txt; each removed from the final text)

| device | grade without it / with the change |
|---|---|
| `version` local (call kept, a 5th arm on MAX_TEXTURE_STAGES instead) | DIFFER(4344v4344), 40 lines |
| `version` local and its arm (4 arms) | DIFFER(4344v4344), 54 lines |
| all 5 arms | DIFFER(4344v4344), 48 lines |
| one arm (4 left) | DIFFER(4344v4344), 48 lines |
| 6 / 13 arms | RELOCATION_MATCH (window) |
| 14 arms | DIFFER(4344v4344), 40 lines |
| per-loop `long n` -> one shared `stage` | DIFFER(4344v4344), 40 lines |
| `const int MAX_TEXTURE_STAGES` -> literal 8 | DIFFER(4344v4344), 40 lines |
| uv_scale / uv_bias -> literal arguments | DIFFER(4344v4344), 40 lines |
| draft's FABLE_RETAIL_CRENDERSTATEMANAGER_DEFAULT_TRANSFORM | no byte change: dropped |
| draft's FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN | no byte change: dropped |
| FABLE_RETAIL_CENGINE / _CENGINELIGHTINGMANAGER / _CDISPLAYMANAGER / _SCOPED_PTR | each SRC_FAIL (header layout selectors, not devices) |

The ablation sources were cut while the text still had DEFAULT_TRANSFORM. That opt-in is proven byte-neutral (ab_nodefxform = the final
text = RELOCATION_MATCH).

## Source shape
Ego_d.pdb's line table (lines 603..674, egod_lines/) and locals:
- function scope `const int MAX_TEXTURE_STAGES = 8` (type 0x1C54), `float uv_scale`, `float uv_bias`;
- `long n` in each loop block.

The uv locals are declared at the top and assigned at Ego_d lines 666 / 667, just before the 39-byte call at 668 (four float locals pushed).
The second loop's SetTextureTransformMode takes the constant 0, as retail does: Ego_d line 640 is 15 bytes against 17 for the n-indexed
setters. Callee and data names come from Ego_r / FableWin (egor_obj_functions.tsv, fablewin_pdb_names.tsv) and the landed bindings.

## Test (test/00b24850.cpp)
Links the real row object. The row body, all 38 expanded SetRenderState / AddStateToUpdateList bodies and the inline SetTextureEnable store
run as compiled. The setup:
- The render manager is a raw 0x4000-byte buffer at the header offsets.
- The twelve D3D*Table arrays are test data with a distinct value per (table, index).
- The engine is a raw buffer: EnableWireFrame +0x10, EnableTextures +0x13, AnisotropicFilteringLevel +0x158.
- The out-of-line callees are __fastcall stubs that log `this` and their arguments.

Model(): written from the source statements and the header's SetRenderState text. The row's buffer must equal the model's byte for byte:
- every DesiredState, BookmarkMask and DirtyListFlag;
- the state-stack and update-list entries;
- both sizes and TexturesEnabled.

The call log must match: order, `this` and arguments, including the UV constants. The engine object must not be written.

Modes:
- everything differs;
- everything already desired (the early-out path);
- a random mixture of desired / differing / bookmarked / dirty states, non-empty lists, a random current bookmark bit and random
  wireframe / textures / anisotropy.

Spot checks independent of the model: stage 7's alpha arg 2, stage 0's alpha op set twice, the wireframe override, an untouched stage-1
transform mode, and others.

## Mutants (mutants/)
- **Live:** 59 one-decision mutants (m248.tsv, m248.log), all killed by the test. They cover every setter's enum / bool / constant, both
  loop bounds, the stage index, the SetTextureTransformMode stage, the deleted fill-mode / texture-op statements, the UV argument order, the
  wireframe test, every out-of-line call and its argument.
- **Devices** (m248dead.tsv / .log, dead/grades.txt). The behaviour test cannot see these; the byte grade can:
  - arm-condition edits (`!=` -> `==`, `>=` -> `<`, 1100 -> 2000, pointer swap): SAME_CODE (byte-neutral);
  - an added arm: SAME_CODE (6 arms, inside the window);
  - `long version`, and `call; unsigned long version = 0;`: SAME_CODE;
  - removing the GPRenderManager arm or the version arm: survives the test, grade DIFFER(4344v4344), 48 lines;
  - removing the local: survives the test, grade DIFFER(4344v4344), 40 lines.

## Header bug found while drafting (fixed by main separately)
engine/CRenderStateManager.h: 524a0abf9 cut the EARLY_RETURN branch's closing braces and `#else`, so c1xx hung on any TU defining
FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN. Main landed the fix on FullDecomp-B. My copy is retired (hdr.retired_20261120/).

## Entry for docs/journal/FINDINGS_LOG.md
2026-11-20 (p25B, 0x00B24850) CEngine::SetDefaultRenderStates.
- The 38 inline SetRenderState expansions carry two independent c2 tie orders:
  - the bookmark test / or operand order follows the local-pool symbol numbering. One dead named scalar local (`version`, number #5)
    shifts every later inlined setter parameter by one;
  - the mask load order follows the count of dead FABLE_RETAIL_ASSERT arms (window 5..13 here).
- Mechanism (c2 symbol-order kit, symlog): the dead local is local-pool #5, so later pool numbers move by 1. The arms add kind-7 / kind-10
  symbols that draw from the shared 32-number counter, so the local pool's later numbers move by 64 (two blocks). Both change the kit's
  `num << 5` sort keys of the later expansions.
- In this row, arms with no inline call move only the load order, the opposite of the fable_assert.h note from 0x00BB3E80.
- Both devices are fitted and labelled `// Inferred:` (dead local: name / type / initialiser unconstrained; arm count fitted, conditions
  unconstrained).
