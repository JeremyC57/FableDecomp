# Landing nf285 (Account A, 2026-11-22): T1 standin_alias, NF284's continuation (18 rows, 1 header opt-in)

Hand-back: `pending/2026-11-22_nf285_handback/`. 18 entries done (18 binder rows changed), 67 skipped as ports (queued in
`naming/T3_PORT_QUEUE.tsv`), 231 not started; nothing rejected.

- **0x009E1DD0 GFAYToVY (ruling D3, both stand-ins):** 0x0048F50C's `__stdcall GFAYToVY_009e1dd0` (the table witness) and the `__fastcall`
  stand-in of 0x00437210 / 0x0043E4D0 all bind `?GFAYToVY@@YIMM@Z`; the stdcall -> fastcall move is byte-neutral (objsec ALL_EQ). 3 -> 1 spellings.
- **TavernServer family (0x00966AF0, 0x00966CF0, 0x00966DA0, 0x009670A0, 0x00967110, 0x00967180):** binder 0x00967650 (a Methods struct over
  the header's data-only view) calls the real members through a new add-only opt-in `FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_FLAT_METHODS` in
  `engine/CAIStateGroup_TavernServer.h` (declarations only; the six spellings in both PDB name sets). equiv_check over the header's 15 landed
  includers: 0 differ; the planted-token control flags 11. Binder 0x00967B40's `FreeBarPoint_009670a0` respelled `FreeServingPoint` (Ego_r
  donor order and the self row: 0x009670A0 is FreeServingPoint; the old comments had it wrong).
- **In-place respells (12 binders):** LockStrafingCombinationAnimsToMainAnim 0x008E97B0, RemoveBoastQuestInfo 0x00D17E70, ResetCameraOnStick
  0x007A9B10 (private), MoveToNext / MoveToPreviousTestQuest 0x0061A9D0 / 0x0061AA80, IsActivatable 0x00702810 (two binders),
  OnShowTrophyPerformAction 0x006DE8C0, IsEatTime 0x00658BB9 (const, two binders), IsThingInViewAngle 0x00664E50, WorldUpdateWeaponTrails
  0x007034D0 (protected). IsExpression 0x005D9830: binder 0x005BE028 onto the existing `FABLE_RETAIL_CTCINVENTORYITEM_KINDS` opt-in as its siblings.
- Proof: objsec HEAD v new ALL_EQ under each row's renames; links resolve every moved reference against its self row, 0 new duplicates.
- Table: 19 lines deleted, 11 PDB lines added.

## Rulings on NF285's questions
- E1: 0x0048F50C, 0x004B1590, 0x004B4AA0 (tests clang cannot precheck: VC7.1 `<ostream>`, explicit-specialisation order) land on this
  landing's recheck, which compiles their tests with VC7.1: all three PASS, so the conditional table lines apply.
- E2: the continuation (NF287) may keep pulling in-place-shaped entries first (the owner's "batch items that are easier"), but every
  classification opens the binder: the 23 entries NF285 classified from the spellings column alone are re-read first.
- E3: the 67 skipped ports are queued by class as T3 units (`naming/T3_PORT_QUEUE.tsv`, with the earlier batches' held ports).
- E4: NAME_FIXES queued (`land1122nf285`). 0x009E1E40 GFVYToAY, which four CRenderManager2D rows declare as GFAYToVY (one name at two
  addresses), goes to NF287 as its first entry; 0x009687B0's self row defining TavernServer::IsCarryingJug (0x00966CF0's name) stays a decision.
- The precheck option `NOWCHAR=1` (`-Xclang -fno-wchar`, VC7.1's default wchar_t handling) is accepted.

## Recheck
`logs/recheck.log` (landed tree, 18 rows with their tests): 18 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
Rule slips (noted; the NF287 brief repeats the rules): one `rm` on a variable path (a symlink in its own work folder); one equiv_check ran
with S at the shared scratchpad (nothing written there, checked).
