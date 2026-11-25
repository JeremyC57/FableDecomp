# NF446 hand-back notes (2026-11-25): four name-fix units, every self + every landed binder in one batch each

u1-u3 ACCEPT (6 rows); u4 (3 rows) fully gated, HOLD on ruling q-NF446-1 (test-side bodiless declaration). Batches: batches/u1 .. u4 (land_scripted format). Locks held (NF446): 00793cb0 00868b40 0074ac80 0089ec60
00a36fe0 006e72a0 006d6570 00899c90 008e7040.

## Callers
Retail exe scan (tools/xr446.py, evidence/retail_xrefs.txt) for every call / jmp / dword reference to each target, cross-checked with
reloc_bindings column 2 and a source grep of every spelling:
- 0x00793CB0: 0x00868B40 only.
- 0x0074AC80: 0x0089EC60 and 0x0074C20A (CTCCreatureGenerator::FrameUpdate, no catalog row, nothing landed).
- 0x00A36FE0: jmp from 0x006E72A0 only.
- 0x006D6570: 0x0077A180 (already binds the PDB name), 0x00899C90, 0x008E7040, and 0x006482A6 (no catalog row).

## u1 0x00793CB0 -> ?GetDamageMultiplier@CTCUnholyPower@@QBEMXZ (+ caller 0x00868B40)
- Self ported onto engine/CTCUnholyPower.h + CSpecialAbilitiesUnholyPowerDef.h. The define set is the landed OnKill 0x007952E0 set, plus
  FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE (the inline StopSound needs CTCSoundPlayer::StopAttachedSound) and FABLE_RETAIL_CDEFSTRINGTABLE.
  It grades MATCH.
- Caller: only the 0x86 call changes, on the 0x85 CTCDivineWrath pattern (bodiless declaration, r3). `?A@Component@Row00868b40` (0x0078BE40) is
  left for NF442 / its own line.

## u2 0x0074AC80 -> ?AddCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z (+ NF435 hold 0x0089EC60)
- Self: the wrong local CTCShop model is replaced by the 0x0074A1E0 alias pattern (generated header data, no local layout). The row's
  intelligent-pointer / STLport stand-ins are kept so its callee spellings (0x00A01Bxx, __find 0x0074CEE0, push_back 0x0074DA40) do not move.
  ObjGate is ALL_EQ under the rename.
- Accessing the vector through a named reference local put `this+0x1c` in esi (DIFFER). Spelling the reinterpret_cast at each use, as 0x0074A1E0
  does, matches.
- 0x0089EC60: NF435's batch row and test, unchanged. Its MapT deletes are still valid: whole-word MapT appears only in 0x0089EC60 at HEAD.
- After this batch, ?AddShopper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z has one definer, the real 0x00773DC0.
- Pair line 0x0074A1E0: already consistent at HEAD (catalog, self Leaf and the table line added by nf435 b1 all say
  ?RemoveCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z). Nothing to do; the queue lines nf435 / nf437 for it can close.
- Follow-up (not done): the landed self at 0x0074DA40 is `?PushBack@CShopperList@@QAEXABUCIntelligentPointer@@@Z`. That is a shop-attributed
  stand-in for the STLport vector<CIntelligentPointer<CThingCreatureBase> >::push_back that CTCCreatureGenerator code calls (name_gate split_fixable).

## u3 0x00A36FE0 -> ?Validate@CClassFactory@@QAEXXZ (+ NF435 draft 0x006E72A0)
- Self ported onto engine/CClassFactory.h (CLASSLIST, typed std::list<CClass*>) plus the add-only NF435 opt-in FABLE_RETAIL_CCLASSFACTORY_VALIDATE
  (hdr/engine/CClassFactory.h; the original is orig/hdr_CClassFactory.h). It uses engine/CClass.h (FABLE_RETAIL_CCLASS) for the CBaseObject
  base: slot 1 = CBaseObject::Validate 0x0099A400 (header comment, CClass vtable 0x0129CF60). It grades MATCH.
- 0x006E72A0: NF435's draft, unchanged code, graded RELOCATION_MATCH. NF435 had no test; a new linked test prints the landed pass pattern
  `VALIDATE PASS`. vland reads pass_pattern from the build entry, so a new marker fails there.
- Table: delete InterruptAllNavigators -> 00a36fe0. The real CNavigatorManager::InterruptAllNavigators is 0x00A14F40 (landed self, AAEXXZ).
- Header proof: hdrcheck2 gives the same text for every macro of CClassFactory.h; equiv_check (rebuild/include copy v copy + overlay):
  0 of 57,160 landed rows differ (batches/u3/evidence/equiv.txt).

## u4 0x006D6570 -> ?GetExperiencePickUpMultiplier@CTCHeroExperience@@QBEJXZ (+ binders 0x00899C90, 0x008E7040): HOLD on ruling q-NF446-1
- Decision (two sources):
  - Both PDB name sets have the CTCHeroExperience name. ?GetComboMultiplier@CTCHeroStats@@QBEJXZ is in neither.
  - Every caller fetches the component through interface 0x68 = TCI_HERO_EXPERIENCE (engine/ETCInterfaceType.h: 104), including 0x008E7040,
    whose local variable was misnamed `stats`.
  - The body is the engine/CTCHeroExperience.h formula.
- Self is a pick-any re-row (GOTCHAS nf86 / nf88): the header's inline member under FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER (the
  0x006D8300 define set) plus a `Fidelity: scaffolding` helper that takes its address.
  - Why: the landed 0x006D8300 already emits this member as a pick-any COMDAT. That is why the flatten1 T3 line was held ("another row defines it").
  - The out-of-line definition on the HEAD local model graded, but the /FORCE link with 0x006D8300 gave LNK4006 (duplicates 0 -> 1).
  - The re-row's COMDATs are comdat_eq OBJECT_EQ to 0x006D8300's (selection included); the link with that definer has duplicates 0 -> 0.
- Constant: retail adds the double at 0x01237F00 = 1.0. HEAD's local model added 0.5 and still graded RELOCATION_MATCH, because the grade does not
  compare constant values (GOTCHA candidate). The header body adds 1.0; the new test checks it (the 0.5 local-model mutant fails it).
- 0x008E7040: IsMetBy (0x00851B50) keeps `const CTCHeroStats*`. Its split with IsValid@CHeroAbilityStatPackage(const CTCHeroExperience&) is a
  separate naming line.
- 0x00899C90 keeps its stand-in self (T3 nf324 held line). Its linked test must define the callee under the new spelling, which needs a bodiless
  CTCHeroExperience declaration in the TEST; r2/r3 cover rows only. Hence HOLD on q-NF446-1 (RULINGS_NEEDED.tsv): name_gate 10 -> 11 on that row.
- No RENAMES / CATALOG_CHANGES: the catalog name and the file already say CTCHeroExperience::GetExperiencePickUpMultiplier.

## Header-debt booking
r3 (ruling.txt in u1 and u4) and T3.txt nf446 header-debt line: 0x00868B40, 0x00899C90, 0x008E7040.

## T3_PORT_QUEUE lines these batches resolve
- u2: nf291 0x0074ac80 (binder respell; skipped-port); nf324 0x0089EC60 (port); the 0x0089EC60 item of the nf435 family line (TC GetTC / PeekTC
  rows round 2); the 0x0089EC60 MapT item of the nf435 LowerBound split line.
- u3: the 0x006E72A0 item of the nf433 family line (research row, CClassFactory.h Validate opt-in) and of the nf435 family line.
- u4: flatten1 0x006D6570 (identity research); flatten-local2 0x006D6570 (unit with binder 0x00899C90); nf303 0x006D6570 (port census).
NAME_FIXES_QUEUE lines closed by landing: sec2c 0x006D6570, nf435 0x006D6570, nf433 / nf435 0x00A36FE0, nf435 / nf437 0x0074AC80,
nf435 0x0074A1E0 (already consistent), nf443 0x00793CB0.

## Tools
tools/ = NF443's batch tools retargeted, plus: run_b.sh / run_bh.sh (runpair + objgate + pk + link; _bh with the hdr overlay), ng446.sh
(name_gate HEAD v new), strict.sh (LANDVERIFY_STRICT_HEADERS=1, HEAD or new), xr446.py (retail reference scan).
For leafless HEAD rows, name_gate needs the full self_symbols spelling; ng446.sh's HEAD runs were redone with it.
