# NF147 notes (2026-11-20, round 10: q6408 mode-pointer remainder, q4802 vector-assign)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (2): done 0; done-already 0; rejected 0; blocked 2 (q6408: self row 0x0073ACC0 left; q4802: 6 rows left); deferred 0; open 0.
- Rows (STATUS.tsv, 15 lines): changed 10, all RELOCATION_MATCH + test PASS in recheck_final.log (9 binder ports 0x008C48A0, 0x008C0C00,
  0x008C0D10, 0x00848DB0, 0x008C1C30, 0x008676A0, 0x008A08A0, 0x008A1A10, 0x00B21E90 + the self rename 0x00868220); not-a-binder 2
  (0x0075B8D0, 0x008C1900); blocked 3 (0x0073ACC0, 0x00762E10, 0x00764E90 in rows.txt; the q4802 rows outside rows.txt 0x00B21DD0 / 0x00BCD850 / 0x00BCD8F0 / 0x00BCD950 were not
  claimed or touched and have no STATUS line).
- Header opt-ins: 3 (hdr/README.md): NCreatureMode_CModeBriarRoseConcentrate.h CTOR, NCreatureMode_CModeDrained.h CTOR, CAnimFilterOfGroup.h CTOR.
- RENAMES: section 1 one row (0x00868220, own parameters); section 2 23 caller respellings.

## q6408 (blocked: only the self row left)
- All 8 binders NF144 left are ported. With NF144's 16 (HELD, landing as nf144) all 34 REFS rows bind the claim (the self row is not a REFS row).
  None of this hand-back's binding moves needs nf144 (the claim and the 0x00718A00 FableWin spelling are already bound by landed rows); only the
  34/34 census and the later self rename of 0x0073ACC0 need both landed.
- Shapes used:
  - 0x008C48A0, 0x008C0C00: the landed twin 0x008C4990's shape (shared CCountedPointer with VISIBLE_CALLED_CTOR; the row's same-TU (T*) ctor
    specialisation and deleter stand-in dropped). Mode class kept global `struct` so its ctor / SetTarget / the called (T*) ctor keep their HEAD
    spellings (callers.py: these rows are their only binders). Only 0x0073ACC0 moves. The HEAD objects defined the (T*) ctor STRONGLY; now pick-any.
  - 0x008C0D10: header inline ctor + ShareData operator=; `PMode.Data->` (not operator->) keeps the Data load after the GetThingInPrimarySlot call
    (form 1 DIFFER 395v393). Mode class respelled IN PLACE into NCreatureMode (same token, header-less: engine/CCreatureAction_JackScream.h only
    forward-declares it), which is how its landed self rows 0x00740450 / 0x008D7370 define it.
  - 0x00848DB0: header inline ctor (retail expands it) + FABLE_RETAIL_CMODEBASE_CTOR; global CModeSleepBed token kept (vtable symbol unchanged).
  - 0x008C1C30: onto engine/NCreatureMode_CModeBriarRoseConcentrate.h (needs FABLE_RETAIL_CMODESTANDBASE + CMODEBASE_VIRTUALS, form 1 C2118 / C2440
    without them) + the landed 0x008C4CB0 def set; the row's local CThing stays (the mode header does not pull engine/CThing.h).
  - 0x008676A0: onto engine/NCreatureMode_CModeDrained.h; its callee's self row 0x00868220 respelled to the PDB (CThing&, CThing&) so the address
    stays on one spelling (refs scan: 0x008676A0 is its only referencer). Lock 0x00868220 claimed (was free).
  - 0x008A08A0: onto engine/NCreatureMode_CModeScreamerSuperAttack.h (its PDB ctor already declared there); the row's local CThing (the token the
    header forward-declares) is passed by reference: none of the row's other HEAD local models (ScriptMethods, CThingNative, the _quarantine
    CThing include) changes.
  - 0x008A1A10: header-less NCreatureMode::CModeScripted re-based on the shared CModeStandBase with a shared CAnimFilterOfGroup member (the
    CScriptedModeInfo stand-in's real identity: 0x0073E8F0 is the CAnimFilterOfGroup ctor, landed self row + binder 0x0073E9A0).
- Self row 0x0073ACC0: not attempted (STATUS has the port recipe). It is the entry's only row left.

## Spelling census (no new split)
Every binding move lands on a spelling a landed row already binds or a self row defines, except these PDB spellings that replace a stand-in at
the same count (brief rule: "A new PDB spelling replacing a stand-in on a callee is accepted when the address's spelling count does not grow"):
- 0x008C2D60: ?Destroy_008c2d60@@YIXPAX@Z (stand-in, 0x008C0D10 only) -> FableWin ?DeleteData@?$CCountedPointer@VCModeJackOfBladesScream@NCreatureMode@@@@SIXPAX@Z (2 -> 2).
- 0x008E78C0: ??0CModeBriarRoseConcentrate@@QAE@PAVCThing@@0_N@Z (0x008C1C30 only) -> Ego_r / FableWin ...@NCreatureMode@@QAE@AAVCThing@@AAVCThingCreatureBase@@_N@Z (2 -> 2).
- 0x008AC3F0: ??0CModeScreamerSuperAttack@@QAE@AAUCThingNative@@ABU1@@Z (0x008A08A0 only) -> Ego_r / FableWin ...@NCreatureMode@@QAE@AAVCThing@@ABV2@@Z (2 -> 2).
- 0x00868220: binder and self move together to the PDB spelling (1 -> 1).
Counts that shrink: 0x008D7370 (2 -> 1), 0x0073E8F0 (2 -> 1), 0x00861E00 (5 -> 4). Moves onto existing spellings: 0x0073ACC0, 0x00718A00,
0x007180B0, 0x0073EAC0, 0x00740450, 0x00669580, 0x0075B8D0, 0x0075C040.

## One definition (defdiff: objdump -t new vs HEAD object + reverse-prefix check against all landed Leaf lines)
- New defined symbols are pick-any COMDATs only: shared-header template instances (CCountedPointer, CDefPointer, STLport vector / map), header
  inlines, and implicit / inline members of header-less local classes (??_GCModeSleepBed@@, ??_GCModeScripted@NCreatureMode@@, the inline
  ??0CModeScripted@NCreatureMode@@QAE@AAVCThing@@ABVCCharString@@@Z). Landed Leaf owners of new symbols: only template instances whose owners
  are the same header bodies (DeleteData<CTCCreatureModeManager::CModeBase> 0x007180B0, GetSubDef<CSpecialEffectsDef> 0x00669580, the
  vector<C3DVector> members 0x0075B8D0 / 0x0075B560 / 0x0075BB00 / 0x0066E370 / 0x0075C040), as in landed 0x008ED950 / 0x008C4CB0 / 0x00B20B60.
- Three rows drop a STRONG duplicate the HEAD object defined: the (T*) ctor specialisations in 0x008C48A0 / 0x008C0C00, and 0x008C1C30's same-TU
  GetEffectFromString (landed owner 0x00669D30), now the header's pick-any COMDAT.

## Local classes (narrow rule; no class token added to any row)
- Header-less local classes kept and re-based on a shared base: CModeScorpionKingTailPlunge (0x008C48A0), CModeJackOfBladesFlash (0x008C0C00),
  NCreatureMode::CModeJackOfBladesScream (0x008C0D10, respelled in place from global struct), CModeSleepBed (0x00848DB0), all on
  CTCCreatureModeManager::CModeBase (landed 0x0096DBC0 shape); NCreatureMode::CModeScripted (0x008A1A10) on the shared CModeStandBase.
  grep `(class|struct) <Mode>\b` over rebuild/include: no definition of any of them.
- Classes with a shared header that were modelled locally and are now on the header: CModeBriarRoseConcentrate (0x008C1C30), NCreatureMode::
  CModeDrained (0x008676A0), CModeScreamerSuperAttack (0x008A08A0). Removed local models: CModeBase / CCountedPointer / SCountBlock /
  CTCCreatureModeManager (all 8 binders), CCharString / CDefPointer / CDef* / CSpecialEffectsDef (0x008C1C30, 0x008A1A10 CCharString),
  CBaseIntelligentPointer (0x008676A0), CCPPointerInfo (0x008A08A0), the empty ETCInterfaceType (0x008C1C30, 0x008A08A0, 0x008A1A10).
  In-place respells of HEAD local models: 0x008C1C30 CThing's def member -> 0x008C4CB0's `CDefPointer<const CDefClassBase> PDef` + GetDef();
  0x008676A0 CCombatActionBase::PCreature -> the shared CIntelligentPointer<CThing>; 0x00868220 CModeDrained ctor parameters.
- Every other HEAD local model is untouched (vland WARN SHARED_HEADER_AVAILABLE lines are the HEAD ones).

## Decisions for the coordinator
- 0x008A1A10: the shared CModeStandBase(CThing&) takes the real CThing while the row's HEAD local CScriptThing::GetThing returns its stand-in
  CThing_Retail*, so the argument is `*(CThing*)thing.GetThing()` (a pointer cast between the stand-in and the real class; no address constant).
  Accept, or hold the row until CThing_Retail / the local CScriptThing / CPlayer move onto their headers (then q6408 has one more binder left).
- nf144 / this hand-back: independent for landing; the 0x0073ACC0 self rename (the entry's last row, 0 forms, not attempted: context) should be
  resumed once both have landed.
- 0x008C0D10 (in-place namespace respell): the object now defines pick-any COMDATs of CCountedPointer<NCreatureMode::CModeJackOfBladesScream>
  (??0 / ??1 / ??4 / ??C / ShareData / ReleaseData / DeleteData), i.e. symbols naming the respelled class, which is the literal wording of
  condition (2) of the in-place respell rule. I read (2) as satisfied: no landed Leaf owns any of them (reverse-prefix check), they are template
  instances of the shared engine/CCountedPointer.h (identical in every includer), and no shared header defines a member of the class itself (it
  is only forward-declared). HEAD's object already defined the same instances under the global token. Fallback if the coordinator disagrees:
  keep the global `struct` token; then only 0x0073ACC0 and 0x008C2D60 move (0x008C2D60 to the non-PDB ?DeleteData@?$CCountedPointer@UCModeJackOfBladesScream@@@@SIXPAX@Z
  at the same count), and 0x00740450 / 0x008D7370 stay on the global stand-ins.
- 0x00B21E90: the object now defines the pick-any ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z (the shared CArray<C3DVector>'s implicit operator=,
  generated by the CCardinalSpline assignment). That is the exact stand-in name landed 0x00B21DD0 still binds at 0x0075B8D0 (NF138
  bindchk_q4802.txt), so in a full link 0x00B21DD0's call would resolve to this implicit member (which calls 0x0075B8D0) rather than to 0x0075B8D0
  itself. Harmless for the per-row grades; it disappears when 0x00B21DD0 moves to the vector spelling.

## Flags lines
- 0x00B21E90: `// Flags: /d2inlS84` (+ `// Inline-budget:` line with the sweep). Its build_candidates.ps1 CompilerFlags is empty today: it must
  become /d2inlS84 at landing (GOTCHAS catalog-flags line). recheck_final.log ran it with "flags": ["/d2inlS84"] in the payload.

## Self renames / descriptive names
- 0x00868220 (outside the entries, leaf-only, both PDBs agree; its only referencer 0x008676A0 binds the new spelling in this hand-back).
- No made-up names added.

## EH funclets
- No file under rebuild/src/compiled or rebuild/tests includes any changed row.

## Tests
- All 10 tests unchanged (standalone host copies of the bodies; PASS). Rewriting them on the shared headers is a follow-up.

## Locks
- rows.txt (14, as NF147). Claimed: 0x00868220 (was free). Not claimed: 0x007624E0, 0x00762D80, 0x00B21DD0, 0x00BCD850, 0x00BCD8F0, 0x00BCD950.
- Account B: none of the 10 changed addresses is `handed off` / `reserved (acctB` in UNMATCHED_ROWS.tsv (checked below in selfcheck).

## Rule slips
- Read-only Bash calls (cat / grep / objdump) mostly ran without the env.sh preamble; every compile ran through g.sh / defdiff.sh / pdiff.sh /
  vland with it.
- GOTCHAS.md and EXTRA_RULES.md were grepped for the relevant lines and sections, not read end to end.
- First vland run of 0x00B21E90 (work/b5.log first pass) went without the flag (mkpay takes catalog flags) and printed DIFFER(581v619); re-run
  with the flag in the payload.
