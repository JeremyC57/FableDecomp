# NF367 hand-back: 0x00749810's NEQ CTCNoiseDef copies and the ??0CTCNoise strong duplicate (Account A naming queue, 2026-11-24)

Folder `$S/w22/agentNF367/` (S = the 40effb6c scratchpad). Lock name NF367. **Nothing is ACCEPT; there is no land_scripted run dir.**
Both parts need a device, so both are handed back as rulings with measured option texts under `held/` (evidence only, never flip to a run dir).
No repo write, no git, no header edit, no test change. Every object was compiled by `tools/pass.py` at ONE path per row
(`work/one/<addr>/`, so ?A0x hashes agree), graded by verify_and_land.parity_of against the catalog oracle.

## Result
| part | address | verdict |
|---|---|---|
| 1 | 0x00749810 (copies of 0x0044C1BB `??0CTCNoiseDef`, 0x0044F71B `??_GCTCNoiseDef`) | skipped: no existing-opt-in / include fix; device form measured (ruling NF367-Q1) |
| 2 | 0x00749A70 (`??0CTCNoise` strong duplicate) | diagnosed; helper fix P2h passes every measured gate but is a device (ruling NF367-Q2) |

## Part 1: 0x00749810's two copies (evidence/cmp/copies.txt, setdiff/, comdats/, pk/gate_*)
HEAD (pk_all with `work/pk_addrs.tsv`, both checkers): `??0CTCNoiseDef` 29 B NEQ (owner / retail 18 B), `??_GCTCNoiseDef` 30 B NEQ (owner /
retail 34 B); every other homed copy EQ; no copy leaf-reached. Plain pk_all (no address file) shows `??_GCTCNoiseDef` as NOADDR, so the
address file is needed to see it at all.

| variant (00749810) | leaf | ??0CTCNoiseDef v owner | ??_GCTCNoiseDef v owner | other COMDATs |
|---|---|---|---|---|
| v1 + `_IMPLICIT_DTOR` | RM | 29 B DIFF (unchanged) | 36 B DIFF (/O2 `add esp,4` form) | + `??1CTCNoiseDef` 11 B |
| v2 - `CSUBDEFCLASSBASE_CTOR_INLINE` | **DIFFER 67 v 79** | 18 B EQUAL | 30 B DIFF | - |
| v3 v1 + v2 | DIFFER 67 v 79 | - | - | - |
| v4 device: size pragma around CTCNoiseDef.h (first include of the def bases) + IMPLICIT_DTOR | RM | 18 B DIFF (calls ??0CDefClassBase) | 34 B EQUAL | 6 EQ copies re-moded (confounded: the pragma also covered the bases' first inclusion) |
| v5 = v4 without IMPLICIT_DTOR | RM | 18 B DIFF | 28 B DIFF | same confound |
| **v6** device: `#include "engine/CSubDefClassBase.h"` above, `#pragma optimize("s", on)` / CTCNoiseDef.h / `#pragma optimize("", on)` + IMPLICIT_DTOR | RM | **18 B EQUAL** (sel 2, align 1, as owner) | **34 B EQUAL** (sel 2, align 1) | only + `??1CTCNoiseDef` 11 B (no retail home) |
| v7 = v6 without IMPLICIT_DTOR | RM | 18 B EQUAL | 28 B DIFF | none |
| v8 `DEF_CLASS_BASE_CTOR_DECL` for `_CTORS` | **DIFFER 67 v 79** (leaf calls ??0CDefClassBase; retail's calls ??0CResource) | 18 B DIFF | - | 4 EQ copies dropped |

- **No existing opt-in fixes either copy.** The only text that makes the `??0` copy owner-EQ without a device (v2) moves the leaf.
  No byte-neutral include change applies: including CSubDefClassBase.h before the CTOR_INLINE define is v2.
- **Causes.** `??0CTCNoiseDef`: the 29 B copy comes from `FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE`, which the leaf needs (retail's
  leaf expands the whole def chain down to the ??0CResource call). The owner row does not define it, so its 18 B copy calls the declared
  base ctor; v2 reproduces those bytes and relocations at /O2, so mode is not the cause of this one. `??_GCTCNoiseDef`: `_IMPLICIT_DTOR` is
  only half the story (premise correction for line 3616): the owner / retail copy is a SIZE-MODE body (the def TU's `optimize("s")`,
  align 1, `pop ecx`) while 0x00749810 is an /O2 TU (16-aligned retail address), so the implicit form there is the 36 B /O2 body.
  Corrected T3 texts: `T3_PROPOSED.tsv` (column `replaces`).
- **v6 is the minimal owner-EQ form** (pk gate_v6, both checkers): both copies turn EQ, leaf RELOCATION_MATCH and its section SAME,
  every other COMDAT SAME except the added retail-homeless `??1CTCNoiseDef` 11 B, reach flips 0. For `??0` the size pragma only
  suppresses the CSubDefClassBase expansion inside the copy; that is not how the owner got its body (a declared base ctor), so v6 is a
  device that reaches the owner's bytes by another route. It is a pragma device in a row (not retail: retail's CTCNoise TU most likely emitted the same
  /O2 copies and the linker discarded them because the def TU came first), so per the brief it is skipped with this measurement.
- **Ruling NF367-Q1:** accept a header size-body opt-in for CTCNoiseDef.h (the NF349 / NF352 / NF361 SIZE_BODY family: own macro,
  add-only, pragscan before == restored after, exactly the two COMDATs change and equal the owner, whole-catalog equiv_check + planted
  control), or leave both copies as debt for the object-order build mode (the race is link order: owners first = retail's result).
  If accepted, the block would sit after CTCNoiseDef.h's `#include "engine/CSubDefClassBase.h"` around the class (v6's scope) and the
  `??1CTCNoiseDef` 11 B COMDAT it adds (with `_IMPLICIT_DTOR`) is a retail-homeless pick-any copy, also emitted by HEAD 0x00749A70 (align 16).

## Part 2: the `??0CTCNoise` strong duplicate (evidence/link_*.txt, setdiff/00749a70_*, pk/gate_p2*, droptest_p2h.tsv)
- **Retail's body is 0x00749810's 79 B** (its row grades RELOCATION_MATCH at the catalog length; pk_all on 0x00749A70's object: the 59 B
  copy NEQ v 0x00749810). 0x00749A70's local model declares `CTCNoiseDef()` out of line, so its ctor calls `??0CTCNoiseDef` (59 B).
- **Why 0x00749A70 defines it:** its leaf is the implicit `~CTCNoise`; VC7.1 emits that only through the vftable's `??_G`, and the vftable
  only from a ctor definition. Measured:
  - p2a ctor declared only: no `??1CTCNoise` emitted at all (no leaf).
  - p2b / p2c / p2d user-defined `CTCNoise::~CTCNoise() {}` (shared CTCNoise.h or local model): DIFFER 39 v 33 / 33 v 33 / 39 v 33; all
    three store the CTCNoise vptr at entry (relocation to ??_7CTCNoise, checked with disfn; retail's stores none), p2c also calls ??1CTCNoiseDef.
- **Extra finding:** 0x00749A70's `??_7CTCNoise` copy is NEQ: slots 4 / 8 / 9 bind ?OnCreate@CTCBase (reloc_bindings 00686850),
  ?HasFrameUpdate@CTCBase and ?FrameUpdate@CTCBase (006868a0); retail 0x01268A74 holds CTCNoise::OnCreate 0x007498E0, HasFrameUpdate
  0x007499E0, FrameUpdate 0x00749860 (labels / landed selfs), which 0x00749810's copy binds. A pick-any data copy: first wins silently.
- **HEAD pair link** (both orders): 1 LNK2005 `??0CTCNoise` (different bodies).
- **Options measured:**
  | form | leaf | `??0CTCNoise` | other effects | pair link |
  |---|---|---|---|---|
  | p2e local CTCNoise model (implicit dtor, 5 overrides) on shared CTCNoiseDef.h + 0x00749810's opt-ins + IMPLICIT_DTOR | RM | 79 B identical to 0x00749810 (sel 1 both) | `??_7CTCNoise` now EQ; ADDS NEQ `??0CTCNoiseDef` 29 B and `??_GCTCNoiseDef` 36 B (a second emitter of the nf359 debt): fails "no new NEQ" | 1 LNK2005, identical bodies |
  | p2f = p2e + the v6 size device | RM | identical | every copy EQ | 1 LNK2005, identical bodies |
  | **P2h** local model, ctor declared only + one `?A0x` helper `noise.CTCNoise::~CTCNoise();` (no pragma) | RM, `??1` 33 B EQUAL HEAD (sel 2, align 16) | **not defined** | drops `??0CTCNoise` (NEQ), `??_7CTCNoise` (NEQ), `??_GCTCNoise` (EQ, kept by 0x00749810), `??0/??_7/??_G CIntelligentPointer<const CThing>` (EQ `??0` kept by 0x00749810 / landed 0x0048A400); adds the 35 B strong helper | **0 duplicates** both orders; with owners only the pre-existing Alloc pair |
- **P2h gates** (`held/p2_helper_unit/00749a70.cpp` is the clean text; compiled at the row path it is object-identical to the P2h probe,
  setdiff all SAME, checked again after its final comment edit):
  grade RELOCATION_MATCH unchanged; leaf section SAME; both checkers HEAD v new: no NEQ added or turned, the only NEQ (the 59 B ctor) goes;
  reach flips 0 (plain and address-file runs); drop test: no reloc_bindings line is witnessed by 0x00749A70, every dropped EQ copy keeps an EQ
  definer (0x00749810), dropped UNDs are the vftable's slots and the ctor's callees; link: unresolved set only shrinks (the three CTCBase
  slot names go), `??1CTCNoise` (UND in 0x00749810) still resolves. Helper name census: no landed source uses `ReferenceNoiseDestructor`.
  The landed test is standalone (own model), unaffected; no vland --recheck-landed run (no run dir).
- **Why still held:** the helper is the NF328 device (ruled per unit, scaffolding debt), and the COMDAT set changes (a copy repair, so the drop
  test applies, which passes). **Ruling NF367-Q2:** accept P2h for 0x00749A70 (then the coordinator builds the run dir from
  `held/p2_helper_unit/00749a70.cpp`; T3 lines 3620 + the new nf367 vftable line close), or prefer option A (p2e / p2f: identical strong
  duplicate, needs Q1's device too to avoid new NEQ copies), or option S (one source for several addresses, coordinator tooling).
- Not compiled (no lock taken): other CTCNoise emitters such as 0x00749A50 (the `??_G` row on a stand-in self) and 0x004D3658 (Alloc, witness
  of the `??0CTCNoise` table line); a landing of P2h should include them in its census link.

## Rule notes
- Locks: NF367 on 00749810, 00749a70, 0044c1bb, 0045c5f6 (tests prechecked before the claim, sources after it, every variant before its
  compile: evidence/pre_*.log, clang only, all clean). Owner locks released right after their HEAD build. Final state: evidence/locks_final.txt.
- No B claim, Codex reservation or other agent's row touched. No retail bytes in hand-back files (names, sizes, verdicts only).
- Disclosure: one unresolved-set comparison command wrote a scratch file `/tmp/x_<pid>` outside my folder (an aborted awk redirect;
  the real comparison wrote work/ur_*.tsv). It holds only symbol names.
- Prechecks: evidence/pre_*.log (pre.sh prints errors only, so the original per-variant logs were empty = pass);
  evidence/pre_summary_rerun.log reruns every variant and the candidate with an explicit PASS line.
- Tools (tools/): NF363's / NF359's helpers repointed to NF367, plus pkpair.sh (both checkers, plain + the address file tools/pk_addrs.tsv), links367.sh /
  links367b.sh (pair links), and pass.py variant mode (`addr@tag=src`).
