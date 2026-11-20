# b9 (hand3c3, Account B, 2026-11-20): NUISystem::CSprite::Draw -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x0041AFA0 | NUISystem::CSprite::Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*) | 1455 | RELOCATION_MATCH | PASS (60,000 cases) | 206 / 207 (1 not observable from this row) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1): `RELOCATION_MATCH PASS`; two warnings, both about the bank pointer's destructor
0x004194B5 (RELOC_CONFLICT with 0x00419108 / RELOC_NAME; see NAME_FIXES.tsv -- the landed same-TU SetGraphicIndex 0x0041B550 makes
the same call). Purity: retail_asm through the shared engine/GFFloatToLongNear.h only (`// Retail-asm:` line with the four VAs; no
asm in the row). Flags: none. Grades: 3 (old local-model baseline 1014v1455; the port; the commented final). No local class model:
the row is the NUISystem::CSprite of engine/NUISystem_CSprite.h (Draw was already declared there).

## What the parked diagnosis missed
The parked draft called this a lane gap (bare fistp + a 400-byte "texture refresh" modelled as two calls, 1014v1455). Both are
ordinary: the snaps are GFFloatToLongNear, and the 400 bytes are the inline CGraphicInfoFrame::GetFrame chain that the landed
SetGraphicIndex 0x0041B550 (same TU, shared headers) already spells as `pinfo->BeginLoading(0, 0x10); CGraphicFrame frame;
pinfo->GetFrame(frame, 0, 0x11);`.

## Source shape
Ego_d.pdb parameters / locals and line table (lines 94..226; egod_lines/), one statement per entry. The /Od line sizes fix the
statement forms: two 84-byte lines = `size.X = (Width == 0) ? Size.X : (float)Width;` (no zoom multiply in them); `range` is a
C2DExtentsF (type resolved from the Ego_d TPI), so the clip box is the existing `C2DBoxF(const C2DVector&, const C2DExtentsF&)`;
the centre move is the block-local `C2DVector move` handed to C2DBoxF::Move (12-byte line); four 27-byte snap lines; two 25-byte
virtual-call lines; a 136-byte layer line = a conditional over two whole `CManager::GetInstance()->GetLayer(..)` calls.
- `// Inferred:` order position / range / clip_box / colour (the Ego_d sizes 21 / 70 / 16 / 21 fit it; the two other orders tried
  differ: 55 instructions off, and 1459 bytes).
- `// Inferred:` Handle.Clear() is expanded in place in retail; VC7.1 calls the plain inline (1404 bytes) and there is no /d2inlT
  window (T75 nothing; T80 / T88 expand the bank pointer's destructor that retail calls, 1438; T100 both, 1489), so the row sets
  the existing FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR.
- The two uv arguments are spelled `C2DVector(0.0f, 0.0f)` (the default arguments of the Ego_r declaration; retail builds them
  in the dead `position` / `range` slots).

## Same-TU COMDATs (`// Fidelity:`)
The row object carries both constructors it calls; `tools/gother.py` grades them from this file: 0x0041BF60 (texture, 167 bytes)
MATCH and 0x0041BEB0 (bank graphic, 171 bytes) MATCH.

## Header overlay (hdr/, hdr.diff; strictly additive: 0 removed lines; copied from today's rebuild/include)
- engine/C2DBoxF.h: `FABLE_RETAIL_C2DBOXF_MOVE` -- ?Move@C2DBoxF@@QAEXABVC2DVector@@@Z as an inline header function (declaration in
  the class, body after it = the landed 0x0041A810 text; Ego_d parameter `pos`).
- engine/CEnginePrimitive2DSprite.h: `FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_BOX_CTOR` -- the box / CTexture& constructor
  (FableWin name in the comment), declaration and inline body; and `FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER`,
  an `#ifndef` put round the ENUM_CTORS definition of EEngine2DLayer for TUs that also include engine/NUISystem_CManager.h (which
  defines the same enum unconditionally; without it the two headers cannot be combined with ENUM_CTORS).
No new header file: no landed local-model row is newly flagged.

## Test (test/0041afa0.cpp)
Links the real row object. No class at all: the row is entered and its ten out-of-line callees are bound through `/alternatename`
(thiscall = fastcall with a spare edx), operator new / delete are the test's own, virtual callees are slots of hand-built vtables
(sprite 101 / 104 / 106, engine 23 / 28 / 30, manager 36, the handle's primitive 1 / 3). Two object sets (sprite, manager, bank,
graphics, frame, frame data, resources, lists, count blocks) are built from the same random numbers: the row runs on one, an
independent model (double arithmetic, own round-to-even) on the other; the event lists (callee, `this`, arguments, the whole
0x54-byte primitive with padding masked) and every object dword are compared, pointers as offsets.
60,000 seeded cases; counters of the run: clear child 6252, clear own 4125 (notify 981, destroy 981), armed 14404, texture 16554,
graphic 26414, no graphic 6655, centred 25622, child 18844, own 30779; touch texture 4624, touch data 6076, reload 7660 (pending
3800), new data 5296, no frame 5349, forced fail 3492, forced null 6709, frame deleted 4418 (texture 1264, uninitialise 2190),
bank deleted 4147. The test fails if any of the 25 path counters is zero.

## Mutants (mutants/)
207 one-decision mutants (196 of the row text, 11 of the two new header bodies): 206 KILLED. The survivor is a header mutant,
`UVOffsetsStart(uv_end)` in the texture constructor: this row passes two zero vectors, so the swap cannot show here (the
constructor itself is graded MATCH against 0x0041BF60).

## NAME_FIXES.tsv
- 0x004194B5: the bank pointer's destructor as this TU calls it (folded 5-byte `jmp 0x00419108`); label is a TailJmp placeholder.
  Recorded so that the two vland warnings are expected; nothing to respell in landed callers.

## For the coordinator
- The landed same-class rows (DrawWithRenderManager 0x0041A980, GetSize 0x0041A850, Initialise 0x0041A8D0, the ctors) and
  CMovie::Draw 0x00547F60 / C2DBoxF::Move 0x0041A810 are local-model texts; with FABLE_RETAIL_C2DBOXF_MOVE and the texture
  constructor opt-in they can be re-ported on shared headers (backlog, not touched here).
