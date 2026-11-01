# Chunk 657 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Byte-identical implicit destructors of different classes are separate retail functions (no ICF); settle each one's class from
  the deleting destructor (vtable slot 0) that inlines the same member sequence, plus its callers or its TU neighbours. Five
  `_Dest_val<...CSymbolInfo...>` rows were five creature-action dtors (0x00628EE0, 0x007ED750, 0x006DD690, 0x008C9580, 0x0066A530).
- VC7.1 emits an implicit destructor's out-of-line `??1` COMDAT whenever the TU destroys the object by name, even when every
  use is inlined; the linker keeps it unreferenced (no /OPT:REF), which is why 0-caller `??1` rows exist (0x007ED750,
  0x006DD690, 0x008C9580). A TU harness that destroys a local object is enough to emit it.
- For a base and a derived class with no members of its own, VC7.1 emits the base's implicit `??1` before the derived one's in
  either use order; the derived dtor inlines the base's member sequence, so the two are byte-identical (0x006DD690 ShowTrophy,
  0x006DD6C0 ShowToSheatheTrophy).
- An out-of-line private member that retail inlines into two virtual callers (identical bodies: FinalCleanup, OnKill, OnRemove)
  is a plain member defined above its callers; VC7.1's automatic inlining at /O2 expands it (0x007C7560 / 0x007C75A0).
- Two byte-identical vtable slots: order them from the TU's function order against the Ego_r donor order (CTCPhysicsStandard
  slot 74 SetFacingAnglesFromRHSet, slot 75 SetRHSet; rigid body 0x007599C0 / 0x00759A00).
- engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) gives the def-pointer member release in an implicit dtor byte-exact
  (`test ; je ; dec [ecx+4] ; jne ; call [eax+4] ; mov [m],0`) with no local model (0x007710A0, 0x00529AD0).
- vland's SHARED_HEADER_AVAILABLE check is a substring test for `engine/<Class>.h` in the source; name the header in the
  `// Layout:` comment when a local model is justified.

Review at landing:
- NAME_FIXES.tsv (19 rows) respells landed rows and callers: 0x007C75E0 -> OnRemove; 0x0072AC40 -> CTCSpecialAbilities::
  FadeOrKillEffect(CThing*) (own leaf plus three caller spellings); 0x00759A00 / 0x007239C0 -> SetRHSet and 0x007238E0 -> the
  virtual (UAE) SetFacingAnglesFromRHSet (0x007238E0 and 0x007239C0 currently share one mangled name); 0x006DD6C0 ->
  ~CCreatureAction_ShowToSheatheTrophy; the deleting destructors 0x0062DA90 / 0x00529AB0 / 0x00771120 / 0x008ECE80; ctors
  0x007710D0 / 0x008EC850 / 0x006DF730 / 0x008679C0 (pointer vs reference parameter spellings); the stand-in binding families
  on 0x00693EF0 and 0x0062E900.
- 0x006DD690 vs 0x006DD6C0 (ShowTrophy vs ShowToSheatheTrophy) rests on emission order (lane experiment + retail ctor/??_G
  order), not on a caller; if a later caller or EH table shows otherwise, swap the two names together.
- The landed twin 0x0057A545's source calls itself CAggregate_0057a545 with members of ~CCharString; the catalog now says
  ~CCreatureAction_PlayIntoLoopOutOfAnimation (callers 0x0057B9CA, 0x00843CAC) -- its own identity was not re-proven here.
- The five action rows and 0x00529AD0 carry a `TUHarness_<Class>` function (not retail) whose only job is to destroy a local
  object so the implicit `??1` is emitted; 0x007710A0 uses the real CTCDegradableBarrier constructor instead.
