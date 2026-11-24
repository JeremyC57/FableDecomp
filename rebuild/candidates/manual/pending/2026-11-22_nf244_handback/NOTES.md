# NF244 notes (2026-11-22, round 15): the CEngine unit (bisect first), CModeBase follow-ups, the IsThingCarryingRangedWeapon binders

Base: the repo tree with nf242 landed. NF245 runs beside me (0x004046B0 list family, DTOR_OUTOFLINE families, NF241's CODEXP27 lines,
0x0049D800, 0x01296F14); none of my rows is on its lists or in `pending/2026-11-22_codex_pass28/RESERVED.tsv` (grep before claiming).
Locks: work/locks.log (0x00B260B0 and the ten test rows were `released`; 0x00B261D0 / 0x0058578B new directories).

## Entry 1 (q12680): the CEngine unit
### Step 1: the bisect of the four CIEngine.h opt-ins on 0x0058578B (HEAD header and source, the macros as /D flags; q.py)
All 16 subsets were compiled (work/bis*.txt): the effect is NOT one declaration and NOT additive.

| Macros defined (/D) | 0x0058578B |
|---|---|
| none (HEAD) | RELOCATION_MATCH |
| RENDER | RELOCATION_MATCH |
| SCREENPOS | DIFFER (the 0x00585D7A swap) |
| CONVERTSCREENPOS | RELOCATION_MATCH |
| CONVERTWORLDPOS | DIFFER (the same swap) |
| RENDER + SCREENPOS | RELOCATION_MATCH |
| RENDER + CONVERTSCREENPOS | DIFFER |
| RENDER + CONVERTWORLDPOS | RELOCATION_MATCH |
| SCREENPOS + CONVERTSCREENPOS | RELOCATION_MATCH |
| SCREENPOS + CONVERTWORLDPOS | DIFFER |
| CONVERTSCREENPOS + CONVERTWORLDPOS | RELOCATION_MATCH |
| RENDER + SCREENPOS + CONVERTSCREENPOS | DIFFER |
| RENDER + SCREENPOS + CONVERTWORLDPOS | RELOCATION_MATCH |
| RENDER + CONVERTSCREENPOS + CONVERTWORLDPOS | DIFFER |
| SCREENPOS + CONVERTSCREENPOS + CONVERTWORLDPOS | RELOCATION_MATCH |
| all four (NF242) | DIFFER |

Every DIFFER is the same single two-instruction swap at 0x00585D7A (checked on SCREENPOS / CONVERTWORLDPOS alone). Controls with the
HEAD header (work/probe/): a `#define` line, a `class CNF244ProbeA;` forward declaration, and an unrelated abstract struct with one pure
`bool (float, int&) const` virtual, each prepended to the source, all stay RELOCATION_MATCH; so it is not a line shift or a plain
"one more declaration" count. The row does not include engine/CEngine.h, so the /D flags change only CIEngine.h.
Two triples keep 0x0058578B: {SCREENPOS, CONVERTSCREENPOS, CONVERTWORLDPOS} and {RENDER, SCREENPOS, CONVERTWORLDPOS}. Chosen: the first
(retire slots 11 / 12 / 13, keep slot 8 on `Virtual08_00b27d90` + the RENDER opt-in): the three Convert* slots share their types, and
CEngine.h already gates its slot 12 / 13 overrides on those two macros. Decision 1 asks to confirm the choice (the other triple is equally
proven on 0x0058578B but not rechecked over the closure).

### Step 1 applied with {SCREENPOS, CONVERTSCREENPOS, CONVERTWORLDPOS} (hdr/engine/CIEngine.h; hdr/README.md has the full proof)
- Built from orig/ (the repo copy), not NF242's draft: slots 11 / 12 / 13 lose their `#ifdef` / `#else` placeholder lines; slot 8 keeps
  `Virtual08_00b27d90` + the RENDER opt-in. pin_asserts pinned 4; assert_names_check 0.
- 0x0058578B graded against the real overlay (not the /D proxy): RELOCATION_MATCH (work/g578_ovl.txt).
- equiv_check over the 159-row closure: 108 rows / 122 files differ (the same count as NF242's all-four header; non-additivity means the
  recheck was redone, not inherited). vland --recheck-landed over all 108 under the overlay: 101 RELOCATION_MATCH + 7 MATCH, 108 PASS
  (work/rc_all.log). The three RELOC_CONFLICT WARNs (0x0064BFBA, 0x0064E978, 0x006B42F0) are identical in a no-overlay HEAD control (work/hc.log).
- Ten tests respelled (rows/<addr>_test.cpp, slot 11-13 lines only; tools/tresp.py). NF242's q12678_step1/rows drafts are superseded (they
  also respelled slot 8).
- What stays: slot 8's `Virtual08_00b27d90` placeholder is the one remaining VirtualNN_ in CIEngine.h. Evidence that it moves 0x0058578B in
  combination: the table above (RENDER + CONVERTSCREENPOS, RENDER + SCREENPOS + CONVERTSCREENPOS, RENDER + CONVERTSCREENPOS +
  CONVERTWORLDPOS and all four DIFFER). Retiring it needs 0x0058578B re-tuned (authoring) or a ruling.

### Step 2: the CEngine.h override opt-in FABLE_RETAIL_CENGINE_OVERRIDES (hdr/engine/CEngine.h; add-only) -- done
- CEngine(), virtual ~CEngine() and the 78 overrides not declared by the default branch (slots 3-84), PDB spellings proven by compiling a
  derived-class probe and finding every one of its 84 CEngine symbols in two PDBs (hdr/README.md). The two device-reset overrides are
  private in both PDBs: the opt-in adds `private:` / `public:` around the existing lines (no existing line edited). The default branch's
  public spelling stays (NAME_FIXES: needs header claim; the self rows 0x00B26010 / 0x00B25D00 spell UAE / a fastcall stand-in).
- Equivalence: no landed file defines the macro; equiv_check with both headers = the CIEngine.h set exactly.
- "83 overrides" in the entry: 78 here + Initialise / Uninitialise / OnPreDeviceReset / OnPostDeviceReset (default) + the dtor.

### Step 3: both rows off their local models -- blocked (needs a header claim; not attempted)
- 0x00B260B0 (ctor; leaf `EngineNative`) and 0x00B261D0 (dtor; ??1CEngine@@UAE@XZ on a different local polymorphic chain) construct /
  destroy members that CEngine.h's retail branch keeps as opaque byte blocks: PreloadCameras (CArray<CCamera>), PreloadStats (the nested
  CEngine::CPreloadStats, FableWin ??0CPreloadStats@CEngine@@QAE@XZ; no header anywhere), VideoOptions (CEngineVideoOptions, header
  exists), FrameMemoryPool (CSimpleMemoryPool, FLAT_CTOR), ScratchZBuffersD24S8 / RenderLayers / EngineComponents (CArray<T*>). With
  opaque blocks the ctor cannot run their constructors in member order, nor the dtor their destructors, so a port needs a typed-member mode
  of CEngine.h (an opt-in that replaces seven existing member lines, plus a nested CPreloadStats) and then re-grading both bodies on it:
  a coordinated header item, and the bodies may need re-tuning (authoring). Decision 3.
- Both rows' locks are kept as NF244 until the coordinator rules (release them if the unit is re-queued elsewhere).

## Entry 2 (q12681): four self-leaf respells onto the PDB spellings -- done (4 rows, no binder moves)
| Address | HEAD self leaf | New self leaf (Ego_r / FableWin / Ego_d raw PDBs, grep -ca = 1 each) | Grade |
|---|---|---|---|
| 0x0073DA00 | ?CModeStandBase_IsAllowingPlayerSpecialMovementMoves@@YI_NXZ (free fastcall) | ?IsAllowingPlayerSpecialMovementMoves@CModeStandBase@@UBE_NXZ | MATCH |
| 0x00861900 | ?GetMovementSpeed@CModeCarryingStandBase@NCreatureMode@@UAEMJJJ@Z (local model) | ?GetMovementSpeed@CModeStandBase@@UBEMABVC3DVector@@0W4EMovementType@@@Z | RELOCATION_MATCH |
| 0x008D6850 | ?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@@QAEXH@Z | ?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXAAVCAnimateThing@@@Z | MATCH |
| 0x00743CB0 | ?Get@T@@QAE_NXZ (gapscan) | ?IsRemoveOnDeactivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBE_NXZ | MATCH |
- Second source for each: the body plus the retail vtable slot (NF242: slot 9 / 7 of the CModeStandBase-derived vtables; slots 2 / 11 of
  0x01268774), and every landed vtable emitter already binds the PDB spelling at that address after nf242 (NF242 NOTES census table).
- Census: no E8 / E9 caller of any of the four (callers.py, empty); no reloc_bindings line names any of them; a grep of rebuild/src,
  rebuild/tests and the table for the four old spellings finds only the rows' own HEAD source / test. Each address goes 2 -> 1 spelling.
- Definers of the four NEW spellings: none besides these rows (grep of rebuild/src/compiled for the qualified definitions; 0x0073D9A0
  defines CModeCarryingStandBase's own IsAllowing..., 0x00739E90 CModeBase's: other classes). No catalog owner for CClass::GetType.
- Link (work/lnk1.txt, tools/lk.sh): the four rows + 0x00743FE0 0x00743600 0x008E3A20 0x00862EC0 0x0073D9A0 0x008E3720 0x008E31A0
  0x008E37C0 0x00741390 0x00740420 0x008AC4D0 (15 objects), HEAD set and new set: 0 LNK2005 / 0 LNK4006 each.
- q12681 does not depend on the overlay headers (CModeStandBase.h / CTCCreatureModeManager.h are unchanged repo headers): it can land
  separately from q12680.
- Ports: 0x0073DA00 / 0x00861900 onto engine/CModeStandBase.h with the existing PASS22_VTABLE opt-in (+ CTCCreatureModeManager.h with
  CMODEBASE_VIRTUALS; 0x00861900 also engine/CThing.h for CClass::GetType, CThingManager.h THING_TYPE_IDS for THING_TYPE_PLAYER_CREATURE,
  EMovementType.h). 0x00861900's body reads the parent thing's CClass::Type (+0x10) against 2 = THING_TYPE_PLAYER_CREATURE (the HEAD
  comment said "AI creature"; CThingManager.h's enum says player creature).
- 0x008D6850 / 0x00743CB0: NCreatureMode::CModeIceTrollGroundSmashIcicleAssault has no header anywhere under rebuild/include. Both rows
  declare it as `class ... : public CModeStandBase` (shared header base) with only its two PDB overrides, the shape of the landed sibling
  0x00743FE0's local class (nf242 ruling 1). 0x008D6850 is an in-place respell (same class token, namespace + base added); 0x00743CB0
  replaces its stand-in `T` with the real class token (a new local class for that row, allowed by the three-condition rule: no header,
  landed sibling shape, shared base). Decision 4.
- Symbol diffs (HEAD v new objects at /O2, work/q81/symdiff.txt): each row only swaps its own DEF leaf; 0x00861900 also defines the
  pick-any inline ?GetType@CClass@@QBEKXZ (shared-header inline, 5 bytes `movzx eax,byte [ecx+10h] ; ret`, inlined in the body; no table
  line maps it, retail has it inline).
- Tests (rows/<addr>_test.cpp, NEW, all link the row object): each calls the row's member non-virtually on zeroed / patterned storage; the
  0x00861900 test has one case per path (walking, running, jogging, null for a non-player thing; walking / running for the player creature),
  the 0x008D6850 test checks the empty body leaves both buffers untouched. The HEAD tests were standalone host models (0x00861900's modelled
  CModeStandBase locally). PassPatterns unchanged.
- RENAMES section 1: 0x00861900 (module NCreatureMode::CModeCarryingStandBase -> CModeStandBase; catalog name propagated) and 0x00743CB0
  (module _gapscan, name vfunc_11 -> NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::IsRemoveOnDeactivate); files move with them.
  vland WARN STANDIN_LEAF on 0x00861900 is that catalog name.

## Entries 3-6 (q12682, q12679, q12676, q12659) -- not started (context; handed back after two entries as the brief allows).

## Rule slips
- One foreground `sleep 60` while waiting for the HEAD control run (single sleep, not a loop; later waits by PID).
- One `rm -rf` on variable paths was attempted for scratch cleanup and refused by the harness before it ran; the cleanup then used the
  guarded tools/clean.py.
- STATUS / APPLIED written per entry, not after every row.

## Self-check
- Repo root listed before (work/root_before.txt) and after (work/root_after.txt). No git run. No __asm / naked / _emit.
- Every hand-back row and test: recheck_final.log (one vland --recheck-landed run over the 4 rows, the 10 tests and 0x0058578B, overlay on).

## Decisions for the coordinator
1. q12680 step 1 landed with {SCREENPOS, CONVERTSCREENPOS, CONVERTWORLDPOS} (slot 8 keeps the placeholder). The other matching triple
   {RENDER, SCREENPOS, CONVERTWORLDPOS} (slot 12 kept) is proven only on 0x0058578B. Accept the chosen triple?
2. Slot 8 (`Virtual08_00b27d90`): leave as the last CIEngine placeholder, or queue 0x0058578B for re-tuning under all four declarations
   (authoring; the swap is at 0x00585D7A)?
3. q12680 step 3: CEngine.h needs a typed-member mode (seven opaque members typed, a nested CPreloadStats) before 0x00B260B0 / 0x00B261D0
   can leave their local models. Queue it as a header claim (and the ctor / dtor re-grade, possibly authoring)?
4. 0x00743CB0's new local declaration of the header-less NCreatureMode::CModeIceTrollGroundSmashIcicleAssault (replacing stand-in `T`) on
   the shared CModeStandBase base, mirroring 0x00743FE0 / 0x008D6850: accept, or add an engine/ header for the class first?
5. CEngine.h's default branch declares OnPreDeviceReset / OnPostDeviceReset public; both PDBs say private (EAE). The step-2 opt-in fixes
   it for its users only; the default and the two self rows (0x00B26010 UAE, 0x00B25D00 a fastcall stand-in) need a header claim (NAME_FIXES).
