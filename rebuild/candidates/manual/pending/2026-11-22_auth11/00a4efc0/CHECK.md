# CHECK 0x00A4EFC0: AllocateType's block local `overflow_check` (AUTH11, 2026-11-22; no grading)
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Ruling: last entry of
docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md ("admissible only if something besides the 261-B debug assert uses it,
and if Ego_r.pdb / FableWin.pdb show it").

## 1. Ego_d.pdb (egod_locals.py / egod_proc.py / egod_lines.py from pending/2026-11-22_codex_pass27/tools)
All 12 instantiations: `this` bprel -56, parameter `elements` (unsigned long, bprel +8), one S_BLOCK32 holding
`unsigned __int64 overflow_check` (bprel -20). Line table (file id 264), C3DVector / bool:

| line | C3DVector | bool | what it is (VC10 calibration below) |
|---|---|---|---|
| 21 | 37 | 37 | prolog (EH frame, assert has inline asm + eh) |
| 22 | 10 | 10 | `if (elements)`: cmp + je near over the block |
| 24 | 16 | 11 | `unsigned __int64 overflow_check = (unsigned __int64)elements * sizeof(T);` (mul for 12; none for 1) |
| 25 | 16 | 16 | `if (overflow_check > 0xffffffff)`: the 64-bit compare, near jumps over lines 27..28 |
| 27 | 261 | 261 | the debug ASSERT |
| 28 | 4 | 4 | `return NULL;` (xor + jmp short) |
| 31 | 17 | 14 | `return (T*)Allocate(elements * sizeof(T));` (re-multiplies, jmp short) |
| 33 | 2 | 2 | `return NULL;` |
| 34 | 16 | 16 | epilog |

Structural point: line 25 is a conditional that skips lines 27..28 (265 B) and falls to line 31; it is not the assert
(an assert on 25 would make 27/28 an unconditional `return NULL`). The only name in scope that it can test is `overflow_check`
(`elements` was tested on line 22).

VC10 SP1 calibration (odall10.py, fablelib preset; v10/at.cpp, out in v10/at.out), Ego_d shape with a filler assert:
`if (elements)` 10, local 16 / 11, `if (overflow_check > 0xffffffff)` 16, `return 0;` 4, Allocate return 17 / 14, `return 0;` 2:
every non-assert, non-frame line equals Ego_d. Control without the local (v10/at_nolocal.cpp): one test line of 32 / 27 B and
no separate local line, which Ego_d's two lines (16+16 / 11+16) contradict.
=> **`overflow_check` is used by the line-25 size test, besides the assert.**

## 2. FableWin.pdb (VC10 debug editor build; symbols dump S/codex27/pdbx/FableWin.symbols.txt)
All 12 `CSimpleMemoryPool::AllocateType<T>` S_GPROC32 records (CMatrix4x4, C4DVector, CShaderRenderManager::CLightInfo, bool,
C3DVector, char, CQuaternion, C3DAnimationSequenceData::C3DVectorWord, unsigned char, C3DAnimationSequence,
C3DAnimationSequenceData, C3DKeyframe): code size 371 (1-B T) / 379 (others) = Ego_d's, flags `has inline asm | has eh |
marked inline`, `this` bprel -56, `elements` bprel +8, S_BLOCK32 { `overflow_check` unsigned __int64 bprel -20 }, one
S_CALLSITEINFO `int ()` inside the block (the assert's call). **FableWin shows the local, same name, type and scope.**

## 3. Ego_r.pdb (VC10 release game build; S/codex27/pdbx/Ego_r.symbols.txt)
One out-of-line copy only: `CSimpleMemoryPool::AllocateType<C3DKeyframe>`, code size 72, flags `marked inline | has async eh`,
`this` in ECX, parameter `elements` (bprel +4). No S_BLOCK32 and no local: in the optimised build the 64-bit temporary lives in
registers (pdb_proc.py docstring: "A local missing from an Ego_r record was optimised away, not absent from the source").
**Ego_r confirms the parameter name `elements`; it neither shows nor contradicts the local.**
Neither PDB has S_INLINESITE records (VC10 emits none), so inline sites cannot be listed.

## Result
Line 25 is 16 B for both sizeof(T) = 1 and sizeof(T) = 12, while the product line (24) is 11 v 16: line 25 compares a stored
value, it does not recompute the product (the no-local control puts the multiply on the test line, 27 v 32).
The ruling's "Ego_r.pdb / FableWin.pdb show it" is met by FableWin only (all 12 records); Ego_r's release record has no locals
(optimised away per the pdb_proc.py rule), so it neither shows nor contradicts it. This reading is flagged for the coordinator.
The local is not assert-only (line 25 tests it, calibrated), and FableWin shows it. Per the brief this is the "otherwise"
branch: step 3 (pre-registered single grade of the row + one order readout + one grade per user with the same header shape).
