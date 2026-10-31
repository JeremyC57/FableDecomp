# Hand chunk 632 (CAIStateGroup_MinionWorkInInn .. CModeBriarRoseConcentrate): 1 of 10 byte-exact

All ten rows were parked same-length retries (211-223 bytes). One became a match; the other nine stay same-length
DIFFERs. Six of them now carry PDB spellings and a smaller or equal residue.

## What matched (1)
- **CAIStateGroup_WorkInWarehouse::Cleanup 0x00943B40** (RELOCATION_MATCH, test PASS under g++ and VC). If the group still
  holds its PTask and the creature is a village member (interface 0x23), the creature is removed from the task through
  CTCVillageMember::GetPVillage()->RemoveThingFromTask(PTask, creature) and PTask is cleared. The SHAPE fix: the base
  CAIStateGroup_MoveGoodsBase::Cleanup 0x009162F0 runs on **every** path. Both retail early-outs `je 0x943C08` land on the
  base call. The parked source called it only inside the guard, so it got the wrong jump target and an ebx/ebp swap
  ("pure colouring" in the sweep's words). With the call at function scope, `this` stays live to the end and takes ebp.
  The model is fully respelled to the PDB: the base is CAIStateGroup_MoveGoodsBase (vtable 0x012898DC slot 9, not
  "CAIStateGroup_WorkBase"), the member is PTask : CIntelligentPointer<CVillageTaskCarryCrate> (Ego_r.pdb, +0x1C retail),
  GetPVillage is a CTCVillageMember member (not GFGetPVillageInterface), and LowerBound is the CVectorMap PDB spelling.
  The intelligent-pointer model is the one from the landed VillageTaskTorch::Cleanup 0x0093EA00. engine/CAIBrain.h and
  engine/CTCVillage.h are included through the layout-rename pattern, so vland reports no SHARED_HEADER warnings.

## Parked for Fable 5.1 (9)
- 0x0090BC60 CAIStateGroup_MinionWorkInInn::GetFreeBench DIFFER(211v211): `mov edx,[eax+4] ; mov eax,[edx+0Ch]` vs retail `mov eax,[eax+4] ; mov eax,[eax+0Ch]` (end()-local form). The natural for_each(begin(), end(), CAddFreeBenchToList(benches)) form instead schedules the 3 vector zero stores before the ObjectList node load | tried: real STLport list/_List_iterator/_Vector_base model plus the FableWin by-reference functor ctor (natural form, unchanged); vector before GetTC (stores before the call); begin+end locals (stores sink below all pushes); end local + begin from its node, with and without the owner local, and via ++ (1 register left) | next: an end()-local form where begin() does not reload the node, or a same-TU STLport list::begin/end body; otherwise lane gap: scheduler/register.
- 0x00AC7EB0 QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction DIFFER(215v215): `fld st1 ; fmul [ecx+78h] ; fmul st2` vs retail `fld st1 ; fmul st2 ; fmul [ecx+78h]`, plus the Dot x term `fld [eax+0Ch] ; fmul [edx+0Ch]` vs retail `fld [edx+0Ch] ; fmul [eax+0Ch]` | tried: plain t*t*w, an inline Sqr(), n*w*n, n*(n*w), /Op (319 bytes), o.X*X; VC7.1 reassociates every x*x*W spelling to (x*W)*x | next: a named `float normalCost = n*n;` DOES give retail's `fld st0 ; fmul st1 ; fmul W` but evaluates the n term before the t term. Combine it with t's in-place square (`t *= t; float cost = t*Wt; float n2 = n*n; cost += n2*Wn;`), which was not graded (attempt cap).
- 0x006B0B30 CTCPhysicsBase::InitialActivate DIFFER(217v217): PlaceInMapwho Y sum `fld [esi+10h] ; fadd [eax+4]` vs retail `fld [eax+4] ; fadd [esi+10h]` | tried: second operator+(C3DVector, C2DVector) with Y written position.Y + offset.Y (no change), one inline GetMapwhoPosition() helper for both calls (the X sum of the second call swaps instead), __forceinline helper returning a temporary (201 bytes), C3DVector(C2DVector + Position.Get2D(), Z) (237) | next: lane gap: x87 operand order of a repeated identical expression (the two retail blocks are byte-identical, ours differ).
- 0x00C590E0 CMixBuffer::WriteChannel DIFFER(217v217): output pointer in edi and source walk in esi (retail esi = output, edi = source) | tried: for-header increments, an inline Saturate16, a stride local, a named dst local + `src[0]`, a __forceinline member helper with (out, src) and (src, out) parameter orders | next: lane gap: register colouring of two equally weighted loop pointers.
- 0x00E7F330 NScript::CV_FishingCompetitionScript::Init DIFFER(218v218): `mov [esi-8],ebx` before `faddp` and the resize-temp store before `fstp` (retail one slot later each) | tried: three statement orders, the weights-first expression `pGlobalScriptDef->FishPrizeWeights[1] * 0.25f + (rand() % 50) * 0.1f` (gives retail's begin-pointer hoist before rand without the invented def local), resize(3) through the one-argument STLport inline, 0L / integer 0 | next: lane gap: x87/store scheduling.
- 0x009B9E00 CStringParser::SkipPastNextItemIfIs DIFFER(219v219): rotated loop end `mov edx,[esi+4] ; add edx,edi` vs retail `mov edx,edi ; add edx,[esi+4]` | tried: &String[StringPos(+n)] direct member reads (frame +4, worse), `String + StringPos + n`, from + n (201), String + (pos + n), a newPos local (213), from/to locals (201) | next: lane gap: operand order of a rematerialised address sum.
- 0x006A7FB0 CThingFilter_BoundingBoxOverlapsMap::operator() DIFFER(220v220): top-left coordinate copy homed at [esp+0Ch] (retail [esp+20h], shared with the thing box) and the converted map Y slots swapped | tried: the whole box as temporaries (222), by-value corners into mapBox's ctor (gives retail's Y slots, but both copies share [esp+4]), a by-value C2DCoordI box ctor (226), block-scoped copies with a default box (264), the thing box as a temporary, the thing box declared first (222) | next: lane gap: frame-slot packing.
- 0x005E1210 std::_Adjust_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*,int,...> DIFFER(221v221): esi/edi swapped throughout | tried (this chunk): the real STLport 4.6 __adjust_heap body as a std template with explicit instantiation, both `*(first + n)` and `first[n]` spellings, engine/CIVCountedPointeeBase.h under FABLE_RETAIL_DEF_CLASS_BASE. Its Release() (`return --IVRefCount`) gives `mov edx,[ecx+4] ; dec edx ; ...` (the known unsigned-refcount block); the direct `--IVRefCount == 0` gives `dec [ecx+4] ; jne` in operator= but not in the value's dtor (225 bytes; see probe/005e1210_a.cpp) | next: give the template form the parked file's pointee Release() member (`if (--IVRefCount == 0) Delete();`) to get back to 221, then try the register levers; otherwise lane gap: register colouring.
- 0x008E7F80 NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter DIFFER(223v223): second-loop index in edi and element in ebp (retail ebp / edi) | tried: the PDB const method (QBE), count_if + not1(CIsImposterHit()) (proven by FableWin.pdb), CIsImposterHit with a user copy ctor (removes the empty-byte copy; without it, 240), WorldRandomRange inline (keeps the `test ebx,ebx` range guard), a named unary_negate and a hand loop | next: lane gap: register colouring (same residue as the sweep's 'index declared at function scope' and 'named element pointer').

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x0090BC60 catalog "CleanBench_PickRandomMarker" = Ego_r.pdb ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ (the landed caller CleanBench 0x0090BD40 already binds it).
- 0x00C590E0 catalog "_gapscan::sub_00c590e0" = CMixBuffer::WriteChannel (descriptive, GameCODA; the landed override CLimitedMixBuffer::WriteChannel 0x00C591C0 spells the slot). The parked hand505 file called it WriteChannel16.
- 0x009162F0 (the Cleanup base call) is CAIStateGroup_MoveGoodsBase::Cleanup (RTTI vtable 0x012898DC slot 9). The parked 0x00943B40 file called it CAIStateGroup_WorkBase::Cleanup.
- 0x008E8A10 is CIsImposterHit::operator() const (anonymous namespace; Ego_r + FableWin), not "CAIStateGroup_VersusScorpionKing::FindChargeEvent" (manifest) and not "CThingFilter_IsNotInMode" (landed count_if rows).
- 0x00AC7EB0's PDB owner is QuadricOptimiserInternals::COptimisedPrimitive, and the method is private and non-const (AAE); the parked file had it const in the global namespace.
- 0x006A7FB0 is `operator()(const CThing&) const` (QBE_NABVCThing@@), and its callee 0x006AF640 is `CTCPhysicsBase::GetBoundingInfo(C3DVector&, float&) const`. The parked file took pointers.
- 0x008E7F80 GetRandomImposter is const (QBE) in Ego_r.pdb.

## Names: proven vs descriptive
- Proven (PDB name set + donor order/callers): GetFreeBench, WorkInWarehouse::Cleanup and every callee of that row,
  CV_FishingCompetitionScript::Init, SkipPastNextItemIfIs, ComputeSelfContraction, InitialActivate, the
  BoundingBoxOverlapsMap operator(), _Adjust_heap (template args; VC7.1 cannot mangle the donor's `$$Q` rvalue-reference
  value parameter, retail passes it by value), GetRandomImposter.
- Descriptive: CMixBuffer::WriteChannel (GameCODA), GetNextPrizeWeight 0x00E7F7D0 (landed, descriptive),
  CThingAICreature +0x170 PWorkplace (as the landed Update 0x0090BED0).
- pGlobalScriptDef 0x0143E90C keeps the landed siblings' `CScriptDef*` spelling. Ego_r.pdb has a dynamic initializer
  and an atexit destructor for it, so its real type is probably CDefPointer<CScriptDef> (one binding spells it
  `NGlobalScriptDefs::pGlobalScriptDef` as a CDefPointer). Review at landing.

## Header debt
- engine/CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE): Release() returning `--IVRefCount` (a mutable
  unsigned long) compiles to `mov edx,[ecx+4] ; dec edx ; mov eax,edx ; test eax,eax ; mov [ecx+4],edx`. Retail's
  inline release is `dec [ecx+4] ; jne` everywhere in this row. A `void`-returning release with the test inside
  (`if (--IVRefCount == 0) Delete();`) or a direct member test is needed. The header also does not include
  rebuild_abi.h in its retail branch (FABLE_STATIC_ASSERT is undefined unless the includer pulls it in first).
- engine/CStringParser.h, CAIStateGroupBase.h, COptimisedPrimitive.h and CTCPhysicsBase.h are Ego_r debug layouts or
  flatten the vptr. The rows use local models, as their landed siblings do.
