# NF123 notes (round 10, PRIORITY slice 8, 2026-11-20)

All numbers below come from APPLIED.tsv / STATUS.tsv / rows/.

## Coordinator decisions needed
1. **q3941 (0x008B0A60 virtual ~CCombatSequenceBase).** The self row is ported (QAE -> UAE, RELOCATION_MATCH PASS) and 0x006F1770 already
   binds UAE. 63 old stand-in rows (62 scalar-deleting-destructor rows binding `?Dtor@<Derived>@@QAEXXZ`, plus 0x008B5530's free fastcall)
   are blocked: port (CCombatSequenceBase lives only in engine/_quarantine/). The self rename shrinks the spelling set, so gate.py
   reports "existing", not CREATED. The brief's "every REFS row on the new spelling" sentence reads stricter than the gate:
   decide whether the self rename lands now or waits for the family.
2. **Header claim: CTCDPhysicalObstruction::ThingsToIgnoreObstruction.** It is `unsigned char[0xc]` in the shared header. Typing it
   (STLport `vector<CIntelligentPointer<CThing> >`, with push_back 0x004AD520 spelled once) unblocks 0x008C6B20, 0x008F5060, 0x008F5220
   and 0x008F5490, the last rows of q6401 and q6402. Each row reaches the obstruction through a driver stand-in that also pushes into the
   ignore vector: CTCDriverED (made up), and in 0x008F5490 a local CTCDriverBase. So an in-place respell would be a new local model.
   Note: 0x008C6B20's def field `ObstructionValue` is a long, but SetLifeTimeSeconds takes a float. Check the def's real member type
   when that row is ported.
3. **q6394 / q6395.** The only wrong binder, 0x008E9A30, is locked by NF126 (nflocks). The self rows already emit the claims.
   Recorded `deferred`; the binder's change is a one-line in-place respell for whoever owns it.
4. **q6403: 0x009551A0.** The lock owner is 'released' (not claimable) and the row is a stand-in hierarchy: blocked.
5. **q4397: 0x00DFDE30.** A whole-row CRetailVector stand-in. Two re-author forms gave DIFFER (145 vs 95, then 74 vs 95; an inline-decision
   residue): blocked: port.
6. **q4958: 0x00934920.** It binds GetNearestMember on a local CTCCreatureGroupTag stand-in, which is not derived from CTCCreatureGroup.
   One header form failed: `CTCCreatureGroup.h(45) C2118` size assert, plus `C2039 'AddMember'` (the header's slot-28 name is
   AddNewMember). The row models the CThing chain, CWorld, CAIBrain, CAIStateGroupBase, CDefinitionManager and CMessageEventManager
   locally; these must move first.
7. **q3970: 0x0086E4B0.** A whole-row stand-in for `vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, ...> >::_M_fill_insert`,
   with same-TU noinline callee stand-ins. It needs a port onto engine/CVectorMap.h, engine/CRandomAppearanceMorph.h and the STLport
   vector model. No form tried.

## Self renames that moved a stand-in onto shared headers (this pass)
- 0x007FD3F0: `CInputManager_007fd3f0::SetVectors(..., long)` -> `CTCDPhysicalObstruction::SetParams(..., float)` (q5136, PROPOSAL).
  Evidence: Ego_r 0x007F86A0 + FableWin 0x0278A9F0, member order, field offsets, and the caller passes a float. MATCH.
  It uses the new overlay opt-in FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_SETPARAMS.
- 0x007FD3D0: `CAIStateGroup_BuyFromSeller::BoughtSomething` -> `CTCDPhysicalObstruction::SetLifeTimeSeconds` (q6401).
  - The row DEFINED the global `g_BuyFromSeller_Scale`. It now reads `CMainGameComponent::GetConstantFPS()`, so 0x01375550 binds
    `?ConstantFPS@CMainGameComponent@@1JA`.
  - Another landed row, 0x007F2100 (also named BoughtSomething, same body shape), still defines `g_BuyFromSeller_Scale`.
    This is an existing split outside the slice (NAME_FIXES 0x01375550).
  - Two sources agree that 0x007F2100's catalog name is suspect: the same stand-in, and the same body as SetLifeTimeSeconds.
    Its real name was not settled here.
- 0x0088B7D0: the stand-in `DrawGetWorldMap(S*)` -> `CAIBrain::PeekCurrentStateGroupName() const` (q6435/q6439; existing
  STATEGROUPTEXT opt-in). MATCH.
- 0x0085ABB0: `GetDef_0085abb0` -> member template `GetDef<CMeleeCombatKnockdownEffects>` (q5100), in the form of the landed sibling
  0x0083E0D0. The only PDB instance is FableWin 0x0282E620.

## Name fixes outside my rows (NAME_FIXES.tsv)
- 0x00693C70: CCreatureActionBase ctor, by reference.
- 0x008D86E0: CModeDragonFireBeam ctor, AAV/AAV/M. The row's own leaf was left alone; only its callee was respelled.
- 0x00857F30: CCombatActionBase(creature&, target&, long, const CSoundPair&). 0x0087FD20 and 0x008CE290 keep the landed PAV binding
  on purpose, because the rename must take all of its REFS rows at once.
- 0x01375550: ConstantFPS (see above).

## Descriptive (made-up) names
- `SetObstructionBetween(CTCDPhysicalObstruction*, CThing*, CThing*, CTCPlacement*)` in 0x008CAF10 is a free inline forwarder with
  no retail symbol (it is inlined). It replaces the stand-in member CObstructionShape::SetBetween and keeps the retail argument
  evaluation order. Descriptive.
- No other new made-up names. The q6403 transformer (tools/cab.py) only adds the real static `CCombatActionBase::GetSoundPair` to each
  row's existing local CCombatActionBase model.

## Splits
No split is created. Every touched address either ends with one spelling across its REFS rows, or is an existing split whose
spelling set shrinks or stays the same:
- 0x007FD4B0: 4 wrong spellings are left in the 4 blocked rows.
- 0x007FD3D0: 3 wrong spellings are left in the same rows.
- 0x008B0A60: 63 rows.
- 0x008561A0: 0x009551A0.
- 0x008128F0: 0x00934920.
- 0x00DFDE30: q4397.
- 0x0086D570: q3970.

Closed fully this pass: 0x008CE290 (q5015: 0x00909070 respelled), 0x008D8580, 0x0088B7D0, 0x007FD3F0, 0x0085ABB0 and 0x0087FD20.

## Entry-file oddity
For q6394 / q6395 / q6398 / q6401 / q6402 / q6403 / q6404 (land0511x / chunk 796 rows) the `correct_symbol` column holds the WRONG
spellings and the claim is in the `evidence` column. Every claim was checked against the PDB name sets before use.

## EH funclets
`tools/funclets.sh` over all 222 changed rows finds no landed funclet (rebuild/src/compiled/01/21) that includes a changed parent's file.

## Tests
- New or changed tests: 0x008AC120, 0x008561A0, 0x0082E8F0, 0x0082E9A0, 0x00816AA0, 0x00802390, 0x00830130 and 0x00909070
  (the 0x00909070 test follows the row's model).
- All other tests are the landed standalone ones, unchanged. Some still spell the old stand-in names inside their own self-contained
  models (for example 0x007FD3D0 and 0x007FD3F0); they do not link the row object.

## Binding check (pdiff, HEAD row vs hand-back row, all callee/data pairs)
I compiled every row changed in this session's second half (0x0087FD20, 0x008128F0, 0x008CE290, 0x00909070, 0x0085ABB0, 0x007FD3F0,
0x007FD3D0, 0x008CBB90, 0x0078F4D0, 0x008C44B0, 0x008CAF10, 0x008D8580, 0x008D86E0, 0x0088B7D0, 0x00922600) both ways. The only
binding differences are the intended ones:
- 0x008CE290 (in 0x00909070);
- 0x01375550 (in 0x007FD3D0);
- 0x007FD4B0 (in 0x008CBB90 / 0x0078F4D0 / 0x008C44B0);
- 0x007FD3F0 (in 0x008CAF10);
- 0x008D8580 (in 0x008D86E0);
- 0x0088B7D0 (in 0x00922600).

The self rows' own leaves move as listed in RENAMES section 1.

## q6394 / q6395 and NF126
NF126's STATUS lists 0x008E9A30 as "none ... not reached: entry open (q6454)": NF126 has not changed it. The row needs two in-place
callee respells to the claims:
- 0x008E8FF0 `?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z`;
- 0x008E9080 `?GetAnimDistanceAndIndexFromAnimName@NCreatureMode@@YI_NABVCThingCreatureBase@@ABVCCharString@@AAJAAM@Z`.

## Rule slips
- Some orientation Bash calls ended on a grep/ls with no match, so they exited non-zero. One was in the earlier part of the pass;
  another was the q5015 orientation call (`ls ... | grep` printed nothing). Neither wrote anything.
- One unquoted heredoc carried a backticked retail-byte comment, and the shell tried to run `[+0x38]` (command not found). It was
  harmless: no file was written and nothing compiled. It was redone with a quoted heredoc.
- One pdiff loop printed empty pair diffs for three rows (a transient, probably the wineserver). Rerun one at a time, they showed the
  expected diffs.
