# NF37 round 3: notes (slice land1011fj..land1113fz, 340 entries, continuing NF35)

## Method
- **Probe.** `callers.py` over all 326 slice addresses gave 20,152 retail call sites in 2,308 landed rows (`work/calls.tsv`).
  - NF35's `probe.tsv` was reused for every row the round-2 landings did not touch.
  - Rows changed by the NF35 and NF36 landings (their STATUS.tsv) and the unprobed small targets were probed again: 375 pairs, private
    `pS/`, `work/probe2.tsv`.
  - Hot targets (more than 20 unprobed caller rows) were not probed. Their binders came from `reloc_bindings.tsv` spellings plus a source grep
    of the caller rows, and each changed row was graded.
  - Data globals and deleters were probed with `absrefs.py` (DIR32 references).
- **Worklist.** `work/wrong.tsv` lists the wrong binders per entry, and `work/kind.tsv` splits them into signature-only and different-name.
- **Ports first.** Most of the work moved stand-in self rows onto their shared header's existing opt-in, cheapest first (2-10 line rows).
  - A row's include chain is copied from the smallest landed row that defines the same opt-in (`bin/pre.sh`, `bin/port.sh`).
- **Grades.** Every grade was a private `q.py` under `timeout 240`, with a `cpp` #error precheck (`bin/g.sh`).
- **Recheck.** Every changed row is in one vland `--recheck-landed` run (`recheck_final.log`): 83 rows, 23 MATCH and 60 RELOCATION_MATCH,
  every test PASS.

## Statuses (APPLIED.tsv)
**done 45, partial 12, blocked 5, deferred 1, rejected 0, carry 277.**

### done (45)
- **Data globals:** q34-q38 (g_MaxAntialiasing / g_MaxResolutionWidth / g_MaxRefreshRate / g_MemoryA / g_MemoryB, all `long`); q48
  (CAFile::m_errorCallback through `CAFile::DoErrorCallback`, header opt-in); q138 (`__real@41000000`, written as the literal 8.0f).
- **Self ports onto existing header opt-ins** (each a section-1 rename):
  - q20 / q92 DetachZBuffer
  - q27 IsCurrentTrapToCauseKnockdown
  - q59 IsDebugProfile
  - q100 IsInMelee
  - q120 GetKeyframeInterpolate
  - q132 GetLevelBeingLoaded
  - q133 IsSerialisingGameState
  - q141 WaitWhileGPUIsBusy
  - q161 / q162 GetMinDistFromStall / GetMaxDistFromCustomer
  - q18 / q54 SetAsPerformedEvent
  - q188 GetMainPlayerJoystickDeviceNumber
  - q201 PeekInterpolatedParams
  - q209 GetSilhouetteColour
  - q246 CalcBlendedMeshTransformations
  - q289 SetupShadowHWAsDepthBuffer
  - q320 GetAbilityButtonScale
  - q330 SetupDepthBufferTransform
  - q331 / q332 Start/EndOcclusionTest
  - q15 / q19 / q53 GetControlMovementFromGameActionEvent
  - q26 CTCEnemy::IsAlliedWithFaction
- **CCountedPointer instances:**
  - q130 / q287: the placeholder pointee replaced by the proven class.
  - q285: the CAction dtor thunk, named by its jump target and its 12 callers.
  - q210 / q304: DeleteData template instantiations.
- **Binder-only:**
  - q121, q144, q245
  - q76: the binder 0x008EF910 moved onto engine/CTCTavernGameCoinBase.h and engine/Coin.h, and its own bool spelling became a section-1
    rename.
  - q84 (3 binders, NCreatureAI::IsAwareOfThingIndirectly)
  - q149
  - q25 (member PFaction)

### partial (12)
Each note gives the exact rows left.
- **q16 / q52 IsEventGameAction:** 12 binders fixed. Left: 0x00687FD0 and 0x006894C0 (base-less process stand-ins with their own vtables),
  0x0068B7C0 (local CInputProcessGameBase kept for its slot-9 virtual; 1 grade tried) and 0x00691AE0 (PhotoInput namespaced stand-in).
- **q74:** 0x0068E450 has a virtual Update over a local base.
- **q24:** 0x008DF3A0 and 0x00972490 model CCharString locally.
- **q140:** 9 CTexturePool stand-in binders.
- **q215:** 0x0096C4F0.
- **q321:** 0x00583BD5 and 0x0068FF50.
- **q67:** 0x009BF350.
- **q73:** 0x00B620A0.
- **q171:** 0x00857B70.
- **q262:** 0x0042DC94.
- **q5:** 0x0069F0F0.

### blocked (5)
- **q8, authoring:** the binder is a compiler-generated scalar deleting dtor for a class with no virtual dtor in the header.
- **q3, port (2 grades):** 0x00704A80 has a local non-template CCountedPointer.
- **q206 / q207, port (2 grades each, 3 rows):** the binders model CCharString and FileAccessType locally, which clashes with
  engine/CAFile.h.
- **q325, port (1 grade, DIFFER):** engine/C3DClothPrimitive.h has no vtable model.

### deferred (1)
- **q194:** binders 0x00B9E950 and 0x00BD1F70 are held by NF38.

### carry (277)
These were not reached; each keeps NF35's round-2 note in QUEUE.tsv as its worklist. Exception: q286 was reached and not changed. 19
NUISystem rows model CCountedPointer<NUISystem::CAction> as a local stand-in that is also in their own signatures, so the respell is a
re-model of rows outside this queue.

## Rows
- **Changed:** 83 rows (rows/), 17 tests (rows/*_test.cpp). Every PassPattern was kept.
- **RENAMES.tsv:** section 1 has 39 self renames, section 2 has 62 caller respellings. Some self rows are outside the queue:
  - 0x008EF910 UpdateCoinSounds(bool), needed by q76;
  - 0x00429F43 and 0x0069BBA0, deleters whose own leaf changed.
- **Section-1 cross-checks:**
  - Each new name is the spelling the shared header already declares at that address, or a PDB name proven by Ego_r donor order plus the
    callers' bindings.
  - No name moved onto an address that already has another name.
- **Tests:** each test that named the stand-in was respelled to the new name.
  - The tests 0x0076C7F0, 0x00A4DEA0 and 0x00687D80 keep their pre-existing local models and change only the method signature.

## Names
- **Descriptive (no PDB/FSE/EgoCore symbol), kept from the queue evidence:** g_MaxAntialiasing, g_MaxResolutionWidth, g_MaxRefreshRate,
  g_MemoryA and g_MemoryB (0x01375080..0x013750A0). One name per address now across the 7 rows that use them.
- **No made-up names were introduced.**
- **Literal-pool constant:** 0x0125916C is `__real@41000000`. Row 0x00977820 now writes 8.0f, not a named global.

## Also found (not mine to fix)
- 0x0076D460 binds `?IsAlliedWith@CFaction@@QBE_NPBV1@@Z` at 0x006B9460. Ego_r spells that address
  `?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z`, which is my overlay opt-in.
- Pre-existing renaming macros that include a shared header and then model the class locally (the round-2 lesson):
  - 0x00972490 `#define CAIStateGroupBase CAIStateGroupBaseLayout`;
  - 0x00B991D0 `C3DMeshInfoLayout`;
  - 0x00F37170 `#define CTexture CTexturePdb`;
  - 0x00689130 `#define CPlayer CPlayerPdb`.
  I edited 0x00F37170 and 0x00689130 only at one declaration each and did not add or touch their macros.
- 0x00B906C0's own signature `DrawVisibilityTestBox(void* box, void* colour)` names its second argument "colour", but it is the occlusion
  handle passed to EndOcclusionTest(unsigned long). The row casts it; its own leaf was left alone.

## Rule slips
- **gb.sh paths:** the first gb.sh run of three port rows used relative body paths, and the compiles read empty bodies (OBJDUMP_ERR). The
  rows were rewritten and regraded; nothing bad was kept.
- **ed.py call lines:** ed.py once wrapped a call line (not a declaration) in `private:` in 0x00B2C4E0. I fixed it by hand before grading,
  and hardened ed.py.
- **Reverted rows:** several rows were taken and then reverted after failed grades: 0x0068B7C0, 0x00A93A60, 0x00704A80, 0x009AEF60,
  0x009AF480 and 0x009B05F0. Each copy was deleted and its lock released. 0x008DF3A0 and 0x00972490 were claimed and released without an
  edit.
