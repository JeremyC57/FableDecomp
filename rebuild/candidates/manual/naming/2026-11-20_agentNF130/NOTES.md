# NF130 notes (round 10, PRIORITY slice 7 fourth remainder)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries (8): 3 done (q6394, q6395, q6454), 0 done-already, 0 rejected, 0 blocked, 0 deferred, 5 open (q6453 / q6455 partial: one more
  binder; q6408, q4802, q4747 not started).
- Rows: 3 ported (0x0073DFF0, 0x008E9A30, 0x00862EC0: RELOCATION_MATCH + PASS in one recheck_final.log run, no HARD / RELOC_CONFLICT),
  0 not-a-binder, 0 blocked, 27 not reached (STATUS `none`). Locks: all three changed rows are in rows.txt (no extra claims).
- Header opt-ins (2, new macros, hdr/README.md): CMODESTANDBASE_ANIMUPDATE, CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR.

## 0x008E9A30 + 0x0073DFF0 (q6394 / q6395 / last q6454 binder): one definition, link-proven
- 0x008E9A30 is NF128's finished port (work/NF128_round1/008e9a30_held.cpp), unchanged except one comment block. Graded RELOCATION_MATCH
  under NF128's two overlay headers (copied into hdr/, see hdr/README.md); its object carries the pick-any COMDAT
  ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z (LINK_ONCE_DISCARD).
- 0x0073DFF0 (owner) moved off its local CCharStringData / CCharString / CCRC / CAnimFilterBase / CAnimFilterOfGroup models (all have shared
  headers) onto fable_string.h (FABLE_RETAIL_CCHARSTRING + _GETCRC_INLINE) and engine/CAnimFilterOfGroup.h (NF128's
  FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE). It holds no body: an anonymous-namespace function returns
  `&CAnimFilterOfGroup::SetCurrentGroupName` (`// Fidelity: scaffolding, not retail`), so VC7.1 emits the header body as a pick-any COMDAT
  (selection 2), like every includer (GOTCHAS nf86 / nf88; precedent 0x00686F10). First form RELOCATION_MATCH. Own leaf unchanged.
  Bindings: 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z and 0x004014A0 ?Calc@CCRC@@SIKKPBXK@Z unchanged; 0x0129AAF4 moves from the
  anonymous "" literal to ?NullChar@CCharString@@1DB (already bound there in reloc_bindings; the PDB spelling, as NF128's 0x008E9A30).
- Link proof (wine link.exe 3077, `LINK=` cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED; work/l/):
  - control work/l/ctl.log: tree 0x0073DFF0 object (strong .text definition) + new 0x008E9A30 object -> LNK2005 on
    ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z, LNK1169, exit 145.
  - after work/l/new.log: new 0x0073DFF0 object + the same 0x008E9A30 object -> no LNK2005, exit 0 (LNK4088 + unresolved externals only).
- No other landed row defines CAnimFilterOfGroup::SetCurrentGroupName (grep of rebuild/src: only the owner; 0x0073E040 is
  CAnimFilterOf2Groups::SetCurrentGroupNames).
- Both rows: vland --recheck-landed RELOCATION_MATCH + PASS (work/b1.log), no HARD / RELOC_CONFLICT / WARN lines. Tests unchanged.
- Hand back both or neither. They need nf128's header opt-ins and 0x0073AAC0's UAE leaf; those reached the repo working tree during this
  session (0x008E9A30 itself was held out of nf128, as planned), so the overlay copies were dropped and the final recheck ran on the repo
  headers.
- 0x008E9A30 RENAMES s1 module is `NCreatureMode::CModeStrafe` (the build entry already carries that Module; the file name
  CModeStrafe_OnActivate_008e9a30.cpp may follow the namespace).
- Existing split left as found (NAME_FIXES): the exe scan finds 8 landed rows referencing 0x0073DFF0, all on local stand-ins: SetGroup in
  0x0073AAC0, 0x008D75D0, 0x008D87C0, 0x008D9220, 0x008D92C0, 0x008D9350, 0x008E9850; SetAndHash@AnimationGroupFilter in 0x008D9530. Not in
  my entries; not respelled.
- Name source: only FableWin.pdb has ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z (0x01DD3450); Ego_r has no copy (the
  landed row's comment cited Ego_r; corrected). Second source: the body against the header layout (Group +4, GroupCRC +8).
- Re-checked after nf128 reached the tree: only the owner defines CAnimFilterOfGroup::SetCurrentGroupName in rebuild/src; no file under
  rebuild/src or rebuild/tests includes any of the three changed row files.

## q6453 / q6455 (0x0073A680): 0x00862EC0 ported, 5 rows left
- 0x00862EC0 CModeStandBase::AnimUpdate moved off nine local models onto the shared headers (define set based on NF128's 0x008621A0 and
  NF124's 0x008D6580): the nested CModeBase (VIRTUALS + GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS), engine/CModeStandBase.h (+ NF130
  ANIMUPDATE), CThing.h (ORIGINAL_VTABLE), CThingCreatureBase.h (+ NF130 PEEKMOVEMENTVECTOR), CWorld.h (FABLE_RETAIL_CWORLD), CThingManager.h
  (+ ALLPAUSE), CTCPhysicsBase.h, GFRandom.h. RELOCATION_MATCH in the third form (form 1: C2248 protected MovementVector + C2039 GetFacingVector
  without FABLE_RETAIL_CTCPHYSICSBASE; form 2: DIFFER 333v333, the creature call at +0x140 instead of +0x13c).
- Identity of what HEAD called stand-ins (two sources each):
  - "GetGlobalComponents()->GetGameState()" = CModeBase::GetWorld 0x0073A680 then CWorld slot 4 PeekThingManager 0x0049E1C0 (CWorld.h
    vtable list); the bytes +0x54 / +0x55 = CThingManager NonScriptedEntitesPauseMode / AllEntitesPauseMode (Ego_r layout +0x68/+0x69
    minus the STLport delta, the header's FableWin inline accessors IsNonScriptedEntitesPauseMode / IsAllEntitesPauseMode that landed rows
    0x004A5A40 / 0x00725B70 read in place).
  - "movement component +0x60 slot 70 GetVelocity" = CThing::PhysicsTC (CThing.h assert +0x60) slot 70 CTCPhysicsBase::GetFacingVector()
    (CTCPhysicsBase.h vtable note, CTCPhysicsStandard / RigidBody slot 70).
  - "slot 79 GetMaxJoggingSpeed" was wrong: +0x13c is slot 79 GetMaxWalkingSpeed (CThingCreatureBase.h ORIGINAL_VTABLE order;
    CThingPlayerCreature.h slot 79 0x006A98A0; CThingAICreature 0x00830000 is GetMaxWalkingSpeed). Jogging (+0x140) graded DIFFER.
  - The planar speed reads CThingCreatureBase::MovementVector (+0x10c, header assert) through FableWin's inline PeekMovementVector.
- Bindings: 0x0073A680 now the claim (?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ, 3 sites); 0x0073A6F0 moves to its
  self leaf ?GetNoFramesBeforePlayingCombinationIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z (already bound by
  0x008D6580). Every other pair unchanged. Own leaf unchanged. Test unchanged (standalone). No funclet includes the file.
- 0x008619C0 AddCombinationIdles keeps HEAD's global declaration ?AddCombinationIdles@@YIXPAVCThingCreatureBase@@AAVCAnimateThing@@@Z:
  both PDBs put it in an anonymous namespace of the CModeStandBase TU (?AddCombinationIdles@?A0x9eb7a2f3@@YIXABVCThingCreatureBase@@
  AAVCAnimateThing@@@Z), which another TU cannot name; respelling it would create a new spelling. Existing split, left as found.
- Left (not reached): 0x008D7630, 0x008DC900, 0x008DCB60, 0x008E7B90, 0x008E7CF0. Read-only notes for two of them are in STATUS.tsv.
  0x008E7B90 also binds 0x008621A0 as ?OnActivate@CModeStandBase@@QAEXXZ while the self leaf is UAE (an existing split).

## Descriptive names
None new. No new local class models (every changed row lost its local models; 0x00862EC0 keeps only the free AddCombinationIdles declaration).

## Flags lines
None added; all three rows grade with their build entries' CompilerFlags (none).

## Funclets
No row under rebuild/src/compiled/01/21 includes any of the three changed files.

## Decisions for the coordinator
- Land 0x0073DFF0 + 0x008E9A30 together (link proof in work/l/).
- 0x00862EC0 needs the two NF130 opt-ins (hdr/engine/CModeStandBase.h, CThingCreatureBase.h).

## Rule slips
- Four grades ran q.py directly without the cpp precheck: the 0x008E9A30 base grade, the tree 0x0073DFF0 control compile, the new
  0x0073DFF0 first form, and the HEAD `-v` compile of 0x00862EC0. None spun; later grades went through tools/g.sh, which runs it.
- The first Bash call of the session (listing the slice with `wc` on directories) exited 1.
- The control compile of the tree's 0x0073DFF0, the HEAD `-v` disassembly compile of 0x00862EC0 and the vland runs count in grades_used.
- I did not read GOTCHAS.md, EXTRA_RULES.md and ASM_RULES.md in full. I read the nf80 / nf86 / nf88 / nf96a GOTCHAS lines and the brief's
  required NF92 / NF44 / NF88-90 files.
