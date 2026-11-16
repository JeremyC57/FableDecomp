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

## 0x0121F86F CAIBrain::`dynamic initializer for 'StateGroupInfo'' -- RELOCATION_MATCH, test PASS, addmatch done, STRICT-HEADER vland PASS (fix-up 2026-11-15)
- FIX-UP (strict headers, 2026-11-15): the 131 state groups with engine/<Class>.h are now #included (0 SHARED_HEADER_AVAILABLE,
  0 HEADER_POLICY, 0 RELOC_IDENTITY under LANDVERIFY_STRICT_HEADERS=1; RELOCATION_MATCH PASS; vt.py PASS; addmatch redone).
  Composition measured (cl /Zs of one TU including all 131): with no extra opt-ins 7 headers fail, all from 3 causes, none a clash
  between two state-group headers: (a) size asserts of classes deriving from CAIStateGroupBase (MinionWorkInInn.h:16,
  MinionGuardHeardYellForHelp.h:20, VillageTaskTorch.h:23, VillageTaskHorn.h:23, KidReactToBlockage.h:19, OnHeroPush.h:16) because
  the default CAIStateGroupBase is not the 0x14-byte retail model; (b) EScriptAIPriority undeclared (VillageTaskTorch.h:8,
  VillageTaskHorn.h:11; declared only in the retail CAIStateGroupBase path); (c) CThingCreatureBase undeclared
  (KidReactToBlockage.h:17, SellToBuyer.h:21). FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS + FABLE_RETAIL_CINTELLIGENTPOINTER and a
  `class CThingCreatureBase;` forward declaration in the row make all 131 compose with 0 errors.
  GetName/Alloc: only 5 headers declared both (MinionWorkInInn, MinionGuardHeardYellForHelp, VillageTaskTorch, KidReactToBlockage,
  BeetleToggleHover); 126 overlay copies gained the additive opt-in FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC (decls only, default
  path only; 598 asserts pinned; 3 headers with no default-path class get a statics-only class under the opt-in; hdr/README.md).
  The 71 state groups without a shared header keep the local statics-only declarations (no header exists: no warning).
  Landed precedent checked: the same-TU sibling 0x0122A20A uses a local CCharString / CAIStateGroupInfo model and descriptive free
  allocator names (pre-strict; not reusable: its names are not the PDB ones). 01221d20 (includes the changed OpinionReactionRoot
  overlay) regraded: RELOCATION_MATCH, vt PASS. Backups: tmp/0121f86f.before_fixup.cpp, tmp/hdr.before_fixup/, tmp/payload.before_fixup.json.
  Remaining non-blocking vland WARNs (pre-existing): 11 RELOC_NAME = misnamed landed GetName rows (NAME_FIXES), _$E2 RELOC_NAME /
  RELOC_CONFLICT (TU-local symbol shared across initialiser rows).
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

- Fourth pass (agentE12d, 2026-11-15; 64 graded variants via fg.py, sources tmp/c6/, helper scripts tools/ins.py g.sh gh.sh od.sh;
  residue unchanged, source unchanged). Re-measured with the WF_RULES last-resort devices in mind:
  * device d (dead release asserts, overlay fable_assert.h + FABLE_RETAIL_ASSERT_DEAD_ARM): FABLE_RETAIL_ASSERT(voice) at the buffer
    branch, (data) at the listener branch / top, (size >= sizeof(long)) before the tail, (voice && data), (id) after the apply,
    (id != 0) inside a braced tail, (propertySet && data && size) at the top, memcmp(&id,&id,4)==0 / memcmp(&size,..) (address-of
    in the dead arm): ALL byte-identical to the baseline. The dead arm is gone before codegen, there are no class temporaries
    (no conditional-temporary bits), and the residue is a codegen-order rotation count, not a record-number tie. Not applicable.
  * device c (k unused named locals): k = 4, 5, 6, 7 at the top of the body (k = 1..3 earlier): byte-identical. Not applicable.
  * device b (inline control): not an inline-decision residue (equal calls, no callee expanded / called differently).
  * other levels, all no movement (same 7 hunks) unless noted: const by-value parameters in the definition (id / data,size /
    all); the QFE-4035 toolset (FABLE_VC71=$S/wine/vc71); /G5 /G6 /GB /Ob1 (/G7: 9356 other regime); listener `default:` placed
    before ROOM or first, buffer `default:` first; ROOM-only `long& room` / listener reference / old-value local (r3: 30 hunks);
    tail spellings the front end folds to the same IL (statement ternary, inverted `if (!changed || deferred) ; else`, *&id,
    *&size, `(id & DEF) ? id : id`, `propertySetType == 2 ? id : 0`, two independent outer ifs (w1: 9044 regime), the tail
    outside `if (propertySetType == 2)`); `(long)id >= 0` tests (9396: a zero register appears, `cmp [id],ebp`); callee
    declared with `long id` (identical); pushing `id & ~DEF` (9216) / `id | DEF` (+or, same rotation); a switch-value local
    pushed instead of id (block: 9216; top: own frame slot, 0xB00 frame); id address escaping (`return (long)&id` in default:
    layout moves, ROOM still ecx: address-taken id does not CSE the test load with the push); inline wrappers CommitVoice /
    CommitListener (9308 / 27 hunks / identical), a template SetEAXValue setter for the single-value cases (8420..8696), the
    buffer switch (+tail) as an inline member (not expanded: 8044; forced: 9024 / 8992). Diagnostics: removing listener NONE
    or ALLPARAMETERS leaves ROOM at ecx (confirms nothing between the tail and ROOM takes a rotation step).
  * Measured model (unchanged): after the buffer-tail id push the pointer is at ecx regardless of history (that push skips the
    busy edx = voice), so retail must either push id with no rotation temp (a CSE of the deferred-test load: no spelling found
    survives the FE as a shared expression) or take one more no-code ecx step before ROOM (only an explicit listener NONE apply
    copy does it, and it stays unmerged, 9220). Equivalent alternative found this pass: same pointer as ours but ecx busy when
    ROOM's data temp is allocated (ROOM edx, ROOMHF eax, ROOMLF ecx, ROLLOFF edx all follow) -- no spelling makes ROOM's
    `this` piece live first. Near miss; parked.

- Fifth pass (agentE12e, 2026-11-15; 15 graded variants via fg.py + 1 rotation probe, sources tmp/c7/; residue and source unchanged):
  * NEW MEASUREMENT (p1): writing the three integer cases in source order ROOMLF, ROOM, ROOMHF gives exactly retail's registers
    (ROOMLF ecx, ROOM edx, ROOMHF eax; everything after unchanged) but the layout follows the source (4 hunks: ROOMLF's block
    first). So retail's state equals "ROOMLF codegen'd first, laid out third" or the known one-step model; c2 does not move
    case blocks (also measured: ROOMROLLOFF written as a duplicated decay-time body, at the end / before FLAGS / in place, is
    neither cross-jumped nor moved: 9220..9264, 28-35 hunks -- the retail ROLLOFF->DECAYTIME fall-through is a real source
    fall-through). Other permutations: HF,LF,R 7 hunks; R,LF,HF 5 hunks.
  * Dead ends (7 hunks, byte-identical unless noted): redundant id guards in the buffer tail (`&& !(id & DEF) && !(id & DEF)`
    folds; `!(id & DEF) && changed && !(id & DEF)` 9 hunks, test order moves, ROOM still ecx; `(long)id >= 0 &&` 9404);
    `propertySetType` / `changed` declared before the preset table (3 orders, identical: no symbol-number tie).
  * Verdict unchanged: one rotation step (the buffer-tail id push temp) with no natural spelling found; parked.

- Sixth pass (agentE12f, 2026-11-15; 10 fg calls / 25 graded variants, sources tmp/c8/; residue and source unchanged, 7 hunks):
  * Re-derived the rotation model from retail: the buffer tail forces size->ecx (pointer must be at ecx before the tail; edx is
    voice), so ROOM = edx needs the tail's id push to take NO rotation temp (codegen reuse of the test's eax). Measured (c2prio
    --trace): id has 5 refs, prio -267, split; its tail pieces are a 1-ref piece (dropped) and a 2-ref piece that is copy-forwarded
    (not listed), so the push reads memory and takes a temp in ours. ALLPARAMETERS' memcmp temp wanting edx would skip to ebp too,
    so it is consistent with retail.
  * Dead ends, all byte-identical (7 hunks) unless noted: a local copy of id in the buffer tail (in a block / before the if / at
    the top: copy-propagated away); `unsigned long propertyId = id` used for every id reference (after `changed` / before the
    preset table / listener only: copy-propagated); `changed = true` set before the listener switch with NONE / ALLPARAMETERS /
    pans just breaking (identical; buffer side: 9060 other regime); ROOM/HF/LF data read as *(int*) / *(unsigned long*) / int in
    the compare only; listener `default: break` (and both switches) and the pan equal-returns as `break`; deferred test as
    `!(id >> 31)` (identical), `id < DEF` (buffer only: cmp/jae, rotation unchanged; both 9208); the GUID ladder as a static
    inline helper GetPropertySetType returning the type (9012: the preset-table stores no longer interleave with the first
    memcmp -- wrong regime).
  * No PDB / Anniversary evidence for this function's locals (fablewin / egor name sets have no CEAXManager entries).
  * Verdict unchanged: near miss, one codegen rotation step; parked.

- Seventh pass (agentE12g, 2026-11-15; 11 graded variants via fg.py, c2prio --trace / --rotation / --json probes; sources tmp/c9/,
  probe script tools/pr.sh = trace id-pieces + rotation window; residue and source unchanged, 7 hunks):
  * MEASURED (c2prio --trace): `id` (#67) is allowed only ebx/ebp (live across the ApplyVoiceBufferProperties call), prio -267,
    split; the tail piece (#20 of #129) has 2 refs (the deferred test in one block, the push in the next) and spill -4 = 4 for
    the refs minus the parameter home-store charge (C2_ALLOCATOR_PROBE.md section 4), so a 2-ref parameter piece can never be
    coloured; 3 refs would still need a single entry. The json refs list (tmp/c9/base.json) shows the test (line 230, block 2)
    and the push (line 231, block 3) in different blocks, so no CSE of the load either. Retail's id push with no rotation temp
    therefore needs the pushed value in a global candidate (CSE temp / coloured piece) -- no source spelling found makes one.
  * CONFIRMED the model from the other side: `(id & DEF) != DEF` (9228, 5 hunks) and `(~id & DEF)` (9212, 6 hunks) for the two
    deferred tests turn the buffer-tail test into a rotation temp (ecx), after which size -> eax and id -> ecx (reload) leave
    the pointer at edx and ROOM / ROOMHF / ROOMLF come out exactly as retail (those hunks vanish); only the tail and the final
    test differ. `!((long)id < 0)` = zero-register regime (175 hunks).
  * Dead ends (byte-identical, 7 hunks): `const unsigned long& property = id` / `unsigned long* property = &id` in the buffer tail
    (the FE folds both back to id); flags /Oi- /Zi /Ob0; /Op 8564 and /Oy- 8420 (other regimes).
  * Devices: c (k unused locals) and d (dead asserts) were measured byte-identical in the fourth pass and the trace confirms why
    (the residue is not a record-number tie but a parameter piece's negative spill benefit); b (inline control) has no inline
    decision to act on. Verdict: near miss, parked.

- Eighth pass (agentE12h, 2026-11-15; 9 graded variants via fg.py + 3 c2 probes; sources tmp/c10/, probe tool
  tools/c41/c2prio_g.py = c2prio_f.py + a hook at the entry of the temp-assigner 0x10775acc that prints each request's tuple
  opcode / operand kind (REQ lines in --rotation output); residue and source unchanged, 7 hunks):
  * NEW MEASUREMENT (c2.dll 0x10775acc read statically + REQ probe): a temporary skips the rotation only when (a) its
    descriptor already has a register (+0x30, hint path 0x107500dd), (b) tuple opcode 1 with a REGISTER operand (kind 1,
    copy coalescing 0x107214dc), or (c) opcode 0x12 on a parameter operand (kind 5) whose +0x34 register link is set
    (0x1077ae58 -> 0x1077ae86 pre-assigns that register). The buffer-tail size and id pushes are both opcode 1 with a MEMORY
    operand (kind 2) -> rotation; the deferred-test load is not a 0x10775acc request at all. So retail's id push with no
    rotation step needs id in a register piece at the push (route b), consistent with the seventh pass (a 2-ref parameter
    piece with spill -4 is never coloured); there is no codegen memory-value cache a spelling could hit.
  * Dead ends (byte-identical, 7 hunks): the buffer `if` and its call on one source line, also the final listener `if`
    (no per-line effect); listener ALLPARAMETERS as an early `if (!memcmp) { changed = true; break; }`, a named
    `bool same`, a typed `EAX30LISTENERPROPERTIES*` local. `if (a != b) changed = true; a = b;` for the single-value
    cases is another regime (ROOM/HF/LF only 9068, listener 8884, all 8900: branchy IL, every register moves); retail
    is `changed = a != b`.
  * Micro-TU check (probe/m1): with low register pressure id is a coloured piece and is pushed straight from the test
    register (`push eax`, no request), the shape retail shows; in this row id is `split, memory` (allowed ebx/ebp only,
    live across the voice call), which is what costs the extra rotation step.
  * Verdict: near miss, parked. Untested lever left: make id's tail piece colourable (more references in the test/push
    blocks, or a live range not crossing a call so eax/ecx/edx are allowed) without changing the emitted code.

- Ninth pass (agentE12i, 2026-11-15; sources tmp/c11/, new probe tools/rr.sh = q.py grade + the data-load register of
  listener ROOM / ROOMHF / ROOMLF, retail `edx eax ecx`, ours `ecx edx eax`; residue and source unchanged, 7 hunks):
  * NEW (diagnostics): the voice call's object is not a rotation factor -- calling through a global pointer, through
    `(CEAXManager*)voice`, or a static member (no `this` at all) leaves ROOM at ecx (x1-x3). An out-of-line (not
    inlined) helper call in listener ALLPARAMETERS moves ROOM to edx (its pushed-argument temp, l1/l2 9176) but the same
    helper forced inline is byte-identical to the baseline (l4/l5). A bool-returning inline deferred test
    (`static inline bool IsDeferred(id)`, i4/h1-h3) gives retail's ROOM registers but codes the test as
    `mov ecx,[id]; shr ecx,1Fh; test cl,1; jne` (9212..9220); unsigned-long / int returning helpers fold to the baseline.
  * Re-derived constraint (from retail's float cases, which alternate ecx / eax with edx = voice busy): the pointer is
    at ecx entering the buffer tail in retail too, so retail's `push eax` for id took NO rotation temp (eax busy with a
    coloured id value, which also explains size -> ecx); any invisible temp between the tail and ROOM would have to land on
    ecx with no code, and NONE / ALLPARAMETERS / the dispatch have no such request (a NONE explicit apply always costs
    either a visible `lea ecx` or two steps).
  * Dead ends (byte-identical 7 hunks unless noted): `unsigned long deferred = id & DEF` / `bool commit` locals (9220-9228
    other regime); buffer tail as `if (!changed) return 0; if (!(id & DEF)) ...` (3 forms); `switch (unsigned long
    property = id & ~DEF)` (listener / buffer / both); member-declaration order, reversed GUID externs, the class's full
    member list declared; `unsigned char` / `char` / `signed char` changed; identity push arguments `+id`, `id * 1`,
    `id >> 0`, `id - 0`, `id ^ 0` (all folded); `(long)id >= 0` final test only / both (9204 but zero-register regime,
    174 hunks), buffer only 9396; uninitialised `changed` with explicit `else return 0` paths; listener ALLPARAMETERS
    before NONE; `else if ((pst & 0x3f0000) && pst == 0x10000)` / `if ((pst & 6) && pst == 2)`; listener ALLPARAMETERS
    copy-only / memcpy intrinsic / memcmp-only (diag, ROOM still ecx); id-not-live-across-the-call tails (diag, other regime).
  * NEW ASSET: a 7-second micro reproducer, probe/m3/m3.cpp + tools/mrun.sh (prints id's allocation, the rotation temps on
    the call line and the tail code). m3 (12 long + 8 float buffer cases with a memcmp ALLPARAMETERS, the same for the
    listener, one trailing `if (changed && !(id & DEF)) B(props)`) emits EXACTLY ours: `mov eax,[id]; test eax,eax; js;
    mov ecx,[size]; push ecx; mov ecx,[this]; push ebp; push eax; push 2; push edx`, two rotation temps (size ecx, id
    eax), id `split, memory`, data ebp, this split into ecx pieces. With ONE buffer float case (probe/m3/f1.cpp) c2 colours
    id's final 2-ref piece eax (trace: the last split leaves a 2-ref piece, prio 10, allowed ac.si.., chosen) and pushes
    id from the test register with no temp -- the retail shape. In m3 / the row the last split leaves a piece whose
    priority / spill stay negative (row: #20 prio -14, spill -4; m3: spill -46), i.e. the split piece is not tight around
    the test + push blocks. Pieces flip-flop as the case temps take eax (linear block ranges), and the listener switch value
    temp (eax) is what forces the last split in m3. When the buffer-path id web is disconnected from the listener one (diag
    v5: buffer returns after both applies) c2 builds a separate 3-ref web for it. So the closer, if any, is a code-neutral
    source fact that keeps id's tail piece tight (or gives the buffer path its own id web); every tail spelling tried in the
    micro folds to the same IL (v1-v4). Successor: iterate on the micro (7 s) before spending row grades.

- Tenth pass (agentE12j, 2026-11-15; ~36 row grades through the new tools/rq.sh = q.py + ROOM/HF/LF data registers + masked
  hunk count (baseline 8 hunks: 2 jump tables, 1 case-table data run, the 5 ROOM/ROOMHF/ROOMLF hunks), 6 flag grades, 6 micro
  probes (probe/m4), 1 rotation probe; sources tmp/c12/; residue and source unchanged):
  * Re-confirmed the model from the rotation listing (tools/rot.sh): tail events size ecx->ecx, id edx->eax (skip voice), so the
    pointer leaves the tail at ecx; retail's ROOM=edx needs exactly one fewer step (id pushed from a coloured piece).
  * NEW micro facts (probe/m4, m3 = exact reproducer): all buffer float cases made integer (mA) or the 8 float cases moved FIRST
    in source (mB) -> only ONE rotation temp at the call (id pushed from a register piece), i.e. what breaks the id piece is
    the fnstsw-ax float blocks ADJACENT (in linear/source order) to the tail; an integer case between them and the tail (n3,
    the row's FLAGS position) still reproduces ours. Removing the two constant-`changed = true` cases (n7, threaded straight
    to the id test) colours id esi for the whole web (another regime): the threaded entries into the id-test block are the
    other factor. A `goto commit` label inside `if (changed)` for those cases (n9) is identical to ours. Retail's case order
    equals its layout (c2 does not move case blocks), so neither lever is available to a byte-identical source.
  * Dead ends (row, 8 hunks unless noted): `long result = 0` accumulator returned everywhere (top / before the table); `int` /
    `long` changed (9208 regime); `0x7fffffff` mask literal; enum-typed / int / long casts of both switch values (named
    DSPROPERTY_EAX30_BUFFERPROPERTY / _LISTENERPROPERTY); float cases as if/else assigning changed, `!(a == b)`, ternary,
    `*(float*)data != a` (64 hunks), a float `value` local (fld/fstp: 9144 / 9172, ROOM right but x87 code wrong);
    `changed & !(...)` (9212 / 9052 / 8720); `changed != false &&`; listener-first inverted outer if (9088); redundant range
    guards before either switch (`> FLAGS` return / `<= FLAGS` wrap / named property + guard: all folded);
    `changed = memcmp(...) != 0; if (!changed) return 0;` in buffer ALLPARAMETERS / `changed = !(equal...)` in the parameter-
    block cases (setne bl / test bl code, 9204-9216: those cases must stay constant `changed = true`); flags /Ox /J
    /Gd /Ob1 (identical), /Oa /Ob1 and /Ow /Oa (9136), /arch:SSE (10044).
  * Devices: c / d byte-neutral (fourth pass) and b has no inline decision; no fitted spelling found. Parked again.

- Eleventh pass (agentE12k, 2026-11-16; 19 row grades (18 fg.py through tools/h.sh + 1 q.py) = fg.py + hunks outside the preset-init run and the
  case tables; sources tmp/s1-3, n1, n4, n6-8, p1; residue and source unchanged):
  * Case tables checked BY TARGET (tools/tbl.py: obj dir32 relocations vs retail words, 23 + 26 = 49 entries, run on the
    differ source's object): all equal. Listener NONE ->
    0x00C51908 (the id test after the `mov ebx,[esp+10h]` reload), buffer NONE -> 0x00C5107B (`mov bl,1 ; jmp 0xc512ee`)
    in both builds, so the residue is not hidden structure behind the masked tables; it is the 3-case register rotation only.
  * `switch (propertySetType) { case 2: ... }` instead of `if (propertySetType == 2)`: listener side byte-identical (7 hunks),
    buffer side codes `sub eax,2` (8 hunks), rotation unmoved.
  * The NONE lever re-measured (replicates agentE12c e2-e4 / f1 in the third-pass notes below; n1 = NONE `if (!(id & DEF)) ApplyListenerProperties(&m_listenerProperties); return 0;`,
    n8 = the same after ALLPARAMETERS): 9220 bytes but only 4 hunks, ROOM / ROOMHF / ROOMLF all equal to retail. The merge
    blocker is precise: the switch head and the NONE test share an id register piece (`mov edx,[id] ; mov eax,edx ; and eax`
    v retail `mov eax,[id] ; and eax`), so NONE codes `test edx,edx ; js ; lea ecx,[ebx+74h] ; push ecx` and only the last
    `mov ecx,ebx ; call` is cross-jumped into the tail (retail tail block: `mov eax,[id] ; test eax,eax ; js ; lea edx,[ebx+74h]
    ; push edx`). For a full cross-jump NONE would need a fresh id load and lea edx, but its lea temp at ecx is exactly the extra
    rotation step ROOM needs (unverified guess: a merged copy would have to take two temps, eax then edx past a busy ecx).
    NONE copy moved LAST (n7): the tail is duplicated in place (13 hunks); copies in NONE + ALLPARAMETERS (n4): ALLPARAMETERS
    reloads id fresh (ecx) and pushes the memcmp lea CSE (25 hunks); `(long)id >= 0` NONE test (n6): zero-register regime.
  * `propertySetType = 0` initialiser and no final else (p1): 9208, preset-table store order changes.
  * Breaking NONE's shared id: the test through `const unsigned long* pid = &id` (n12) is byte-identical to n1 (folded); a top-level `const bool deferred` (n9,
    9240) and one `property = id & ~DEF` local for both switches (n10 with the NONE copy 9212, n11 without 9192: computed at
    the top, not sunk) are other regimes. No code-neutral NONE copy found (inference, not measured: retail likely had none).
  * Devices: c / d byte-neutral (fourth pass), no inline decision (b), no FITTED spelling found. Parked again (near_miss).

- Twelfth pass (agentE12l, 2026-11-16; 13 row grades + 4 c2 probes; sources tmp/s12/; residue and source unchanged):
  * Harness check: baseline 9204v9204 / 8 hunks / ROOM=ecx,edx,eax; the known mover (listener NONE explicit apply, n1)
    still gives 9220 / 4 hunks / ROOM=edx,eax,ecx, so "no movement" below is real.
  * Diagnostic, INCONCLUSIVE: tools/c41/c2prio_h.py = c2prio_f.py whose C2_FORCEPAIR hook also writes the candidate's
    spill (+0x40) from C2_FORCESPILL. Forcing id's tail piece #20 (prio 40, spill 20): c2 reports `#20 -> edx` with allowed
    set `..dsi.p`, but voice holds edx over 146..230 and the object is byte-identical to the baseline (tail code and the two
    line-230 rotation events unchanged); #20 is "forwarded, not listed", so the forced colouring never reached the push.
    This is NOT a test of model A (what an eax id piece would do stays unmeasured); why eax / ecx are missing from the
    allowed set is also unmeasured (propertySetType eax over 125..233 is a guess). Measured: diagnostic d1 (listener
    tests removed, so propertySetType ends at the buffer head) leaves the tail piece at `spill -4` and the rotation unchanged.
  * Unproven signature (no PDB entry for the class) re-measured: id `unsigned int` / `long` / `int`, size `unsigned int`
    (t1-t4, leaves ...@@IPAU.. / ..PAXI@Z / ..JPAU.. / ..HPAU..PAXI@Z): all 9204, 8 hunks, ROOM=ecx,edx,eax.
  * Flags not tried before: /EHsc (the audio TU's landed siblings carry it), /GX, /Ob2, /EHsc /Ob2: byte-identical to the
    baseline build (diff of the whole listing = 0 lines).
  * Verdict: near miss, parked. Neither model (A: id pushed from a coloured piece; D: one extra no-code ecx temp) has a
    byte-identical spelling after 283 grades; the coordinator may accept the row as near_miss or leave it for a c2 splitter study (where the piece
    boundaries of #67 come from).
  * Lead left (inference, not measured): besides a coloured id piece (model A), a no-new-code route is the
    temp assigner's non-rotation paths (eighth pass): the id push would skip the rotation if id's descriptor still held
    eax from the deferred-test load when the push block is generated (hint path a), or if eax/ecx/edx were all busy so
    the best-score path picked eax without moving the pointer (as the ALLPARAMETERS memcmp temp does: ebp, pointer
    unchanged). A successor should measure in probe/m3 whether the test-load register is still recorded at the push
    (REQ probe c2prio_g.py) before writing any more spellings.

- Thirteenth pass (agentE12m, 2026-11-16; 17 row grades through tools/rq.sh + 2 c2 probes (rot.sh, pr.sh) + one static c2.dll read;
  sources tmp/s13/; residue and source unchanged: 9204v9204, 8 hunks, ROOM=ecx,edx,eax):
  * Re-measured on today's headers: rotation listing FLAGS edx->eax (ptr ecx), tail size ecx->ecx, id edx->eax, ALLPARAMETERS
    ecx->ebp (ptr stays), ROOM ecx; trace: id #67 prio -267 split/memory, tail piece #20 2-ref "forwarded, not listed", later
    skipped with spill -4 (micro f1: the same 2-ref piece re-listed with prio 10 -> eax). Model A (id pushed from a coloured
    piece) stands; the only source facts that ever moved it are IL-shape facts (entries into the id-test block, float blocks
    near the tail), none code-neutral.
  * NEW (x1): EXCLUSIONPARAMETERS falling through into a shared `case NONE: changed = true; break;` (one fewer IL block / entry
    into the id test; retail's NONE target 0x00C5107B IS EXCLUSIONPARAMETERS' `mov bl,1 ; jmp`, so the layout would be the same)
    = 9504, 28 hunks: voice/data global pieces move (edx/ebp reloaded per case). Confirms entry structure is a strong RA lever,
    but this shape is not retail. x2 (NONE as its own case after EXCLUSIONPARAMETERS): byte-identical.
  * Dead ends (byte-identical, 8 hunks unless noted): both callees declared `void` (v1/v2); `changed |=` in ROOM/HF/LF (v3);
    redundant `&& changed` after the id test (t4); buffer apply moved to the `pst & 6` level (z1); explicit `else return 0` for
    pst != 2 (z2); block-local `unsigned long deferred` (d1) / block-local id copy inside `if (changed)` (d2).
    Also identical: listener ALLPARAMETERS with `changed = true` before the memcmp (a1); buffer NONE after ALLPARAMETERS (a2).
    Other code: `(long)(id & DEF) >= 0` and `id <= 0x7fffffffUL` tests (9208, 10 hunks); `changed = true` before the stores
    in the three parameter-group cases (14 hunks, `mov bl,1` moves, ROOM unchanged).
  * Static c2.dll read (tmp/c2re/prio.s): the candidate's spill (+0x40) gains w(b)*cost per reference at 0x1078267D and LOSES
    w(b)*cost (0x1078002F of the block) for a tuple of opcode 0x26A at 0x10782807 (also sets flag +5 bit 0x4) -- the likely
    "parameter home store" charge that keeps a 2-ref id piece at spill -4. Not followed further (which tuple is 0x26A and how a
    split piece inherits it is unmeasured): the lead for a c2 study, not a spelling.
  * Evidence checks: the Anniversary 360 PDB has no EAX / CEAXManager / CODA strings (no third source); the unlanded getter twin
    0x00C4F470 (same GUID ladder, edx = type, -4 on no match) calls GetVoiceBufferProperty(voice, 2, id, data, size) out of
    line, i.e. the setter's buffer part was likely written inline here (an inlined helper was measured as another regime).
  * Devices: c (k locals) / d (dead asserts) remain byte-neutral (no tuples, no class temporaries); b has no inline decision.
    Verdict: near miss, parked (300 grades in total).

## Header changes and regrades (agentE12)
- New: engine/CGameEventDispatch.h, engine/COpinionReactionBehaviour.h, engine/CAIStateGroupInfo.h. New opt-ins:
  FABLE_RETAIL_CWORLD_EVENTDISPATCH (CWorld.h), FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH (CMainGameComponent.h),
  FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS (CAIStateGroup_OpinionReactionRoot.h), FABLE_RETAIL_CAIBRAIN_STATEGROUPINFO
  (CAIBrain.h); moved asserts pinned with pin_asserts.py (see hdr/README.md).
- All three matched rows regraded after the last header change: RELOCATION_MATCH. wfcommon style rows that include a changed header:
  10 RELOCATION_MATCH under this overlay; 00601310 SRC_FAIL, 006096e0 DIFFER, 0061e6d0 DIFFER are the same with the four headers
  reverted (pre-existing: they need other batches' opt-ins), so the changes are neutral for them.

## Decisions for the coordinator
1. 0x0121F86F: RESOLVED by the 2026-11-15 fix-up (shared headers included; strict-header vland PASS). Coordinator: land the 126
   overlay header copies (opt-in FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC) together with the row.
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

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).

- Fourteenth pass (agentE12n, 2026-11-16; 6 row grades (rq.sh) + 1 c2prio --rotation probe; sources tmp/e12n/; source unchanged):
  * Harness check: baseline 9204 / 8 hunks / ROOM=ecx,edx,eax; the known mover tmp/s12/n1.cpp (listener NONE explicit apply)
    still 9220 / 4 hunks / ROOM=edx,eax,ecx, so the no-movement results below are real.
  * Rotation events between the last agreeing temp and ROOM (tools/rot.sh on the baseline, unchanged from earlier passes):
    line 230 size push wanted ecx got ecx; line 230 id push wanted edx got eax (voice holds edx); line 239 ALLPARAMETERS memcmp
    result wanted ecx got ebp (pointer not advanced); line 244 ROOM wants ecx. Retail needs edx at 244: model A (no id-push
    temp) or model D (one extra no-code temp landing on ecx before 244).
  * NEW level, sibling evidence for model A: every landed EAX setter of the audio TUs that tests the deferred bit
    (CEAXListener::SetContextProperty 0x00C683C0, CEAXBuffer SetSourceProperty 0x00C68510, CEAXListener SetEffectProperty
    0x00C68F90; grep of rebuild/src/compiled/00/c4..c9 and wfcommon/style_* for 0x7FFFFFFF / 0x80000000 / DEFERRED) ends in
    `if (result >= 0 && !(property & 0x80000000)) result = Commit...();` -- none forwards the tested id to a call, so no matched
    row supports (or refutes) an id push without a rotation temp. Model A stays unsupported; lesson c5 (a memory parameter
    push always takes a temp) stands.
  * Model D via a merged NONE copy is excluded by rotation arithmetic: a NONE copy byte-identical to the tail block needs its
    lea temp on edx; with the pointer at ecx that happens only by a busy-ecx skip, which leaves the pointer at eax (ROOM = eax,
    wrong). n1 works only because its lea takes ecx (unmergeable `lea ecx`). Consistent with the eleventh-pass guess.
  * `changed` type: `unsigned char` byte-identical (8 hunks); `int` / `long` other regime (9208, 232 hunks).
  * Verdict: two level changes (sibling evidence, changed type) moved nothing; parked again as near_miss (306 grades total).
