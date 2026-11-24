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
