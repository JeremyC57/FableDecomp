# 0x00ED8680 NScript::CV_TalentlessBardScript::CTB_Bard::Main -- B24A9 (2026-11-24)

## Lessons that apply
- INLINE_DEPTH_REVISIT.md "the 65": retail CALLS the CScriptThing copy 0x004ABE90 inside the expanded CIsThingAlive ctor (SetPredicate
  at entry, 0x00ED86A4) and in the CSpawnedFunc1 parameter copy (0x00ED877F), where ours expands the copy. Same blocker as 0x00DE9690.
- Brief gotcha: ~CIsThingAlive / ~MovieBase / ~CTimer inline cost decide inline-v-call at exits; read $S/w22/b24a/SHARED_FORM.md and
  $S/w22/b24b/SHARED_FORM.md before working exits.
- AddSpawnedFunction: the DEFAULT_REGION opt-in is gone; landed siblings (0x00ED5F50, 0x00E07640) pass the region explicitly,
  `..., CCharString(""))` = retail `push -1; push 0x122D70E; call 0x0099EBF0` before `call 0x00CB7E50`.

## State (3 grades by B24A9)
- The released draft did NOT compile on today's headers (TIMEOUT: it included engine/CV_TalentlessBardScript.h, a never-committed
  overlay). PORTED: now includes engine/NScript_CV_TalentlessBardScript.h with a new additive overlay opt-in FABLE_RETAIL_CTB_BARD_DECLS
  ($W/hdr/, README there: nested CTB_Bard from Ego_r.pdb struct_layouts_egor CTB_Bard 44 bytes + reloc_bindings symbols), region
  argument explicit. Grade: DIFFER(3793v3652) 54 hunks = the best on record; differ/00ed8680.cpp is the ported text (b9/p1.cpp).
- First hunk (0x00ED868C): retail `lea eax,[esi+8]; push eax; lea ecx,[esp+6Ch]; ...; call 0x004ABE90` (CScriptThing copy called inside
  the expanded CIsThingAlive(Me)); ours expands the copy (`mov eax,[esi+0Ch] ... inc dword ptr [eax]`).
- Doubts / cleanup for the next agent: the draft still declares local enums EScriptAIPriority / ETextGroupSelectionMethod, (the local
  `inline CIsThingAlive::CIsThingAlive(const CScriptThing&)` body is the header's own contract, engine/CIsThingAlive.h lines 7-8: keep), `extern pGlobalScriptDef` and `operator+` -- check each against the
  headers (CIsThingAlive.h, CScriptDef.h, fable_string.h) and replace with the header text; the TB_T / TB_SAY / TB_START_SCRIPTING
  macros are a source device to review. The landed CTB_Bard siblings (0x00ED7D50 ctor, 0x00ED7E00 Init, 0x00ED99D0 GetRandomSpeech)
  still carry local CTB_Bard models; the overlay opt-in is the header form they could move onto (not done: landed rows).
- Next idea: the CIsThingAlive copy split (INLINE_DEPTH_REVISIT) -- no pragma-free spelling known lane-wide; check whether the
  CSpawnedFunc1 / CIsThingAlive header texts (engine/CSpawnedFunc1.h, engine/CIsThingAlive.h) match what retail's TU could see.
- INLINE_DEPTH_REVISIT.md line 239 ("the 65") is exactly this residue: retail calls 0x004ABE90 / 0x004AA840 out of line inside
  inline-expanded CIsThingAlive / CSpawnedFunc1 special members; /d2inlT55-65 reproduces 9 rows but is a diagnostic only. 0x00DE9690
  shares the same first hunk (compiles today, DIFFER(3428v3412) 55 hunks): one finding serves both rows. Landed sibling
  0x00ED8120 (CTB_BardSpawnPoint::Main) not yet compared for a 0x004ABE90 call inside its predicate ctor.

## B24A10 (2026-11-24): RELOCATION_MATCH (flags /d2inlT51), vt PASS, strict vland PASS (no WARN / RELOC_ lines). payload.json written.
LESSON (top): a quest-script inline split that the stock threshold cannot separate is three decisions read from the c2 inline trace
(a10/inl/*.txt, tool copied from $S/w22/b24a/00d11930/inl): pick the per-row /d2inlT<N> that separates retail's called scores from its
expanded ones (here called 65 / 51, expanded 48 -> N = 51), after the definition-level inline_depth(0) devices fix the nested copies.
- What made it match (a10/c1.cpp = 00ed8680.cpp = differ/):
  1. `#pragma inline_depth(0)` round the row-local inline CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (chunk H40 lever,
     landed 0x00D82640 / 0x00D8BCD0): the nested CScriptThing copy 0x004ABE90 is CALLED at 0x00ED86A4 (54 -> 52 hunks).
  2. explicit specialization of CSpawnedFunc1<CV_TalentlessBardScript,CScriptThing>'s ctor under inline_depth(0) (landed CSwordCrowd::Main
     0x00ED5F50 device): Par1 copies called at 0x00ED877F / 0x00ED886F (3729, 38 hunks). An in-class pragma on the template ctor in a
     CSpawnedFunc1.h overlay does nothing (template instantiation ignores it) -- not kept.
  3. main loop `for (;;) { do { if (IsActiveThreadTerminating()) return; } while (0); ... }` (same-TU landed WatchForAttack /
     WatchForResetWanderCentre form): const-0 register and exits as retail (the while form keeps ebp=0: 19 hunks).
  4. /d2inlT51: hit-block terminate return ~STB (score 65) and talk-block MovieBase ctor (score 51) called, normal exit ~STB (48)
     expanded. T50 3740; T51 match; T60 needs an extra in-body inline_depth(0) round the talk-block `movie` (L3.cpp + T60 also matches).
- Cleanup done: TB_T / TB_SAY / TB_START_SCRIPTING macros expanded in place; local enums -> engine/EScriptAIPriority.h (existing opt-in
  FABLE_RETAIL_PHASE1_ARCHER_PASS26, as landed 0x00D43DB0) and engine/ETextGroupSelectionMethod.h; extern pGlobalScriptDef ->
  engine/fable_script_def_global.h; operator+ -> fable_string.h FABLE_RETAIL_CCHARSTRING_CONCAT_STRING. Kept (header contracts): the
  row-local inline CScriptThing copy ctor and CIsThingAlive ctor, the declared-only CP<MovieBase> dtor specialization.
- Overlay: unchanged from B24A9 (hdr/engine/NScript_CV_TalentlessBardScript.h, FABLE_RETAIL_CTB_BARD_DECLS). No new opt-in.
- Pick-any (pickany_vs_retail.py on lv object): 9 EQ, 2 NEQ, both byte-identical to copies landed rows already emit (a10/pk/cmpcopy.py):
  ??1CScriptGameResourceObjectMovieBase 0x006E7B80 LEAF-REFERENCED (= landed 0x00D62480's copy: the family's call-form ~MovieBase; retail's
  0x006E7B80 comes from another TU, B24A4 SHARED_FORM) and ??1CSpawnedFuncBase 0x00CDD4C0 not leaf-referenced (= landed 0x00E07640's copy).
  The first one is a hold question for Account A: see ESCALATE.md.
- Test: 00ed8680_test.cpp (model + body verbatim, 12 scenarios); a mutated body (intro flag not set) FAILS.

## B24A10 FINAL (supersedes the /d2inlT51 hand-back above): RELOCATION_MATCH with STOCK flags (a10/d1.cpp = 00ed8680.cpp = differ/)
- Debug build (Ego_d.pdb locals + FableWin 0x03AB2C80): locals sehMe, randomSpeech, _Fail_Val (TRetVoid), speech_movie /
  PauseMyEntitiesPlease, YNQAnswer, return_tag (both line buffers), int verse; every terminate exit `return _Fail_Val.Return();`; the main
  loop is `if (false) {} else for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); ... }` and
  the verse loop `if (false) {} else for (int verse = 0; ...)` (FableWin `xor reg,reg ; je ; jmp` openers, GOTCHAS 442). With that
  text the stock threshold gives retail's split: no /d2inlT51, no do-while(0) head, no movie pragma.
- Still needed (each removal measured): inline_depth(0) round the CIsThingAlive ctor + NOVTABLE (without: 3668 / 4 hunks) and round
  the CSpawnedFunc1 ctor specialization (without: 3684 / 20 hunks).
- LESSON: read FableWin's loop openers before any inline-budget device: the for-scope idiom + `for (;; NSF)` changed c2's block
  frequencies exactly as retail's (the device-only route needed a flag).
- vt PASS (test rebuilt on the new body; mutated body FAILS); strict vland PASS, no WARN / RELOC_ lines; payload.json has no flags.
- Pick-any, every code COMDAT checked (a10/pk/): reloc_bindings checker 9 EQ + 2 NEQ (??1MovieBase 0x006E7B80 leaf-referenced,
  ??1CSpawnedFuncBase 0x00CDD4C0 not) -- both byte-identical to landed copies (0x00D62480 / 0x00E07640); vtable members by hand:
  CSpawnedFunc1 ??_G = 0x00ED99A0 and SuspendableProcess = 0x00ED9960 (vtable 0x012F37D0 slots 0 / 4) equal up to relocation fields;
  ??_GCIsThingAlive (no table line, 30 bytes) differs from 0x00CDF160 but is byte-identical to the copies landed H40 rows 0x00D82640 /
  0x00D8BCD0 emit (HEAD debt, not leaf-referenced). ESCALATE.md has the ~MovieBase hold question.
