# wfE12 notes (agentE12)

## 0x0121B9B0 CMainGameComponent::`dynamic initializer for 'EventDispatch'' -- RELOCATION_MATCH, test PASS, addmatch done
- Extent: `push esi` .. 1392 dword stores .. `pop esi ; ret` (0x0121DF0E); catalog length 9567 right.
- Identity: Ego_r.pdb 0x0100E510 + FableWin.pdb 0x03FE65C0 `dynamic initializer for 'CMainGameComponent::EventDispatch''; the
  table at 0x013B9288 is the one CWorld::GetEventDispatchTableEntry (landed 0x0049D8E0) indexes; 87 x 64-byte CGameEventDispatch.
  Member spelling from the Anniversary PDB (LF_STMEMBER EventDispatch, protected, const CGameEventDispatch[87]) ->
  ?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B (objdump of our obj).
- Handler names: slot i = EGameEventType i (Anniversary PDB enum, 88 enumerators, NO_GAME_EVENTS = 87); every slot's handler
  name is the event's EA name, present in BOTH Ego_r.pdb and FableWin.pdb name sets; sizes agree with the Ego_r donor bodies
  (retail = donor - 0..96 bytes); landed rows agree everywhere except 0x006294A0 (landed as CRenderManager2D_AddLineToDrawBuffer /
  Leaf HandleLeaveWorldPacket: it is CWorld::EAPlayerRespawn, slot 8 -> NAME_FIXES.tsv). Note: retail function order in
  0x0062A180..0x0062BEF0 is NOT the Ego_r donor order (zip by order is wrong there); the event enum resolves it. Full list LABELS.tsv.
- What made it match: the natural aggregate `{ &CWorld::EAx, &CWorld::LEAy-or-NULL, NULL, NULL }` per slot, 16-byte PMFs because
  engine/CGameEventDispatch.h (new overlay header) is included before CWorld is defined. First grade matched (probe), then the
  shared-header version matched. No flags. Headers: FABLE_RETAIL_CWORLD + _EVENTACTIONS + _EVENTDISPATCH (new),
  FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH (new), engine/CGameEventDispatch.h (new). All EVIDENCED (no FITTED spelling).
- vland: RELOCATION_MATCH PASS; one WARN RELOC_NAME (0x006294A0, see NAME_FIXES); leaf warning as for the landed 0x0121AF20 precedent
  (payload name carries "(object symbol _$E1 of its TU)").
- Coordinator: landed rows that model the table as `GameEventDispatchTable` / `g_GameEventHandlers` should be respelled to
  CMainGameComponent::EventDispatch (NAME_FIXES.tsv).

## 0x01221D20 CAIStateGroup_OpinionReactionRoot::`dynamic initializer for 'Behaviours'' -- RELOCATION_MATCH, test PASS, addmatch done
- Extent: `push ecx` .. 79 entries .. `push 0x0122A460 ; call _atexit ; add esp,4` .. `ret` 0x01223C55 (catalog 7990 right).
- Identity: Ego_r 0x01017780 / FableWin 0x040005B0 `dynamic initializer for 'CAIStateGroup_OpinionReactionRoot::Behaviours''.
  Element = COpinionReactionBehaviour (Ego_r 40 bytes: UpdateFunc, IsRunnableFunc, Name); member spelled from the Anniversary
  PDB (private static const [79]) -> ?Behaviours@CAIStateGroup_OpinionReactionRoot@@0QBVCOpinionReactionBehaviour@@B.
- Handler names from each entry's literal OPINION_REACTION_TYPE_<X> -> <X>Update / <X>IsRunnable, all 83 in Ego_r + FableWin as
  private bool; shared wife is-runnables resolved by donor order + size (WifeClothingIsRunnable 0x00985E90,
  WifeNotWhileJustMarriedIsRunnable 0x00985EF0). Entry 0 (NONE) has null pointers in retail although Ego_r has NoneUpdate.
- What made it match: the inline 3-argument constructor (FableWin signature) with the name passed as a BARE LITERAL converted to
  const CCharString& (explicit CCharString("...") = 7562 bytes: passes the ctor's eax, retail re-takes `lea eax,[esp+10h]`;
  aggregate form 6117; assignments in the ctor body 7246). VC7.1 aligns the 16-byte PMFs to 8 (sizeof 40 asserted). No flags.
- Headers: engine/COpinionReactionBehaviour.h (new), FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS (new),
  FABLE_RETAIL_CCHARSTRING (existing). All EVIDENCED. Test: standalone model (lane convention), mutant (one pointer added) FAILs.

## 0x0121F86F CAIBrain::`dynamic initializer for 'StateGroupInfo'' -- RELOCATION_MATCH, test PASS, addmatch done (HEADER DECISION NEEDED)
- Catalog name ParticleEngine_ModuleInit is wrong. Start 0x0121F86F proven (`push ebp ; mov ebp,esp` right after the landed
  LSocket::GlobalList initializer 0x0121F850); end `leave ; ret` 0x0122183C; 8142 bytes right.
- Identity: Ego_r 0x0101368D / FableWin 0x03FFAA4B `dynamic initializer for 'CAIBrain::StateGroupInfo''; atexit partner =
  landed 0x0122A20A (_$E2). Member from the Anniversary PDB: private static const CAIStateGroupInfo StateGroupInfo[] ->
  ?StateGroupInfo@CAIBrain@@0QBVCAIStateGroupInfo@@B (landed $E2 row spells it public non-const: NAME_FIXES).
- Per slot: GetName (string SG_...) + Alloc; class from the Alloc's vtable store -> RTTI (tools/allocrtti.py; follows a tail
  jump into the ctor and takes the first [esi] vtable store) = class of the landed GetName row for 191/202; 11 landed GetName rows
  are misnamed (GetActionName / CAIStateGroupNameTag ctor ...: NAME_FIXES). MinionHawkingFromStall by its Ego_r Alloc name + literal.
- What made it match: `#pragma optimize("s", on)` (EVIDENCED: same-TU landed _$E2 carries it; /O2 = 10723 bytes);
  CAIStateGroupInfo(X::GetName(), X::Alloc) per entry, sentinel CAIStateGroupInfo("", NULL) (CCharString("") also matches: tie);
  ~CAIStateGroupInfo() {} user-declared INLINE (an out-of-line dtor also grades RELOCATION_MATCH under q.py because call targets
  are masked, but then $E1 calls ??1 where retail calls ~CCharString: found by objdump -dr / vland RELOC_CONFLICT).
- Header debt: the 202 state-group classes are declared in the source with only `static CCharString FABLE_FASTCALL GetName();
  static CAIStateGroupBase* FABLE_FASTCALL Alloc();`. 131 have engine/<Class>.h, but those headers do not compose in one TU
  (probe/inc_all.cpp: VillageTaskTorch / VillageTaskHorn need CThingAICreature, KidReactToBlockage / SellToBuyer need
  CThingCreatureBase + CIntelligentPointer, MinionWorkInInn / MinionGuardHeardYellForHelp size asserts fail without opt-ins, 13+
  headers carry two class branches), so vland prints 131 SHARED_HEADER_AVAILABLE warnings (strict mode would reject). The landed
  same-TU sibling 0x0122A20A also models the table locally. Coordinator: accept, or schedule a header wave that makes the
  state-group headers co-includable (then add a GetName/Alloc opt-in to each and include them).

## 0x00C4F600 CEAXManager::SetEAXProperty -- parked DIFFER(9204v9204)
- Name: catalog PropertySerializer_GetPropertyTypeCode is wrong; CEAXManager::SetEAXProperty per the earlier chunk (landed QueueCommit
  0x00C51A20 declares and calls it; LABELS in that chunk). Extent 9204 = code to 0x00C5192C + `lea ecx,[ecx]` + 23-entry buffer case
  table 0x00C51930 + 26-entry listener table 0x00C5198C (the 'cmpsd' last instruction of the extent scan is table data).
- Residue (unchanged from H40*): ROOM / ROOMHF / ROOMLF reload `data` into ecx, edx, eax; retail edx, eax, ecx (ROOMHF loads
  `this` from [esp+10h] before the data in retail). Same length, same calls; one c2 rotation step. Before the jump table retail
  hoists `data` into ecx (0x00C51343) and still reloads it in these three cases.
- Tried this pass (fg.py, ATTEMPTS.log): swapped comparison operands (wrong cmp direction), per-case `long value`, `const long*`,
  static_cast + this->, k = 1..3 unused named locals (no effect at all), `bool changed` declared before the GUID ladder,
  #pragma optimize("s") (7921 bytes, wrong regime). Header debt: row-local CEAXManager / CEAXVoice, as all 15 landed CEAXManager
  rows (no engine/CEAXManager.h exists). Parked on sight per WF_RULES (pure register-pick, ~100 earlier grades).

- Successor pass (agentE12b, 7 fg calls / 13 variants, plus c2prio --trace / --rotation / --json probes; sources in tmp/c4/):
  MEASURED (C41 c2prio_f.py --rotation on differ source): in ROOM / ROOMHF / ROOMLF `data` and `this` are memory-resident in the
  global allocation; `this` comes from a 2-ref split piece coloured ecx, `data` from an expression temporary of the codegen
  rotation (0x10775acc, pointer cycles eax->ecx->edx, skips busy registers). Rotation events, ours: buffer tail line 230 size->ecx
  (ptr->edx), id->eax (edx busy with `voice`, ptr->ecx); ALLPARAMETERS memcmp result ->ebp (no scratch free, ptr stays ecx);
  ROOM ecx, ROOMHF edx, ROOMLF eax; ROOMROLLOFF (line 250) skips ecx (global `data` piece) -> edx, after which ours and retail are
  in the same state again (why nothing later differs). Retail = pointer at edx on entry to ROOM, i.e. EITHER one fewer rotation
  temp at the buffer tail (id or size loaded by a global piece instead of a temp: matches H40f's "passing 0 for id or size gives
  ROOM = edx") OR one more temp landing on ecx with no code (e.g. the voice call's `this` as a rotation temp instead of the
  ecx piece; or a duplicated NONE tail that cross-jumps away -- H40f's NONE direct-apply variant moved the pointer right but its
  `lea ecx,[ebx+74h]` cannot merge with the shared tail's `lea edx`). The id test at the trailing listener apply (0x00C51908) and
  the switch loads are global pieces (eax), the buffer-tail id is a rotation temp in ours.
  New dead ends (no movement, same 7 hunks unless noted): guiddef.h `IsEqualGUID` inline / `operator==` / `memcmp(...) == 0`
  for the GUID ladder; the landed same-TU ApplyVoiceBufferProperties 0x00C4EA30 body defined above the row; the landed
  ApplyListenerProperties 0x00C4E3C0 body above (alone and with the other); /Oa and /Ow (9136 bytes), /GS (no change); a named
  `bool deferred` (before `changed`, at the top, after `changed`: 9216-9220 bytes, other regime).
  Name debt found: this row declares `ApplyListenerProperties(const EAX30LISTENERPROPERTIES*)` (leaf ...PBU_EAX30LISTENERPROPERTIES@@@Z)
  while the landed callee 0x00C4E3C0 is `...PBU_EAXLISTENERPROPERTIES@@@Z` (local struct _EAXLISTENERPROPERTIES): one address, two
  spellings -- NAME_FIXES.tsv; the row text keeps the shared header's EAX30 type until the coordinator picks one.

## Header changes and regrades (agentE12)
- New: engine/CGameEventDispatch.h, engine/COpinionReactionBehaviour.h, engine/CAIStateGroupInfo.h. New opt-ins:
  FABLE_RETAIL_CWORLD_EVENTDISPATCH (CWorld.h), FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH (CMainGameComponent.h),
  FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS (CAIStateGroup_OpinionReactionRoot.h), FABLE_RETAIL_CAIBRAIN_STATEGROUPINFO
  (CAIBrain.h); moved asserts pinned with pin_asserts.py (see hdr/README.md).
- All three matched rows regraded after the last header change: RELOCATION_MATCH. wfcommon style rows that include a changed header:
  10 RELOCATION_MATCH under this overlay; 00601310 SRC_FAIL, 006096e0 DIFFER, 0061e6d0 DIFFER are the same with the four headers
  reverted (pre-existing: they need other batches' opt-ins), so the changes are neutral for them.

## Decisions for the coordinator
1. 0x0121F86F: accept the 202 local static-only state-group class declarations (131 SHARED_HEADER_AVAILABLE warnings; the headers
   do not compose in one TU), or schedule a header wave first.
2. Data-member spellings come from the Anniversary PDB only (access + const): ?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
   ?StateGroupInfo@CAIBrain@@0QBVCAIStateGroupInfo@@B, ?Behaviours@CAIStateGroup_OpinionReactionRoot@@0QBVCOpinionReactionBehaviour@@B
   (the dynamic-initializer names in Ego_r + FableWin prove the owner class and member name).
3. NAME_FIXES.tsv: 0x006294A0 EAPlayerRespawn, 0x013B9288 EventDispatch (landed GameEventDispatchTable / g_GameEventHandlers),
   11 misnamed GetName rows, 0x00984A20 / 0x009863E0 access spellings, 0x00985430 FollowFarUpdate, 0x0122A460 is the Behaviours
   atexit destructor (landed as CreateCRC), 0x013BB008 StateGroupInfo, 0x008FE93B inline user dtor copy.

- Third pass (agentE12c; sources tmp/c5/, probe script tools/rot.sh = c2prio --rotation filtered to the buffer FLAGS..listener
  DECAYTIME window with source text). Rotation listing line numbers are one LESS than the source line (both 'line 230' events are
  the ApplyVoiceBufferProperties call line). MEASURED LEVER: one extra expression temp laid out between the buffer tail and ROOM
  fixes all three cases with nothing else moving: listener NONE written as `if (!(id & DEFERRED)) ApplyListenerProperties(...);
  return 0;` (e2/e3/e4, also placed after ALLPARAMETERS f1) leaves ONLY its own unmerged copy (9220v9204, 4 hunks: switch head
  keeps id in edx for the copy's test, copy's `lea ecx,[ebx+74h]` v survivor's `lea edx`). A merged copy cannot give the step from
  ptr ecx (lea would have to land on edx: +2 steps -> ROOM eax), so the real source fact is some other no-code ecx temp or one fewer.
  Dead ends this pass (ROOM still ecx, same 7 hunks unless noted): listener cases in enum order (layout follows source, 8 hunks);
  deferred test as `id < DEFERRED`, `!(id >> 31)`, `(id & DEFERRED) == 0`, `if (changed) { if (id & DEF) {} else Apply }`,
  trailing `else ;`; typed EAX30LISTENERPROPERTIES* in ALLPARAMETERS; `default: break` (buffer / listener); ALLPARAMETERS with
  `changed = true` in both arms; memcpy intrinsic copy; `unsigned long deferred` at top (ROOM eax, other regime); a block-local
  `unsigned long property = id` for the call; `bool immediate` at top; block-local `bool immediate` (ROOM edx but materialised
  shr/not/test, 9216); listener apply copied into the buffer branch with `changed` uninit / init / with returns (9104, regime).
  More dead ends (agentE12c): top-level `unsigned long property = id & ~DEFERRED` for both switches (before / after the
  GUID ladder); a block-local `unsigned long property = id` for the deferred test and the call (copy-propagated, same 2 events);
  buffer NONE moved after EXCLUSIONPARAMETERS / last; equal-early-exits as `break` instead of `return 0` (buffer param cases,
  listener pans); `EAX30BUFFERPROPERTIES& properties = voice->m_eax3BufferProperties` (voice push becomes a 3rd temp) and
  `EAX30LISTENERPROPERTIES& listener = m_listenerProperties` (no change).
  MEASURED FACTS (tools/c41/c2prio_t.py = c2prio_f.py + raw tuple words per temp event): both call-line events are argument-push
  tuples (word 3 = 0x2004010d): size -> ecx and id -> eax; the deferred test's `mov eax,[id]; test eax,eax; js` is NOT a rotation
  temp (probe/r/t2.cpp: a lone `if (changed && !(id & DEF)) ApplyL(props)` emits that load with no event), so the id push's own
  load is the one removed afterwards as redundant. The deferred test and push therefore cost one rotation step that retail does
  not pay, OR retail pays one more no-code ecx step between the tail and ROOM; a merged explicit copy cannot supply it (above).
  The id split piece covering the tail (#20 of #129 of #67) is skipped with spill -4 (2 refs; its block has two entries: the bl
  test fall-through and the threaded `mov bl,1; jmp` of NONE / ALLPARAMETERS / OBSTRUCTION / OCCLUSION / EXCLUSION): if retail
  coloured that piece eax the push would need no temp -- no natural spelling found that makes it worth a register.
