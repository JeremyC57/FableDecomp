# acctB hand-back, chunk 2044 (1 row matched, 1 parked)

Authored by an Opus 5.5 agent; a short follow-up agent completed the asm evidence table. Re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00A9F540 `?CalcMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@PBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z` | RELOCATION_MATCH 2288v2288, test PASS (g++ and VC7.1); grade `retail_asm` | 4 grades (matched at the first full grade, from the source built for its twin). True length 2288 (estimate 2884) |
| 0x00AA0090 C3DAnimationManager::CalcBlendedMeshTransformations | **parked**, DIFFER 2390v2390, 604 of 622 instructions exact, all frame offsets equal; test PASS | `rows/parked/00aa0090.cpp` + `.test.cpp`; 10 grades. Goes to the acctB near-miss lane |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no warnings); the parked source regrades DIFFER(2390v2390). The landed 0x00A52650
ConvertTimeToFrames still grades MATCH under `hdr/`. Purity and padded-struct greps are clean (I reworded one comment
word that tripped the textual grep and regraded both sources).

## Review items for landing (matched row)

1. **`retail_asm`, with the full per-site table.** The row reaches bare `fistp dword` only through the shared headers:
   six `GFFloatToLongNear` sites inside the inline GFFastArcCos / GFFastSin (0x00A9F7CF / 0x00A9F83D / 0x00A9F8A7 and
   0x00A9FAF0 / 0x00A9FB5E / 0x00A9FBC8: the same expression on the same inputs, shown against retail's constants) and
   `GFFloatToLongFloor` in the TU's out-of-line ConvertTimeToFrames copy (0x00A52677). ASM_RULES rule 4 table:
   **38 plain-C++ + 15 intrinsic spellings for the Near sites, 32 + 14 for the Floor site** (12 and 11 of the intrinsic
   entries are compiler intrinsics; the rest are `_controlfp` / `_control87` and the MXCSR macro), each compiled as a
   fragment on the site's own input under the row's flags and under /QIfist, /arch:SSE2, /arch:SSE, /Op, with what
   VC7.1 emitted: `ASM_ATTEMPTS_TABLE.md` (and `NOTES.md` "C++ attempts before helper"; summary in the row's
   `// Retail-asm:` block). No spelling gives a bare `fistp dword`: casts give `call __ftol2`, /QIfist gives
   `fistp qword`, intrinsics give SSE conversions, /arch:SSE2 double casts give an `fnstcw ... fldcw` wrapped truncation.
   The landed 0x00A52650 row has no such table of its own; this one covers its site.
2. **`FABLE_FORCEINLINE` on four file-static helpers** (`ApplyRelativeAnimation`, `ApplyRelativeAnimationAlpha`,
   `ApplyPartialAnimation`, `ApplyPartialAnimationAlpha`) and on the keyframe / quaternion bodies written in the row
   (`SetSlerpFromIdentity`, `PostApplyRelativeAnimationAlpha`, `BlendWith` with landed 0x00AA3C50's text, GFFastArcCos /
   GFFastSin), with an `// Inline-budget:` line: FableWin's COMDAT run after the function names these undecorated
   file-static helpers; retail keeps uncalled LTCG copies of them (0x00A9D710, 0x00A9D980, 0x00A9D9E0: custom calling
   conventions, the .obj lander cannot grade them) but expands them in both rows.
3. **Headers** (`hdr/README.md`; all new opt-ins): `C3DAnimationManager.h` MESHTRANSFORMS, `C3DAnimationBlendState.h`
   SHAREDDATA, `C3DAnimationStateAlpha.h`, `C3DAnimationInfo.h` GETANIMATION, `C3DAnimation2.h` MANAGER and
   CONVERTTIME_BODY (the ConvertTimeToFrames body in the class: needed, with only a declaration the row is 2249 bytes;
   the landed 0x00A52650 defines the same body out of line -- regrade it if the opt-in is folded in),
   `C3DAnimationStats.h` ISRELATIVE, `C3DAnimationSequence.h` FRAMEKEYFRAME, `C3DAnimationSequenceData.h` ISIDENTITY,
   `C3DMesh2.h` BONECOUNT, `CSimpleMemoryPool.h` FREEALL, `CQuaternion.h` SLERPFROMIDENTITY, `C3DKeyframe.h` BLEND.
4. **Name fixes** (`NAME_FIXES.tsv`): 0x00AA0090 is bound by landed 0x00AA09F0 with pointer / non-const parameters (the
   PDB has const references); 0x00AA09F0's own Leaf has the same wrong parameter kinds; 0x00A4C1F0
   GetKeyframeInterpolate is `QBEXAAVC3DKeyframe@@KM@Z` (bound as `PAV...JM`). `LABELS.tsv` adds 0x00A9D700
   (GetAlphaWeight COMDAT) and the three LTCG helper copies; 0x00A9D730 / 0x00A9D740 are catalogued "SetMember*" but look
   like Initialise / Uninitialise (not graded). Both table globals are bound under two names each (g_ArcCosTable /
   g_ACosTable, g_CosTable / g_cosTable): the row uses the majority spelling, please pick one.

## The parked row

Whole residue: the product order in q.Y, q.Z, q.W inside the expanded BlendWith (partial-alpha loop) at 0x00AA07E8,
0x00AA0808, 0x00AA0828 -- retail does `fld [esp+10h] ; fmul [esi-14h|10h|0Ch]` (scale0 * Rotation) first, ours the
scale1 * other term. It follows the amount of IL in this one function (the r3010 mechanism): a diagnostic with 20 dead
float locals grades RELOCATION_MATCH (never deliverable); about 20 natural code-neutral spellings do not move it; an
LTCG build gives the same orders. `fabsf` in the full-weight test fixed the dot-product term order on the way.
