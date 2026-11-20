# NF159 notes (round 10, TC component / AI creature naming)

Counts come from STATUS.tsv / APPLIED.tsv. Rows changed: 47 (30 in-place, 17 ported onto shared headers); graded one by one
(RELOCATION_MATCH) and rechecked in one vland run (recheck_final.log). STATUS also lists 8 rows left unchanged: blocked binders
0x007A6DF0, 0x007E1AA0, 0x007D5AD0, 0x007D7520, 0x006A2370, 0x006A23E0, 0x008F7BD0, and the not-a-binder 0x004804DE.

## Descriptive names
None new. One descriptive name was replaced by its PDB name: 0x008F09E0 TurnAllCards -> CardsFinishedMoving.

## Files moved by catalog renames (self renames that change the catalog name)
- 0x007D3510 `vfunc_32` -> `NewRound` (?NewRound@CTCTavernGameCoinGolf@@UAEXXZ, both PDBs + vtable slot).
- 0x008F09E0 `TurnAllCards` -> `CardsFinishedMoving` (?CardsFinishedMoving@CTCTavernGameCardBase@@UAEXXZ, both PDBs + vtable slot).
- 0x007D2900 `FableEndCut_007d2900::NewGame` -> `CTCTavernGameCoinGolf::NewGame` (class part of the catalog name changes).
No EH funclet includes any changed row (`grep -rnE '#include.*_(<47 addresses>)\.cpp' rebuild/src/compiled rebuild/tests`: no hits).

## Flags lines
None.

## Callee respellings added to keep one spelling per address (each family moved whole; coll.py clean)
- 0x004C10E0 ?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ: 0x006D5B50 bound the stand-in
  global GetHeadPosition; moved with its q4421 respell (spellings at the address 2 -> 1; self row + 0x007EFF80 already bind it).
- 0x0088C160 CAIBrain ctor: 0x00833010 (its only landed caller, exe scan) now binds the self row's exact symbol
  (CMainGameComponent* first, CDefPointer<const CBrainDef>; 2 -> 1). The self row's pointer parameters differ from both PDBs
  (reference parameters): NAME_FIXES.tsv. The caller converts GetDisplayEngine() (retail 0x004C7A70 = CThing::PeekComponent, a
  split-heavy address left alone) to CMainGameComponent* at the call site.
- 0x00436AC0: 0x006A2220 moved from the CArray InsertGrow stand-in to the landed ?_M_insert_overflow@?$vector@PAVCThing@@...
  spelling (census first: 7 binders already on it).
- 0x008EF190 ?EndRound@CTCTavernGameCoinBase@@UAEXXZ (was QAE): binders 0x007CF4C0, 0x007D2920 (the self row is an extern "C"
  stand-in, see NAME_FIXES.tsv).
- 0x008F09C0 ?SelectCard@CTCTavernGameCardBase@@UAEXXZ: 0x007D5FB0 moved to the PDB spelling (the split at this address shrinks,
  it does not grow).
- 0x005EB4B0 GetHeroStats, 0x005EB080 ?SetStateAfterOK@CTCTavernGame (IAE), 0x005EBFA0 CameraUseCameraPoint: the header ports
  bind the spelling the self rows already define.
- 0x007D2EF0 ?SetupForNewRound@CTCTavernGameCoinGolf@@AAEXXZ (QAE -> AAE, both PDBs): self row + its only binder 0x007D3510.
- 0x008EF390 ?EnterGame@CTCTavernGameCoinBase@@UAEXXZ (QAE -> UAE): self row + binders 0x007CF530, 0x007D34A0.

## Tests left as they are
The tests of 0x006D5B50, 0x007153B0, 0x00847AC0, 0x0084A5F0, 0x00848730, 0x0084A350, 0x008FD0A0, 0x00833010, 0x006A10A0, 0x006648D0,
0x00897220 and 0x006A2220 are standalone copies (their own local models, a copy of the body and their own stubs; they do not link the
row object), so the respells do not reach them and they keep passing. They still spell the old names in their local copies.

## Test rewrite
- 0x007D2900: rows/007d2900_test.cpp rewritten on the shared headers (callees stubbed, qualified non-virtual call on raw storage,
  links the row object); PassPattern `ENDCUT_007d2900_OK`.

## Local CThingObject stand-ins (q4474)
- 0x00848730, 0x0084A350: the empty local `class CThingObject : public CThing` existed only for the old PutThingInCarrySlot
  parameter; dropped (no new class token).
- 0x008FD0A0: the local CThingObject (Create 0x00703210) now derives CThing so `*thing` binds CThing& (RTTI .?AVCThingObject@@:
  CThingGameObject@0 : CThingPhysical@0 : CThing@0; direct bases elided, layout-only).

## Anonymous-namespace hash (0x00783380)
The row's static BOASTING_CROWD_MEMBER_SCRIPT_NAME (0x013BADE8) mangles with a path-dependent `?A0x<hash>` (HEAD probe 0x263641ec,
scratch build 0x8c0ee8c7, reloc_bindings 0x95c8fd80). Not a rename: left out of RENAMES section 2.

## In-place local-model notes (advisor rule 2026-11-20: same class tokens as HEAD, no new symbols on the respelled class)
- 0x005EA100: adding the virtual made the implicit vptr 8-aligned because of a double member; the +0x08..+0x1F span is now an opaque
  `_base08[0x18]` so the layout is unchanged.
- 0x007D34A0: the empty local CTCBase base sat after the derived vptr (`lea ecx,[esi+4]`); declaring `virtual ~CTCBase();` in the local
  CTCBase puts the vptr at +0 again (pad 4 shorter).
- SetState(long) -> SetState(nested `ETavernGameStates`) and PlaySound -> protected `PlaySoundW` returning unsigned long: the prior access
  label is restored after the inserted protected block.

## Coordinator decisions needed
1. **q4047 / q4049 (??_G stand-ins 0x007A6DF0, 0x007E1AA0).** The real ??_G...UAEPAXI@Z is emitted only with the vtable, i.e. in a TU
   that defines the ctor or dtor (0x007A66A0 / 0x007A66F0; 0x007E1510), all rows owned elsewhere. Experiment work/exp/e1.cpp: a
   `delete p` scaffolding emits neither ??_7 nor ??_G. Options: a funclet-style include of the dtor TU, or leave the stand-ins as
   NAME_FIXES entries. Needs a decision.
2. **q5079 ctor family (0x007D5AD0, 0x007D7520).** Porting the two ctors onto the headers moves the CTCTavernGameCardBase ctor
   0x008F1CD0 from CThing* to the PDB CThing& with every binder: 0x008F1CD0, 0x007D5AD0, 0x007D7520, 0x007DB280, 0x004D5A11,
   0x004D5A44 (6 rows; the factories 0x004D5A11 / 0x004D5A44 are outside this slice). Needs a family claim.

3. **q11671 (0x006A2370, 0x006A23E0).** Both rows DEFINE a same-TU noinline stand-in body of CTCCarrying::GetAllCarriedThings
   (0x006A2220 was compiled just above them in retail; with a declaration only 0x006A2370 loses the caller's zero store, per its
   notes). Respelled to the PDB `std::vector<CThing*>&` the stand-in would be a second strong definition of the symbol the self row
   0x006A2220 defines. Same TU-emitter question as 1. 0x006A23E0 additionally models CTCCarrying, CBaseIntelligentPointer, vector,
   CWorld and a scene chain inside namespace NRemap006a23e0.
4. **0x008F7BD0 (q4981, q11769, q11770).** NF155 released the lock late in this run (not attempted). The row is a whole TU inside namespace
   NFoot008f7bd0 (read first-hand: local C3DVector, CCharString, CThing, CThingCreatureBase, CTCDParticleEmitter, CThingVector, CBaseClass,
   CParticleUpdateParameters);
   the PDB spellings need a port onto fable_string.h, stlport and the engine headers first (NF155 STATUS says the same for q4046).

## Needs header claim
None.

## Rule slips
- Several rows were edited in work/ before their locks were claimed; every one was free or `released`, and all were claimed before
  grading (lock owner NF159).
- A few Bash calls exited 1 (a grep with no match at the end of a pipeline; a `cat` of a missing lock file). No retries of failing calls.
- Queue order: q4273, q4274, q5079 and q5123 were worked inside the tavern family (same rows, same edits) before q4421..q4720.
