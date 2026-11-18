# egod3a notes (round 3)

## Tools written here (all inside this directory)
- g.sh <addr> <src> "<idea>" [q.py args]: preflight + q.py grade + relocation-masked diff (tmp/d_<addr>.txt) and frame-masked diff
  (tmp/dm_<addr>.txt), one ATTEMPTS.log line. `OV=<dir>` grades against a probe header overlay instead of hdr/.
- tools/sched.sh (copy of the chunk-892 c2 list-scheduler trace, w22/agent892/tools/sched, read-only use) + tools/regs.py: prints
  the scheduling regions (81-tuple cap; `0x269` = the no-code PAREN / conversion tuple; `_epop` = epilogue pops).
- tools/fws.py: plain /Od /Gy compile, bytes / instructions per COMDAT next to FableWin.exe.signatures.tsv sizes.
- tools/regr.sh: regrade copies of LANDED rows (q.py, their Flags / Leaf) under a probe overlay with a prepended define: checks
  that a helper text is consistent with every landed user.

## FableWin size evidence (applies to every row)
FableWin.exe is a VC10 /Od build (its STL COMDATs are VC10 `_Vector_const_iterator`, `_Iterator012`; CRT `@sqrtf@4` shows /Gr).
VC10 pushes a float argument read from memory as `push ecx ; fld [m] ; fstp [esp]` where VC7.1 writes `mov eax,[m] ; push eax`:
+1 instruction / +3 bytes per such push. With that correction: GFFastSqrt `return sqrt(value);` 19b/8i -> 22b/9i = FableWin;
sqrt(float) under /Gr 14b/7i -> 17b/8i = FableWin; GetMagnitude2D `float squared=X*X+Y*Y; return GFFastSqrt(squared);` 49b/20i ->
52b/21i = FableWin (the landed running-sum text is 53b/21i -> 56b/22i, not FableWin); GetMagnitude `float squared=<sum>; return
GFFastSqrt(squared);` 63b/25i -> 66b/26i = FableWin. Raw VC7.1 sizes compared with FableWin without this correction mislead. CAVEAT (advisor): under /Gr a call to sqrtf also loses its
`add esp,4` (-1i/-3b), so `float squared=...; return sqrtf(squared);` lands on the same 52b/21i / 66b/26i: the sizes prove the
"one named sum" shape, not that the callee is GFFastSqrt. Measure with FW_EXTRA=/Gr, then add the push correction.

## 0x00A6C640 CPhysicsMesh::CalculateBoundingInfo -- RELOCATION_MATCH PASS (vland strict), src/ + test/
- Name: ?CalculateBoundingInfo@CPhysicsMesh@@AAEXXZ (private; FableWin.pdb + Ego_d, as egod2a found).
- Residue was `pop edi` after `fsqrt` (0x00A6C839). Measured with the c2 scheduler trace (tools/sched.sh): the tail block is
  exactly 81 IL tuples in the old draft, so it fills one scheduling region and the epilogue pops are not scheduled with it
  (`fstp` then `pop edi`). One FEWER tuple (80: e.g. GetMagnitude through `float squared=...; return GFFastSqrt(squared);`
  without the sum parentheses) pulls the epilogue into the region and `pop edi` floats up to 0x00A6C7B7. One or two MORE tuples
  (82/83) cut a second region [.. fstp, _epop, _epop] and give retail's `fsqrt ; pop edi ; fstp ; pop esi`.
- Closed by GetMagnitude2D = `float squared=(X*X+Y*Y); return GFFastSqrt(squared);` (new opt-in, hdr/README.md): the FableWin
  size of GetMagnitude2D after the VC10 float-push correction (see the FableWin section above); the parentheses add the one
  PAREN tuple (FITTED). Same result with GetMagnitude also in that form `float squared=(X*X+Y*Y+Z*Z); return GFFastSqrt(squared);`
  (FableWin 66b/26i after correction), not used (GFFastSqrt is not declared on GetMagnitude's default path).
- Other +1 tuple spellings that also match but were rejected: GFFastSqrt `return (sqrt(value));` (breaks landed 0x00803D60),
  GetMagnitude `return (GFFastSqrt(GetSquaredMagnitude()));` (no FableWin support, unnatural).
- COORDINATOR: one text per helper. Under the new GetMagnitude2D body (and also under the unparenthesised FableWin-sized text)
  landed 0x007F5AE0, 0x00952890 and 0x009711D0 grade DIFFER; they match with the running-sum default (which is NOT FableWin's
  size after the VC10 correction). Every other landed GFFastSqrt / GetMagnitude2D user (12 rows) is unchanged. Decide whether
  this row may carry its own GetMagnitude2D text; the alternative is a row-level no-code tuple, none found.
- Test: links the row object; each of the six compares supplies a different bound, last point carries two extremes (loop bound),
  sphere / circle centres and radii against a double model, plus a one-point mesh.

## 0x00A229E0 CRightHandedSet::CRightHandedSet(const C3DAngles&) -- RELOCATION_MATCH PASS (vland strict), src/ + test/
- Residue (egod2a): the first Normalise's `mov esi,17Ch` hoisted to 0x00A22ADF. Scheduler trace: our second region (81 tuples)
  began at the E-entry `fsubr` (0x00A22ADB) and held the whole first exponent computation, so the constant load filled its first
  free slot. Retail's order (`shr ecx ; and ecx ; mov ecx,[table] ; mov esi,17Ch ; sub esi,edx`) is that computation split by a
  region cut right after `and edx,0FFh`: 9 tuples earlier.
- Closed by writing each of the nine rotation entries as one parenthesised expression (`rotation.E11 = (vx2 + cos_xz - vx2 * cos_xz);`):
  nine no-code PAREN tuples, exactly the 9 needed. Row-level, no header change; same opt-ins as the egod2a draft. One grade.
- Test: builds the 128-entry GFastOneOverSqrtTable itself, compares Up / Forward with a double Rodrigues model for five angle
  sets (all angles non-zero with mixed signs, plus XZ = 0). Window: exactly 9 parenthesised entries (8 or 10 DIFFER).
- Mutants (tools/mut.py, row object + test): E21 `+ stvz` -> `- stvz` FAIL, cos_up_xy sign FAIL; row 1: dropped Y-min compare
  FAIL, loop bound size()-1 FAIL.

## 0x00BE2CD0 CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables -- OPEN, DIFFER(1091v1088), differ/00be2cd0.cpp
- Residue (341 diff lines, one cascade): retail materialises const 1 in edi at 0x00BE2D8C for the ofs-table stores (stored in
  array order, 0 from ebx) and keeps it to the y-loop prologue `and edx,edi` (0x00BE2E42; re-set `mov edi,1` at the y latch
  0x00BE30C3, `xor ebx,ebx` there too); poly_within_quad is never in a register ([esp+20h]: 0x00BE2F74 cmp, 0x00BE2FAD,
  0x00BE309D); the tri-list cursor is &tri_list[poly_count*3+2] (0x00BE2E4E `lea esi,[edi+esi*2+4]`); vert of i=0 is ebx itself.
  Ours (c2prio, tmp/prio_be2.txt): const 1 split to eax for the table only (stores grouped by value), poly_within_quad split edi
  #61 in the y prologue, cursor base +1 element, `mov bx,di` / `mov bp,di` copies; the three CArray zero stores use a fresh
  `xor edx,edx`.
- Tried this round (no change at all): FABLE_RETAIL_STLP_VECTOR (_M_fill_insert body is already visible), implicit
  `poly_verts[i] = vert;`. 0x005149F0 = vector<unsigned short>::_M_fill_insert (reloc_bindings).
- Next: the cursor base (+2 v +1) is the one non-colouring clue: find the source shape that makes c2's strength reduction pick
  index *3+2 (e.g. a different poly_count update point or index expression), then re-read c2prio for const 1 v poly_within_quad.

## 0x00803280 CTCTargetingPlayer::UpdateTargetThreatening -- RELOCATION_MATCH PASS (vland strict), src/ + test/
- The old draft was a local model; rewritten on the shared headers with the define / include set of the LANDED same-TU sibling
  CTCTargetingPlayer::UpdateProjectileTargeting 0x00803D60, plus FABLE_RETAIL_CTCTARGETINGPLAYER_FRAMEUPDATE (declares the row),
  CTCHEROOPINIONDEEDLOG, CTCWEAPON(+_READYPROJECTILE, _LOADING), and two hdr additions (NCreatureAI IsWillingToAttackTarget opt-in,
  new EOpinionPostDeedType.h). No Flags (the sibling's /d2inlT68 is its own).
- Ego_d locals verbatim and scoped: ptargeted_creature { ptc_deedlog, ptc_carrying (const) { pcarried_thing, ptc_weapon (const)
  { threaten_deed_type } } }; one-argument GetTC(&ptc_deedlog) (non-const LowerBound 0x0040F020) and PeekTC(&ptc_carrying /
  &ptc_weapon) (const LowerBound 0x004365B0); deed OPINION_DEED_TYPE_EXPRESSION_THREATEN_LARGE (41) / _SMALL (40);
  `PLastThreatenedThing = GetPTargetedThing();` (ctor 0x004A96F0 + operator= 0x004A9760 called in the deep branch) and
  `PLastThreatenedThing = 0;` (the same inlines expanded in the shallow else branch: frequency decides).
- What closed the last 55 diff lines (3 grades): the three fastcall tests take `*ParentThing` / `ParentThing` directly (retail
  re-reads [esi+4] after each GetPTargetedThing() call); through the inline GetParentThing() VC7.1 evaluated the parent first and
  kept it in edi.
- vland WARN (expected): RELOC_NAME 0x008645E0 labelled 'CMainGameComponent::BeginInputLoading' is a propagated wrong label; the
  function is NCreatureAI::IsAwareOfThing (NAME_FIXES.tsv). The type test is `GetType() == 1` (THING_TYPE_AI_CREATURE in Ego_d's
  EThingType; no repo header carries that enum yet).
- Test: fake thing / player / component map / vtable slot 33; every condition both ways, deed 41 / 40 / 40, PostDeed arguments,
  the cleared pointer. Mutants (drop `!` on AreThingsEnemies, drop the loaded test, drop ZTargeting) all FAIL.

## 0x0059FEE3 CTCInventoryTrade::AddBuyItemToList -- OPEN, DIFFER(1552v1556), differ/0059fee3.cpp, ESCALATE (tmp/ESCALATE_0059fee3.md)
- c2prio (tmp/prio_59f.txt): `this` prio 17 refs 5 -> ebx for the whole body; ptc_inventory prio 13 split (its 0 stored to
  [ebp-8], reloaded on the miss path); const 0 immediate. Retail: ptc_inventory (=0) in ebx from `xor ebx,ebx`, `this` homed at
  [ebp-0Ch] then esi from 0x0059FF54.
- Tried this round: PeekTC(CTCInventoryItem::GetInventoryType(item_def), &ptc_inventory) without the inventory_type local (Ego_d
  lists none): byte-identical to the HasTC / GetTC spelling, so not the lever.

## Rows not ported this round (old local-model drafts; differ/ holds them with a Grade line)
- 0x0091C220 (needs a new CAIStateGroup_NymphAttack header), 0x00935C40 (new CAIStateGroup_MinionPutOutFires header),
  0x0068EF50 (no CInputProcessUseRangedWeapon header), 0x0076CCE0 (CTCEnemy.h + landed siblings 0x0076D1B0 / 0x0076D270 /
  0x0076D2F0 are the port base), 0x00BC7660 (landed sibling RenderSubPrimitive 0x00BC6DB0 is on 15 shared headers; Ego_d:
  primitive is CEngineInternalPrimitive2DText&, alpha_mask_info, z, { graphic_frame }), 0x00490494, 0x0092D1F0 (landed siblings
  0x0092D6F0 / 0x0092C740 / 0x0092DA20 on shared headers). Triage grades of the drafts as they sit: ATTEMPTS.log.
- Method that matched 0x00803280 in 3 grades and should transfer: copy the landed same-TU sibling's define / include block
  verbatim, write the body with Ego_d's locals in scope, then read the first diff.
- (row 4, ruler) one extra reference to ptc_inventory right after the lookup (FABLE_RETAIL_ASSERT dead arm, with and without
  FABLE_RETAIL_ASSERT_DEAD_ARM): identical bytes, so the colouring gap is not ptc_inventory's reference count. Probe only, not
  in the differ source.
- (row 9 gate) neither the sibling 0x00BC6DB0 nor the twin 0x00B9E950 pulls the font-frame / CGraphicFrame / GetTexture inlines
  from shared headers (CGraphicFrame.h / CGraphicInfoFrame.h / C2DAlphaMaskInfo.h exist; no CEngineInternalPrimitive2DText
  header): a port needs those inline bodies first. Not started for budget.
