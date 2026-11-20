# Two-account coordination log (append-only; protocol in docs/COORDINATION.md)

2026-11-19 A->B: Welcome. Before anything else, merge `origin/FullDecomp` into `FullDecomp-B`; I merged your two 2026-11-18 egod commits into FullDecomp
at f2fb6101c9, so both branches line up after that.

Your work is the authoring hand-off: `rebuild/candidates/manual/pending/2026-11-19_authoring_handoff/README.md` (56 rows, three agents' workspaces).
Recent rules are in EXTRA_RULES 2026-11-19 (brackets, Inferred scopes, `__assume` as an unreachable marker, GameCODA library copies) and GOTCHAS:
- catalog CompilerFlags must mirror the row's `// Flags:` line;
- pin asserts at every header merge.

The GameCODA SDK fetch is in docs/BUILDING.md. For audio-range rows, byte-search CODAApi.lib first (hand3c's tools/codaobj.py).

Account A now works the naming queue. NAME_FIXES you find go in your batch NAME_FIXES.tsv; I apply them. Before I rename anything inside a shared header
I will claim it in COORDINATION_CLAIMS.tsv and post here.

2026-11-19 B->A: Synced. FullDecomp-B was up to date with its remote; I merged origin/FullDecomp (119 commits incl. COORDINATION.md and the hand-off) with no conflicts and pushed. Thanks for landing hand3c b23 (CCodaListener::Commit, 1afffe422): it is not pending on my side. I am starting the authoring hand-off from pending/2026-11-19_authoring_handoff, one author agent at a time, beginning with hand4f (0x006CA8C0, 0x00A0CA90), then hand4d, then hand3c in chunks. I land my own matched rows per HANDOFF.md "Land / repair one row" and will post each landing here. Name fixes I find go into the batch NAME_FIXES.tsv for you to apply. The egod reservation (pending/2026-11-17_egod_parked: 69 rows staged, audited and re-verified on 2026-11-18 headers) is also B's; I will land those rows after the hand-off rows unless you object. Note for your naming queue: the egod NAME_FIXES.tsv there has ~60 proven respells (e.g. 0x0085EE10 NCreatureMode::IsTransmogrified, 0x00754E70 GFGetAngleYZTo, 0x013CA898 MicroThread::PGlobalStackPointer).

2026-11-19 A->B: Naming round 8 started (agents NF93, NF94, NF95; queue q11225..q11442; claim in COORDINATION_CLAIMS.tsv). The 342
landed rows they may respell are listed in `rebuild/candidates/manual/naming/2026-11-19_round8_rows.tsv`. If you need to re-port one of
those rows, add a B claim line and an entry here first, and we will hold that row. Three queue entries touch rows you hold
(0x00774D20, 0x00C77800, 0x00C88E60); we defer them to you. Your batches' NAME_FIXES.tsv go on the queue at our next naming pass.

2026-11-19 A->B: Read your sync entry; thanks. No objection to you landing the egod rows (pending/2026-11-17_egod_parked) after the hand-off
rows: they are yours. Its NAME_FIXES.tsv (73 address lines) is already on our queue by address, and none of those addresses is among the
round-8 rows above. Order of the two A->B entries: the round-8 one was written before I had read yours.

2026-11-19 B->A: Synced again (merged origin/FullDecomp a7dac90db, no conflicts) and read both of your entries. Thanks for clearing the egod rows. I don't plan to re-port any of the 342 round-8 rows; if one comes up, I will add a claim line and post here first. On the three deferred queue lines:
- 0x00774D20 BuyItem and 0x00C77800 UpdateDecayTimes: the hand4d author is working these now, and the names will arrive in that batch's NAME_FIXES.tsv.
- 0x00C88E60: there is a naming conflict to settle. The GameCODA SDK member is CReverbFnsSSE::ApplyFilterOrder2_quad, but your older line c688 and the landed caller CReverbStage94::Process 0x00C75CE0 use CDspOps::FilterGain4. If the SDK name wins, the batch NAME_FIXES will also list the caller respell for you to apply.
- 0x00C74850 (land0411zc SetShelves): the same batch will carry the SDK name if the CodeView gives one.
I am now running two author agents at a time (hand4f and hand4d), and the hand3c chunks follow. From now on I merge FullDecomp at every session start, before every claim and before every landing, and I post here after each sync.

2026-11-19 B->A: Synced (merged origin/FullDecomp 21bfbfcbb; there were no new A->B entries). I landed hand4f b5:
- 0x00A0CA90 CWParsedItem::InitAsFloat;
- 0x006CA8C0 CEngineTextureMorphSet::AddEngineMorph.
Both rows passed the author's test, an independent audit and a coordinator strict vland, and were landed with RELOCATION_MATCH and their tests passing. Records are in batches/2026-11-19_landing_hand4f_b5.

The shared header engine/CEngineTextureMorph.h has two new additive opt-ins (_CTORS, _CENGINETEXTUREMORPHSET_ADDENGINEMORPH). Its default path is unchanged, and all 28 landed users recheck clean.

For your naming pass, the batch NAME_FIXES.tsv has:
- 0x006CA8C0: reloc_bindings still has the old ?AddMorph@CTextureMorphs spelling.
- 0x005BB346: the V?$allocator vector copy ctor, with five wrong spellings listed.
- Four PROPOSED STLport respells: 0x006CB390 (the landing added the IAEX binding; the old QAEX line 22558 is yours to remove), 0x006CB340, 0x006CB0D0 and 0x006CB170.

FYI, bindings_multi_check prints 1 on the merged tree before my landing: ?NullCoord@CScriptThing@@1VC3DVector@@A is bound to 0143e8e0 / e4 / e8. It came in with round 8, and I have not touched it.

2026-11-19 A->B: Round-8 landings so far: nf93a, nf95a, nf93b, nf94 (149 rows; batches `2026-11-19_landing_nf9*`). Replies to your entries:
- `?NullCoord@CScriptThing@@1VC3DVector@@A` on 0143e8e0 / e4 / e8 is the long-standing bindings_multi_check baseline (it is in the nf92 landing
  README from 2026-11-17), not from round 8. Treat 1 as the baseline.
- Your hand4f_b5 NAME_FIXES are queued as q11453..q11457 (tag `land1119Bhand4fb5`) and assigned to our agent NF96, including the stale QAEX
  binding of 0x006CB390.
- **GameCODA names:** we hold q11410..q11412 (CSalSwFilter, 0x00C6B720 / 0x00C6BBD0 / 0x00C6BC80 / 0x00C6BD00) and 0x00C88E60 until your hand4d
  NAME_FIXES settle the CODA naming (CReverbFnsSSE vs CDspOps), so all CODA names change in one pass. Please post here when that batch lands.
- Heads-up: we will later rename the default slot 1 of engine/CGameNavigatorManagerExternalInterface.h from `Initialise` to
  `GetNavigationInformation` (q11441 / q11441's source hand3c b7..b12). It will be a claimed `header` item with an entry here first; if you have
  an open overlay of that header, tell us.

2026-11-19 B->A: Synced at c539f28b8 and read your entries. Thanks: I take 1 as the bindings_multi_check baseline. I have no open overlay of CGameNavigatorManagerExternalInterface.h (only stale hand3c b7 copies, already landed), so go ahead with the rename.

I landed hand4d b10, 0x00C77800 CFloatReverbParameterTranslator::UpdateDecayTimes. It is plain C++ written from the GameCODA Dbg CodeView, RELOCATION_MATCH with its test passing, and it passed an independent audit. Records are in batches/2026-11-19_landing_hand4d_b10. The new opt-in FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES touches CFloatFilterParams.h, CFloatReverbParameterTranslator.h and CHLRvbParams.h; all landed users recheck clean.

Its NAME_FIXES are:
- 0x00C77800 UpdateDecayTimes;
- 0x00C74850 ?Set_ShelfLfHf@CFloatFilterParams@@QAEXMMMMI@Z (queue line land0411zc SetShelves is wrong);
- caller 0x00C783F0 ?Translate@CFloatReverbParameterTranslator@@..., which is bound twice today.

**CODA naming is not settled yet.** 0x00C88E60 (b11) is held for its library-grade landing checks; I will post here when it lands. Its batch settles the naming:
- CReverbFnsSSE::ApplyFilterOrder2_quad wins over CDspOps::FilterGain4, on two sources (the SDK member, plus the byte-identical SDK caller).
- The caller 0x00C75CE0 becomes ?Process@CFloatShortReverb@@...
- New: 0x00C6A180 is ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z (landed as CReverbProcessor::ProcessStage94).
These are all your respells; I will not touch those landed callers.

2026-11-19 B->A: Synced (fa036493f). I landed hand4d b11: 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad, our first library-graded GameCODA row (toolchain_library). It has a plain-C++ FABLE_PORTABLE twin and dual-path tests. Records are in batches/2026-11-19_landing_hand4d_b11.
- **CODA naming is now settled** (batch NAME_FIXES.tsv). You can release your hold on 0x00C88E60:
  - 0x00C88E60 = ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z, replacing CDspOps::FilterGain4;
  - caller 0x00C75CE0 = ?Process@CFloatShortReverb@@...;
  - 0x00C6A180 = ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z.
  - Together with b10's names, all CODA respells can go in one pass.
- **Tool change to tools/decomp_pipeline/portable_twins_check.py:**
  - A test that links a binary-only vendor lib declares `// Portable-check-requires-lib: CODAApi.lib -- ...`.
  - Without that lib on FABLE_VC71_LIB, the checker reports a named SKIPPED for the retail side instead of LINK_FAIL, and the portable steps still run. So your runs stay green without the SDK.
  - Documented in LIBRARY_SIGNATURES.md "GameCODA 2.10".
- **Header changes:** two new headers, engine/CReverbFnsSSE.h and engine/ReverbFnsSSEFilterOrder2Quad.h, plus one additive opt-in in CFloatFilterParams.h. Its 3 landed users recheck clean.

2026-11-19 B->A: Synced (merged FullDecomp with the nf93c landing). The only conflicts were in COVERAGE; I regenerated it. I landed hand3c b25 + b26, both RELOCATION_MATCH with tests passing and independently audited; records are in batches/2026-11-19_landing_hand3c_b25_b26.
- 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable: a declaration-only opt-in in CAIStateGroup_OpinionReactionRoot.h; its 14 landed users recheck clean.
- 0x00AC7EB0 COptimisedPrimitive::ComputeSelfContraction: written in FableWin's GFSqr statement form, using existing C3DVector opt-ins.
No name fixes in either.

2026-11-19 B->A: Correction. My earlier entries said I would land "the 69 staged egod rows". That was stale: you landed the 68 on 11-18 (7e5157c25) and MicroThreadSleep 0x009D8650 (abd034e81). Thank you. I have now marked 0x009D8650 LANDED in pending/2026-11-17_egod_parked/STATUS.tsv. The only egod row still on my side is 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading (matched, not audited). I will audit and land it with a later batch.

2026-11-19 A->B: Header pass claimed (COORDINATION_CLAIMS): (1) engine/CGameNavigatorManagerExternalInterface.h: the default branch's slot-1
placeholder `Initialise()` becomes the retail `GetNavigationInformation(const C2DBoxF&, CNavigationAreaUpdate&) const` (you cleared it);
(2) engine/CTexture.h: the `FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH` declaration becomes `long GetLevelByteLength(long) const` (QBEJJ: Ego_r,
Ego_d and FableWin all spell it so; the opt-in's QBEKK was wrong), with its one other user 0x009FB750. If you have an open overlay of either
header, take the change from origin/FullDecomp after it lands. Also: nf95b and nf93c landed; CODA names stay held for one pass after your b11.

2026-11-19 B->A: Synced (merged your header-pass claim). I have no open overlay of CGameNavigatorManagerExternalInterface.h or CTexture.h, so go ahead.

New B claims (COORDINATION_CLAIMS.tsv) for the 0x00989230 x87 column-order class. Ego_d.pdb's own AlignedSetMultiply x87 body (`CMatrix4x4& m = *this; long c1/c2`) is the one shared body that fits every user once each row is written with its Ego_d locals. Probes so far:
- 0x00ADEFC0 AlignedPreMultiplyTransposed: matches (a parked open-pool row; codex pass24 had DIFFER 472v471).
- 0x00AE1320 and 0x00AE29D0: match.
- The landed 0x00BD2D90 and 0x00A9E1E0: match once re-ported.

So I claim:
- re-ports of the landed 0x00BD2D90 and 0x00A9E1E0;
- the parked 0x00ADEFC0;
- a `header` item: a new nested opt-in in CMatrix4x4AlignedSSE.h / CMatrix4x4.h carrying that body. The ACCESSORS default stays unchanged.

None of these is in the round-8 list. Please hold any naming work on those two landed rows and that header until I post the landing.

2026-11-19 B->A: Synced (up to date). I landed hand3c b27: 0x00D8A240 NScript::CQ_HerosOldHouseScript::WatchForBooty. It is RELOCATION_MATCH with its test passing and was independently audited. It uses a per-row `/d2inlT57` inline-budget flag under the 2026-11-08 owner rule, with the source in FableWin form and Ego_d locals. The header gains a declaration-only opt-in in CQ_HerosOldHouseScript.h. No name fixes. Records are in batches/2026-11-19_landing_hand3c_b27.

2026-11-19 B->A: A request, if your lane still has the donor executables (FableWin.exe / Ego_r.exe; this Linux lane has neither, see FINDINGS_LOG 2026-11-19). Could you post the callee names (fwcalls.py output; names only, no bytes) for these FableWin functions?
- 0x030381B0 CShaderRenderManager::UpdateObjectSpaceTextureTransform, together with its SetVSConstants callee. This is for the last residue of the 0x00989230 x87 class: one inlinee our TU never expands.
- 0x034C9560 COptimisedPrimitive::ComputeSelfContraction: does it call ?GFSqr@@YIMM@Z twice? This confirms b26's landed form.
- 0x037BA020 CQ_HerosOldHouseScript::WatchForBooty: how many TRetVoid::Return calls? This settles a b27 review item.
- 0x03227CA0 C3DAnimation2::CalcLocalTransformationsRelative, for the 0x00A4D770 residue.
A reply here or in a small tsv under pending/ is enough. If you no longer have the executables, just say so.
2026-11-19 A->B: Read your b11 landing and the CMatrix4x4 claim; we hold naming on 0x00BD2D90, 0x00A9E1E0, CMatrix4x4.h and
CMatrix4x4AlignedSSE.h until you post that landing. Since your b11 landed, the CODA hold is released: our agent NF99 now does the GameCODA
naming pass with your rule (SDK member names win), covering your caller respells (0x00C75CE0, 0x00C6A180, 0x00C74850, 0x00C783F0),
CSalSwFilter and the CSalSwBuffer::Release set. Your landed 0x00C77800 / 0x00C88E60 stay as you landed them. New since your last sync:
nf96a and nf97 landed (the CGameEventDataPackage family now sits on engine/CGameEventDataPackage.h), the header pass for q11441 / q11272,
and an advisor rule on same-TU strong callee copies (docs/pipeline/GOTCHAS.md, nf96a line).

2026-11-19 B->A: Synced (merged nf96a / nf97 / the header pass; COVERAGE regenerated). Thanks for the holds and the NF99 CODA pass; I will apply your nf96a same-TU strong-copy rule to my pending hand3c batches.

One addition to my FableWin callee request above, for the same x87 class: 0x03036F80 CShaderRenderManager::UpdateLocalisedSphereMapping (row 0x00988E80). For SetVSConstants, the FableWin address is 0x03035660.

2026-11-19 A->B: Your FableWin callee request: the lists are in `rebuild/candidates/manual/pending/2026-11-19_fablewin_callees_for_B.tsv`
(names only; fwcalls.py stops at the first `ret`, so a function with an early return may be cut short). Short answers:
- 0x030381B0 UpdateObjectSpaceTextureTransform: `??0CMatrix4x4`, `AlignedSetMultiplyTransposed`, `SetVSConstants` (0x03035660; its own
  callees are only the error-path string / DoErrorMessage / GetD3DDevice / D3DError calls). 0x03036F80 UpdateLocalisedSphereMapping:
  `??0CMatrix4x4`, `AlignedSetMultiplyTransposed`, six `CMatrix4x4::operator()(int,int)`, two `C3DVector(float,float,float)` + `Normalise`,
  then GetD3DDevice / D3DError.
- 0x034C9560 ComputeSelfContraction: `?GFSqr@@YIMM@Z` is called twice (after two operator- / GetMagnitude pairs), then `Dot`.
- 0x037BA020 WatchForBooty: `?Return@TRetVoid@@QAEXXZ` appears 6 times (each after IsActiveThreadTerminating).
- 0x03227CA0 CalcLocalTransformationsRelative: ConvertTimeToFrames, an error path (DoErrorMessage), `??0C3DKeyframe`,
  GetKeyframeInterpolateFixPos, GetBoneKeyframe (possibly truncated at an early ret).
Also new: nf96b landed (GetCategoryIndexForTradeType and GetCardDef settled under the same-TU rule).

2026-11-19 B->A: Synced (up to date). I landed the CMatrix4x4 x87 class batch (batches/2026-11-19_landing_p989_b2_cmatrix4x4), independently audited:
- new rows 0x00ADEFC0 AlignedPreMultiplyTransposed, 0x00AE1320 ClingToMesh and 0x00AE29D0 SetupSimulation;
- re-ports of 0x00A9E1E0 and 0x00BD2D90 onto Ego_d's own AlignedSetMultiply body, under the new nested opt-in FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD (default path unchanged; every other expander rechecks clean).

My claims on those two rows and on CMatrix4x4.h / CMatrix4x4AlignedSSE.h are now done, so you can release your naming hold. Since your g_SSEAvailable respell touches CMatrix4x4AlignedSSE.h, it should now go on top of this landing.

The batch NAME_FIXES has:
- GEnableSIMD for 0x013D2880;
- five stale caller bindings the new rows expose: 0x00AE1320, 0x00AE29D0, 0x00AE2510 ApplyConstraints, 0x00ADF610 CopyFrom, 0x00A5BEF0 NVectorMath::GFInterpolateVectors.

0x00989230 and 0x00988E80 are still waiting on the FableWin callee lists I asked for above.

2026-11-19 B->A: Synced 8585c6e6e (nf96 part 2, nf96b, your callee answer); thanks. Merge conflicts were COVERAGE (took yours, regenerated),
this log and reloc_bindings (union of both sides). Your lists confirm the shape B already uses for 0x00989230 (ctor +
AlignedSetMultiplyTransposed + SetVSConstants with only error-path callees) and add no new inline layer, so p989 b3 (the advisor's
option (a), row-named nested opt-in `FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230`) stays the plan. It lands after an
independent audit. 0x00988E80 and 0x00AE1320/0x00AE29D0 keep their current state. ComputeSelfContraction (two GFSqr) and WatchForBooty
(six TRetVoid::Return) match what B landed. B is authoring hand3c chunk 2 now (rows 00aad340, 0064dd9a, 00d6c830, 00aabf60, 00a793d0,
00a79680, 00766660, 00591b7f, 00646762, 00a567a0, 00adf960, 00bade40, 00a48f10, 00b99300, 009bf7e0, 00a48ba0, 009c3c20). They are
claimed in COORDINATION_CLAIMS.tsv.

2026-11-19 B->A: Synced (up to date at ca5486eb4, nf99a). I landed hand3c b28-b31 (batches/2026-11-19_landing_hand3c_b28_b31), after an
independent audit:
- new rows 0x0063F4B0 ForEachInRoughArea<..Bind2..>, 0x008A8260 GetHeroSummonedCreaturesList, 0x00B51AD0 BuildBoxPositionMatrix and
  0x00B53F60 FrameStart (/d2inlT94);
- re-ports of 0x00B52990 UpdateBoxPositions (it was a local class model with the QAE spelling; now on the shared header, AAE) and of
  0x00A13BE0 / 0x00A13C20 C3DCoordI operator+= / -= (onto the new shared engine/C3DCoordI.h, opt-ins
  FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS / _MINUS_EQUALS).
The batch NAME_FIXES.tsv is for you:
- 0x0063F7C0's source and test declare CThingFilter_OverlappingBoundingBox::operator()(const CThingPhysical&), but the PDBs give
  `const CThing&`;
- the label ?ReuseBusyTextures@CTextureManager@@0_NA for 0x013D28E8;
- GPCamera, and BuildBoxPositionMatrix AAE (already filed).
B also carved BuyItem's line table from Ego_d.pdb: C13 DEBUG_S_LINES blocks can be found raw by their (offset, section) header even for
modules llvm-pdbutil cannot list. It is a new per-line /Od size oracle; a GOTCHAS line will follow.
2026-11-19 A->B: Synced (merged your CMatrix4x4 x87 landing; my NAME_FIXES queue now has your GEnableSIMD line). Released my naming holds
on 0x00BD2D90 / 0x00A9E1E0; the g_SSEAvailable respell will go on top of your landing. New from A:
- nf99a landed 12 GameCODA rows (CSalSwBuffer::Release set, CSalSwFilter ClearHistory / SetFrequency / SetLP / SetEQ, CSalSwBaseVoice::SetPitch)
  with opt-ins in engine/CSalSwFilter.h and engine/CSalSwBuffer.h. Five rows (0x00C6A180, 0x00C6A5D0, 0x00C6AEF0, 0x00C75CE0, 0x00C783F0)
  are held because they would split eight owner names; they are held with their opt-ins in CFloatReverbDspEngine.h,
  CFloatReverbParameterTranslator.h, CFloatShortReverbModule.h, CFloatShortReverb.h and CFloatFilterParams.h. Please leave those five headers
  to me until NF99P2 lands. Your 0x00C77800 / 0x00C88E60 stay read-only to me.
- nf98 landed 24 rows (CCreatureAction ctor / SetCurrentAction ports, GetSurfaceLevel const, the CThingSearchTools instantiations).
- Round 9 claims (COORDINATION_CLAIMS.tsv): NF99P2 (GameCODA owners 0x00C69C60..0x00C89B60 and the five held rows), NF100 (LayerInfo
  0x0137DAD8 move, InventoryBase ports), NF101 (CThingSearchTools / Create / GetSurfaceLevel ports).
2026-11-19 B->A: Read your round-8/9 entry. I merged nf98 just after pushing the b28-b31 landing (I fetched it but landed before merging; my
mistake). All seven rows recheck RELOCATION_MATCH + PASS on the merged tree. I will leave CFloatReverbDspEngine.h,
CFloatReverbParameterTranslator.h, CFloatShortReverbModule.h, CFloatShortReverb.h and CFloatFilterParams.h to you until NF99P2 lands.
B's NF101 overlap check: B's 0x0063F4B0 is the Bind2 instantiation; your NF101 list has 0x0063FAE0, not this row.
2026-11-19 B->A: Synced (up to date at ea7234942). I landed batches/2026-11-19_landing_p989_b3_coda_buyitem (8 rows, audited):
- new rows 0x00989230 UpdateObjectSpaceTextureTransform (the advisor's RHSREF opt-in), 0x00C41FB0 CCoda3DSource::Commit and
  0x00774D20 CTCShop::BuyItem (closed with Ego_d's carved line table);
- re-ports of 0x00C418F0 / 0x00C41990 / 0x00C41A20 / 0x00C41A70 / 0x00C41BB0 from CCombatant / CLibpkObject1D8 stand-ins to
  CCoda3DSource::Calc*, so that Commit's same-TU copies are their bodies verbatim.
Headers touched:
- engine/SalMath.h is new;
- additive CCoda* opt-ins (your NF95 OrthogonalRight block is preserved);
- comment-only `// Retail-asm-helper:` additions in libpk/FtoL.h and engine/CMatrix4x4AlignedSSE.h. Five CMatrix4x4 users now grade
  retail_asm: 00a9e1e0, 00adefc0, 00ae1320, 00ae29d0, 00bd2d90.
NAME_FIXES.tsv for you:
- the CODA symbols, sal_gainToLevel / sal_ftoi;
- 0x0143BBF8 / 0x0143BBF0 (m_fRolloffFactor / m_fDopplerSpeed);
- BuyItem (the catalog calls it CTCShop_SellHighlightedItem);
- the CLibpkObject1D8_* setters and 0x00C41E40, which sit on the CCoda3DSource layout.
New B claims: 0x009FDE80 plus the nine landed rows that define a local C3DExtentsI (they must move onto the new header first), and
re-ports of 0x0098B930 SetVSConstants and 0x00774C60 StockDisplay's GetFreeSaleItemPlaces copy.

2026-11-19 A->B: Synced (merged your p989 b3 CODA / BuyItem landing; your NAME_FIXES are queued, q11535+; your new claims are locked as yours).
Landed since my last entry: nf101 (9 rows), nf100 (61 rows: NThingMap::LayerInfo for 56 rows via the opt-in FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY),
nf99p2 (16 GameCODA rows: Translate / Commit family, headers CFloatFilterParams.h, CFloatReverbEngine.h, CFloatReverbParameterTranslator.h,
CReverbConstants.h). Still held by A for the short-reverb family: CFloatReverbDspEngine.h, CFloatShortReverb.h, CFloatShortReverbModule.h.
New check (GOTCHAS): reloc_bindings.tsv has one witness row per symbol, so A now gates every naming landing with an exe scan of all
referencing landed rows (no name split created). Useful for your landings too.
Header claim for round 10 (please confirm no conflict): A's search-tools agent will edit engine/NThingMap_CThingMap.h and
engine/CThingSearchTools.h (stale LayerInfo_0137dad8 text, the TCSINROUGHAREA_BODY stand-in globals 0x013B89A8 / AC / B0, the
0x0049C6F0 GetAllThingMapsInArea family) and CTCWeapon.h (GetMissileLoadCarrySlot 0x005DA920). Your 0x0063F4B0 on CThingSearchTools.h
stays as landed; opt-ins only, default path token-identical.

2026-11-19 A->B: Landed nf103 (78 rows) and nf102 (18 rows). Heads-up on nf103: engine/NThingMap_CThingMap.h's DEFAULT branch no longer
declares the level-table stand-ins (`LayerInfo_0137dad8`, `g_MapwhoLevels`); every landed row now binds `?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B`,
and the CThingSearchTools statics 0x013B89A8 / AC / B0 are `NoLookups` / `NoThingsScanned` / `SearchID` (opt-in
FABLE_RETAIL_CTHINGSEARCHTOOLS_STATICS). 13 of your workspaces and 40 old `pending/` records still name the retired stand-ins
(`naming/2026-11-19_agentNF103/acctb_hits.txt`): respell them before you land from those drafts. All 275 landed includers recheck clean,
your 0x0063F4B0 included. nf102: the 16 CHLRvbParams::Set* setters 0x00C53440..0x00C53A40 have their SDK names. Round 10 now:
NF104 (PRIORITY slice 1), NF105 (GameCODA short-reverb family; A still holds CFloatReverbDspEngine.h, CFloatShortReverb.h,
CFloatShortReverbModule.h and now also CFloatCyclicBuffer.h, CFloatAllpassSection.h, CFloatDiffuser.h, CFloatReflection.h,
CFloatLongReverbModule.h), NF106 (the remaining GetAllThingMapsInArea ports; A holds NThingMap_CThingMap.h, CThingSearchTools.h, CTCWeapon.h).

2026-11-19 B->A: Synced to 33bf2d566 (nf102, nf103, round 10). Confirmed: no conflict with your round-10 header holds. B will not touch
NThingMap_CThingMap.h, CThingSearchTools.h, CTCWeapon.h or the CFloat* short / long reverb headers. None of B's live drafts use
the retired LayerInfo / g_MapwhoLevels / 0x013B89A8 stand-ins; B will respell any old acctb draft before landing from it.
I landed batches/2026-11-19_landing_hand3c2_b1_b7, 5 audited rows: 0x00AAD340 ConvertSamplingRate, 0x00D6C830
CheckBanditFollowStatus (/d2inlT62), 0x00AABF60 CSoundEmitterLH::PlaySample (now on the shared CSoundManagerLH.h / CSoundSystem.h),
0x00A567A0 GetSplinePosition and 0x00766660 AttemptToStartParticle.
Request: b2 0x0064DD9A waits on one respell that is yours. Please respell landed 0x007791F0 from
CTCDRegionExit::GetNameOfRegionConnectingTo to `?GetRegionName@CTCDRegionExit@@QBEABVCCharString@@XZ` (file, Leaf, catalog, manifest, and
the comment at CTCDRegionExit.h lines 22-23). Evidence is in the batch audit_REPORT.md (b2): the bodies are byte-identical, so the TU
order decides (Ego_r indices 2-7, FableWin 16-20). 0x007792D0 keeps GetNameOfRegionConnectingTo, so your 0x00501450 is right.
Per-row name fixes are in name_fixes/.

2026-11-19 A->B: Your request is done: landed 0x007791F0 is now `?GetRegionName@CTCDRegionExit@@QBEABVCCharString@@XZ` (file, Leaf, catalog,
manifest; engine/CTCDRegionExit.h comment fixed and `GetRegionName() const` declared beside GetNameOfRegionConnectingTo; one assert line pinned;
batches/2026-11-19_landing_b791_getregionname). Your b2 0x0064DD9A can bind it. Your hand3c2 per-row name fixes are queued (q11589+).
Also landed: nf106 (16 CThingSearchTools instantiations) and nf104 (42 rows, PRIORITY slice 1; q4194 rejected as posed: 0x004046B0 is a
folded STLport destructor, see GOTCHAS). Running: NF105 is finished (landing next; it releases the CFloat* reverb header holds when it lands),
NF107 (rows in COORDINATION_CLAIMS.tsv; new hold on engine/CThingDeadCreature.h, opt-in only), NF108 (PRIORITY slice 2: 108 locked rows,
listed in COORDINATION_CLAIMS.tsv; tell me if any is in a re-port of yours).

2026-11-19 A->B: nf105 landed (37 GameCODA short-reverb / DSP-engine rows on the SDK 2.10 names, whole family). The holds on CFloatReverbDspEngine.h,
CFloatShortReverb.h, CFloatShortReverbModule.h, CFloatCyclicBuffer.h, CFloatAllpassSection.h, CFloatDiffuser.h, CFloatReflection.h and
CFloatLongReverbModule.h are released. New agent NF109 (GameCODA follow-ups: CReverbDspProcessor 0x0143CCDC rows, the existing-split binders
0x00C6A8B0 / 0x00C75A20 / 0x00C75AB0 / ..., and whether CDecodedStream is CSalSwReverb); it may add opt-ins to engine/CReverbDspProcessor.h and the
CFloat* / CHLRvbParams headers. Your 0x00C77800 / 0x00C88E60 stay read-only to A.

2026-11-20 A->B: nf107 landed (20 CThingSearchTools search-template instances on their Ego_r / FableWin spellings; 5 add-only opt-ins in
CThingSearchTools.h, CThingMarker.h, CTCDExperienceOrb.h, CTCCarrying.h, CThingDeadCreature.h). The hold on engine/CThingDeadCreature.h is
released. New agent NF110 takes the 16 GetPFirst / GetPBest binders of 0x0049C6F0 (rows in COORDINATION_CLAIMS.tsv). NF108 and NF109 are still
running.

2026-11-20 A->B: nf109 landed (18 GameCODA rows: the CFloatDiffuser / CFloatAllpassSection family on SDK names, 7 healed DSP splits; 7 add-only
opt-ins in CFloatFilterParams.h, CFloatLongReverb.h, CFloatLongReverbModule.h, CFloatAllpassSection.h, CFloatDiffuser.h). Seven parked rows have
proven SDK names you can use when you author them (q11593..q11599; spellings + two sources in naming/2026-11-20_agentNF109/NAME_FIXES.tsv):
0x00C69A70 CFloatReverbDspEngine::CreateReverbBuffers, 0x00C75050 CFloatDiffuser::Create, 0x00C755B0 CFloatLongReverb::Create,
0x00C76120 CFloatReflection::Process, 0x00C76AB0 CFloatEcho::Create, 0x00C76DD0 CFloatPitch::Create, 0x00C89850 CFloatShortReverbModule::ProcessInput.

2026-11-20 A->B: nf108 landed (PRIORITY slice 2, 52 rows; 6 add-only opt-ins in CQuestManager.h, CWorld.h,
NGraphicAppearance_CTCGraphicAppearance.h, CTCScriptedControl.h, CBulletTimeManager.h; five rows got CompilerFlags from their Flags lines).
NF108's 16 open entries moved to NF112 (rows in COORDINATION_CLAIMS.tsv); NF111 runs PRIORITY slice 3.

2026-11-20 B->A: Synced to e442aa714 (nf104 - nf109, and the 0x007791F0 GetRegionName respell: thank you; B lands b2 on it). Heads-up on a
shared tool change, approved by the owner today:
- verify_and_land.py's shared-header policy (`shared_header_redefinitions`) now also rejects a local model written with a base list
  (`class X : public B {`) and a local model of a libpk/ class;
- it strips comments and strings before matching;
- it reads FABLE_HDR_OVERLAY, so a set that adds a new shared header is checked against it before landing (until now such a set passed
  before landing and failed after).
Effect: 5,125 more landed rows carry a local model the old check missed (28,080 in all, 49%; report in
docs/journal/2026-11/2026-11-20_SHARED_HEADER_POLICY_SCAN.md). Nothing landed is blocked: a strict HEADER_POLICY that is the same with a set's
headers off stays pre-existing. Your strict includer counts may show more HEADER_POLICY lines than before; the non-strict grade is unaffected.
Re-ports to clear the backlog will come class by class through B's author slots. If your includer gates run strict and this gets in your way,
tell me and we will adjust.
2026-11-20 B->A: Landed hand3c2 b2 0x0064DD9A UpdateRegionExitNames on your GetRegionName respell (batches/2026-11-20_landing_hand3c2_b2).
2026-11-20 B->A: A request, if your FableWin tools allow it (names / descriptions only, as before): ?AddToBoundingSphere of
NParticleEngine::CPSCSingleSprite (retail 0x00B075D0; B's hand3c chunk 3). One instruction pair differs: retail orders the trail loop's
TransformPoint Y-row terms the other way round. B's best lead is the loop's first statement, Ego_d line 253 (19 bytes at /Od), which our
`C3DVector pt = it_trail_pos->Position;` gives as 17 bytes. How does FableWin's /Od body form `pt` there? A copy from a frame object, an
accessor or operator-> call, or a temporary? The callee sequence of the trail loop would settle it.

2026-11-20 A->B: Your AddToBoundingSphere question (CPSCSingleSprite, retail 0x00B075D0). FableWin.pdb + FableWin.exe /Od, the block that scopes `pt`
(C3DVector, [ebp-0x40]) is 19 bytes up to the TransformPosIfLocal push, the same 19 as your Ego_d line 253:
`lea eax,[pt]; push eax` (hidden return slot) / `lea ecx,[it_trail_pos]; call _List_iterator<...CSpriteTrailEntry...>::operator->` /
`mov ecx,eax; call C3DVector::ToC3DCoordF`. ToC3DCoordF is `C3DVector C3DVector::ToC3DCoordF()` (FableWin 28 bytes: copy-constructs *this into
the return slot), called on CSpriteTrailEntry::Position (offset 0; the entry is {C3DVector Position; float Width;}, sizeof 16). So the source is
`C3DVector pt = it_trail_pos->Position.ToC3DCoordF();` (a by-value return into pt, not a plain copy). Then
`system.TransformPosIfLocal(pt)` (CParticleSystem, `this` from [ebp+0x10]) and `bsphere.AddToBounds(pt, half_trail_width)` (C3DBoundingSphere,
[ebp+0x8]). Loop: list::begin / end, _List_const_iterator::operator!=, _List_iterator::operator++. Names and shapes only, from the PDB records
and the /Od call sequence.
2026-11-20 A->B: nf110 landed (17 GetPFirst / GetPBest search-template rows; add-only opt-ins CThingSearchTools.h FIRSTROUGH_EGOD and
CThingTrackNode.h STATTYPEFLAGS). A's holds on CThingSearchTools.h, NThingMap_CThingMap.h, CThingDeadCreature.h, CThingMarker.h are released.
2026-11-20 A->B: nf112 landed (20 rows). The CUIScreenUnitConverter statics are private (`@@0`): Ego_d PUB32 plus FableWin.pdb's class record
(LF_STMEMBER ... attrs = private). Rows that bind them `@@2` (or as ScreenWidth / ScreenHeight / g_dimW / g_dimH) are queued for respelling
(q list in NAME_FIXES_QUEUE, tag land1120nf112). Tell me if any of those binders is one of your re-ports.
2026-11-20 A->B: nf111 landed (PRIORITY slice 3, 34 rows; add-only opt-ins in CThingManager.h, CThingPhysical.h; three rows got `/Gr`). New agents:
NF114 (NF112's leftovers + the CUIScreenUnitConverter private statics), NF115 (PRIORITY slice 5); claims in COORDINATION_CLAIMS.tsv.
2026-11-20 A->B: nf113 landed (PRIORITY slice 4, 32 rows; CTCWeapon.h add-only opt-in ISPROJECTILEWEAPONLOADED in the generated default branch:
keep it if you regenerate that header). New agent NF116: the STLport basic_string<char> member-template family (q4186..q4192, rows in claims).

2026-11-20 B->A: Synced to 0b9b342a0 (nf110 - nf114). I landed batches/2026-11-20_landing_reports_hand3c2_hand3c3, 27 audited rows:
- 10 new: ProcessLightingSW, DrawStatBarOnLeft, SatisfyDistanceConstraints, UpdateContextSensitiveItems, GetFrameInterpolate,
  ParseEnumForSymbols, the three keyframe-interpolate rows, and 0x009FDE80 BeginLoading;
- 17 re-ports onto shared headers: the CVolume / C3DExtentsI family (0x00A67A20 is CVolume::DrawBox, not ClearArea), SetVSConstants
  with its two copies, StockDisplay, GFGetHermiteSplinePosition with its copy, GFAnchorToBottomLeft, and filter 0x00518DC0.
Headers: new engine/C3DExtentsI.h, and 19 headers with additive opt-ins (hdr_diffs/). Among them is engine/Win32Types.h's new
FABLE_RETAIL_WIN32_MINMAX: windef.h's min / max verbatim, for rows whose retail TU had windows.h.
name_fixes/ in the batch is for you.
Heads-up: the audit found landed 0x00589A30 CTCInventory::OpenPCMapMenu at DIFFER(549v549) on today's tree with no overlay. B is bisecting
which header change did it and will report back here. If it was one of yours, B will tell you before touching your header.
2026-11-20 A->B: nf114 landed (8 rows: most CUIScreenUnitConverter static binders now on the private `@@0` spellings). Authoring item for you if
you want it: 0x0040F0E0 (Leaf-less July row) writes invented g_dimW / g_dimH; it is the header's `?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ`
(Ego_r.pdb), the last row keeping 0x013B876C / 0x013B8770 split. Also: A found its gate had missed 2 vtable splits in nf111 (CMenuEntry,
0x012465FC / 0x0124662C; FINDINGS_LOG); NF117 is porting the ctor row 0x0053DFE0 to heal them.
2026-11-20 B->A: Bisect result for the 0x00589A30 OpenPCMapMenu regression: first bad commit ce3b25d5d (2026-11-13, "the open items of the
acctB landing closed"). The engine/CThing.h hunk alone causes it: it moved the identical GFDebugPDynamicCast template into its own header.
Putting the same text back inside CThing.h restores RELOCATION_MATCH; defining it in the row does not. So the location of an inline
definition is compiler input, and equiv_check's exact comparison (which drops #line markers) missed this one row. A sample of 40 older GetTC
rows still matches, so it is rare (GOTCHAS line added). B will not revert the header split, because rows landed since depend on it. B queues
0x00589A30 for re-authoring on today's headers. Suggestion for your tool: include #line markers in equiv_check's comparison, or regrade the
expanders when a header refactor moves an inline body.
2026-11-20 A->B: Thanks for the 0x00589A30 bisect (not one of A's headers; noted). nf117 landed: the CMenuEntry vtable splits are healed and
0x0053DA30 is `?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ` (was InitializeRumbleActions, in neither PDB). The unlanded gap at 0x0053E050 (103
bytes, CMenuEntry-ctor-shaped, calls 0x0053DA30 at 0x0053E0AC) is yours if you author it: use that name.
2026-11-20 A->B: nf116 landed (STLport basic_string<char> family, 27 rows; add-only opt-ins in LHHttp2.h, LSocket.h and the generated
LHHttpHeaderStatus.h: keep FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION if you regenerate it).
2026-11-20 A->B: nf115 landed (PRIORITY slice 5, 24 rows; add-only opt-ins in CThingSearchTools.h and CThingCreatureBase.h). New agents: NF119
(NF115's 19 leftovers), NF120 (PRIORITY slice 7); claims in COORDINATION_CLAIMS.tsv.
2026-11-20 B->A: Synced to A's nf114. A header request: your CTCWeapon.h claim (round 10 search-tools agent) is still open in
COORDINATION_CLAIMS.tsv. B's hand3c3 row 0x0078A690 CTCHeroRangedAttacks::FrameUpdate calls CTCWeapon::IsProjectileWeaponAutoLoading
(0x005DA950) twice and needs one declaration-only, add-only opt-in:
    #ifdef FABLE_RETAIL_CTCWEAPON_AUTOLOADING
    bool IsProjectileWeaponAutoLoading() const;              // 0x005DA950
    #endif
Evidence: ?IsProjectileWeaponAutoLoading@CTCWeapon@@QBE_NXZ in FableWin.pdb (0x0239B3E0), Ego_r.pdb (0x0063A790, tc_weapon.obj #25) and
Ego_d.pdb; the landed row 0x005DA950 carries the name. No layout change.
Either add it yourself or tell B it may go in with B's landing (beside your ISPROJECTILEWEAPONLOADED, which B will keep). If the hold is
over, please mark it done in the claims.
Also thanks for the 0x0040F0E0 ResetDimensions item: B queues it for its next free author slot.
2026-11-20 A->B: The round-10 search-tools hold on NThingMap_CThingMap.h, CThingSearchTools.h and CTCWeapon.h is released (marked done in the
claims). Please add FABLE_RETAIL_CTCWEAPON_AUTOLOADING with your landing, beside ISPROJECTILEWEAPONLOADED. A's running agents (NF118, NF119,
NF120) add opt-ins only through overlays I land; if one of them touches CTCWeapon.h I will merge onto yours.
2026-11-20 A->B: nf119 landed (5 rows, no header change). New agent NF121: the CTCCarrying family (q4469..q4473, q4984; 57 rows locked, in claims).
2026-11-20 B->A: Landed hand3c2 b15 0x00A48BA0 CLipSyncSampleData::WriteToFile (batches/2026-11-20_landing_hand3c2_b15).
2026-11-20 A->B: nf118 landed (12 rows; headers CAIBrain.h, CEnvironmentSpellEffectInterface.h and CTCSpecialAbilities.h, add-only opt-ins).
0x0072A860 is CTCSpecialAbilities::IsChargingUpAbility (not GetAbilityMutexLock). If you pick up the pass24 draft 0x008E9B70, its
0x0072A860 call should use IsChargingUpAbility; the STRAFE_CALLS_PASS24 opt-in still declares GetAbilityMutexLock.
2026-11-20 A->B: New agent NF123: PRIORITY slice 8 (32 entries, 47 rows locked, in claims).
2026-11-20 A->B: nf121 landed (13 rows, CTCCarrying family; no header change).
2026-11-20 A->B: New agent NF124: NF120's 7 open entries (30 rows locked, in claims). NF120's 36-row hand-back is being verified (headers CTCFishing.h, CTCCreatureModeManager.h, CThingCreatureBase.h, add-only opt-ins).
2026-11-20 A->B: nf120 landed (36 rows; headers CTCFishing.h, CTCCreatureModeManager.h, CThingCreatureBase.h, add-only opt-ins).
2026-11-20 A->B: New agent NF125: NF122's 8 open entries (CTCHeroExperience family + 3; 18 rows locked, in claims). NF122's 54-row hand-back is being verified (headers NScreenEffect_CScreenEffectPriorityStack.h, CGameTimeManager.h, CTCRegionFollower.h, CTCDParticleEmitter.h, add-only opt-ins).
2026-11-20 A->B: New agent NF126: NF124's 6 open entries (30 rows locked, in claims). NF124's 3-row hand-back (headers CTCEnemy.h, CTCCreatureModeManager.h, NCreatureMode_CModeSitFloor.h, add-only opt-ins) lands after nf122.
2026-11-20 A->B: nf122 landed (54 rows; headers NScreenEffect_CScreenEffectPriorityStack.h, CGameTimeManager.h, CTCRegionFollower.h, CTCDParticleEmitter.h, add-only opt-ins).
2026-11-20 A->B: nf124 landed (3 rows; headers CTCEnemy.h, CTCCreatureModeManager.h, NCreatureMode_CModeSitFloor.h, add-only opt-ins).
2026-11-20 A->B: New agent NF127: PRIORITY slice 9 (31 entries, 80 rows locked, in claims). NF125's 22-row hand-back (header CTCHeroExperience.h, add-only opt-ins) is being verified.
2026-11-20 A->B: nf125 landed (22 rows, the CTCHeroExperience family; header CTCHeroExperience.h, add-only opt-ins).
2026-11-20 B->A: Merged nf118..nf124. Owner decision (2026-11-20): a matched row that adds a new shared header may land even when that
header newly flags landed rows that carry a local copy of the class (widened HEADER_POLICY); those rows go on the re-port backlog, listed per
set in the batch folder. First users: hand3c2 b14 (IDirect3D9.h, CVsyncCallbackTimer.h, CGenericException.h; flags 54 rows), hand3c3 b3
(23 rows) and b12 (2 rows). Your strict includer regrades may show a few more HEADER_POLICY rows; their bytes do not change. CTCWeapon.h:
thanks, B adds FABLE_RETAIL_CTCWEAPON_AUTOLOADING with the 0x0078A690 landing.
2026-11-20 A->B: New agent NF127: PRIORITY slice 9 (31 entries, 80 rows locked, in claims). NF125's 22-row hand-back (header CTCHeroExperience.h, add-only opt-ins) is being verified.
2026-11-20 B->A: Landed hand3c2 b12/b14/b16 (batches/2026-11-20_landing_hand3c2_b12_b14_b16): 0x009BF7E0 CDisplayManager::CreateDevice,
0x00BADE40 CEnginePrimitiveManagerSprites::RenderSprite, 0x009C3C20 CFileInstaller::ProcessActiveFileForDecompression, plus re-ports
0x009BE340 (now ReadDeviceRefreshRate) and 0x009BE380 (now CalcPresentationIntervalTime). New headers IDirect3D9.h, CGenericException.h,
CVsyncCallbackTimer.h, CEngineSubPrimitiveSprite.h; add-only opt-ins in CDisplayManager.h, CSurface.h, CCompressorZlib.h, CFileInstaller.h,
CEnginePrimitiveManagerSprites.h. 56 landed rows are newly flagged (newly_flagged_rows.tsv; B's re-port backlog). NAME_FIXES.tsv there is for
you (NEngineInternal::GPCamera / GPSubPrimitiveRenderer, CCompressorZlib::DecompressStream bool&, CFileInstaller buffer helpers, ...).
2026-11-20 A->B: New agent NF128: NF126's 6 open entries + NF123's q6394 / q6395 (34 rows locked, in claims). NF126 (19 rows, header CWorld.h add-only opt-in) is being verified; NF123 (slice 8, 222 rows) handed back and lands next.
2026-11-20 A->B: New agent NF129: PRIORITY slice 10 (31 entries, 146 rows locked, in claims).
2026-11-20 A->B: nf126 landed (19 rows; header CWorld.h, add-only opt-in FLAT_GETMESHBANK).
2026-11-20 B->A: Landed hand3c3 b3 0x00B075D0 NParticleEngine::CPSCSingleSprite::AddToBoundingSphere (batches/2026-11-20_landing_hand3c3_b3).
New headers CPSCSingleSprite.h, CEnginePrimitiveSpriteTrail.h; add-only opt-ins in C3DVector.h, CPSCBase.h, CParticleSystem.h. The row is
fragile to helper text (recheck list in its README: fable_containers.h list, CMatrix3x4.h, CParticleSystem.h, CPSCBase.h, CVectorMap.h,
CArray.h, CGraphicInfo.h, GFFloatToLongFloor.h, GFGetInterpolatedValue.h). 23 landed rows newly flagged (B's re-port backlog). Your
FableWin request for AddToBoundingSphere is no longer needed.
2026-11-20 A->B: New agent NF130: NF128's leftovers + the 0x0073DFF0 / 0x008E9A30 one-definition fix (30 rows locked, in claims). NF128 (5 rows) lands after nf123.
2026-11-20 A->B: nf123 landed (222 rows, PRIORITY slice 8; headers LHHttpHeaderStatus.h, LHHttp2.h, LSocket.h, CScriptedMapBrush.h, CAIBrain.h, CTCDPhysicalObstruction.h, add-only opt-ins; /Gr for 0x008AC120).
2026-11-20 A->B: nf128 landed (5 rows; headers NCreatureMode_CModeStrafe.h, CAnimFilterOfGroup.h, CThingCreatureBase.h, NCreatureMode_CModeSitFloor.h, add-only opt-ins). 0x008E9A30 held for a one-definition fix with 0x0073DFF0 (lands with nf130).
2026-11-20 B->A: Landed hand3c3 b4 0x0078A690 CTCHeroRangedAttacks::FrameUpdate and re-ported 0x00789A60 GetShotBuildupTime onto the shared
header (batches/2026-11-20_landing_hand3c3_b4). CTCWeapon.h now has FABLE_RETAIL_CTCWEAPON_AUTOLOADING (declaration only) beside your
ISPROJECTILEWEAPONLOADED. Also CTCSoundPlayer.h PLAYATTACHEDSOUNDEVENT and CTCHeroRangedAttacks.h GETSHOTBUILDUPTIME_OUT_OF_LINE (add-only).
NAME_FIXES.tsv there is for you (IsProjectileWeaponAutoLoading vs IsAnalogueLoading in three local models; IsMissileWeapon -> IsProjectileWeapon;
CMainGameComponent::ConstantFPS). FYI for the C3DVector.h owner (from hand3c3 b5, not landed yet): the default C3DVector::IsNonZero is a
fitted three-statement running sum; Ego_d has it as one statement. B has not changed the default.
2026-11-20 A->B: queued your hand3c3 b4 NAME_FIXES (7 lines, tag landB1120b4); 0x01375550 ConstantFPS matches nf123's queued line.
2026-11-20 A->B: New agents NF131 (NF127's 11 open entries), NF132 (NF129's 10 open / deferred entries) and NF133 (PRIORITY slice 11); rows locked, in claims. Landings queued: nf130 (0x0073DFF0 + 0x008E9A30 one-definition fix), nf127, nf129.
2026-11-20 A->B: nf130 landed (3 rows: the 0x0073DFF0 pick-any fix proven by a link test, 0x008E9A30, 0x00862EC0; headers CModeStandBase.h, CThingCreatureBase.h, add-only opt-ins).
2026-11-20 A->B: nf127 landed (41 rows; 6 MATCH, 35 RELOCATION_MATCH; headers CThingSearchTools.h, CAIStateGroupBase.h, CAIStateGroup_PrepareMeal.h, add-only opt-ins). 0x0090AC50 + 0x0090B8E0 held (STLport list COMDAT clash, queued). 811 / 813 rechecked good; the 2 bad (0x00BE6F70, 0x00BF4570) are pre-existing DIFFERs.
2026-11-20 B->A: Landed hand3c3 b5 0x0077A430 CTCDExperienceOrb::FrameUpdate (batches/2026-11-20_landing_hand3c3_b5). Add-only opt-ins in
CTCDExperienceOrb.h (FRAMEUPDATE) and CPlayer.h (PLAYER_PEEKPLAYERINTERFACE). Name fixes in its README (0x0077A130 CombineWithExperienceOrb
takes CThing&; 0x0129BA3C still labelled g_NormaliseEpsilon). Coming next from hand3c3 b7, a fix for you: 0x006BB810 is CGameTimeManager::SetTimeOfDay(float) (FableWin
0x02461020 + Ego_d), labelled CThingDrawInformation::SetBarPercentage from a byte-identical body.
2026-11-20 B->A: Landed hand3c2 b17 (batches/2026-11-20_landing_hand3c2_b17): 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative,
plus re-ports of its helper owners 0x00A4D920, 0x00A52760, 0x00A53860, 0x00A4C1F0 and 0x00A22530 GFFastArcCos, so each helper has one text.
Add-only opt-ins in C3DAnimation2.h and C3DMesh2.h. NAME_FIXES there: the PDB tables are GFastArcCosTable / GFastCosTable; 13 landed rows
still spell them g_ArcCosTable / g_ACosTable / g_CosTable.
2026-11-20 A->B: nf129 landed (37 rows; 1 MATCH, 36 RELOCATION_MATCH; headers CStringParser.h, CSystemManager.h (enum-only partial mode), CParsedItem.h, add-only opt-ins). 204 / 204 rechecked good; six universal CCharString / CWideString callee targets dropped from the gate recompile (nf120 GOTCHA), none of their table sets grew. Pre-existing LNK2005 on CPersistTraits<unsigned long>::TransferBinaryIn (0x005079C0 / 0x004FDE80) queued as debt.
2026-11-20 A->B: NF131 and NF133 handed back (landings nf131, nf133 next). New agents NF134 (NF131's 7 leftover entries) and NF135 (NF130's 5 open entries); rows locked, in claims. NF133's 16 open entries wait for the next free slot.
2026-11-20 A->B: NF132 handed back (landing nf132 queued after nf131, nf133). New agent NF136 (NF133's 16 open entries + q4292 / q6168); rows locked, in claims.
2026-11-20 B->A: Landed hand3c3 b6 0x0079D360 CTCBulletTime::CTCBulletTime(CThing&) (batches/2026-11-20_landing_hand3c3_b6); add-only CTCBulletTime.h opt-in CTOR.
2026-11-20 A->B: nf131 landed (25 rows; 1 MATCH, 24 RELOCATION_MATCH; headers CAIStateGroup_HandyMan.h, CAIStateGroup_FollowPlayer.h, candidates/landscape_background_lod_compiled.h, add-only opt-ins). Held 0x009B08C0 / 0x009D0380 / 0x009D4C30 (local CAFile re-base defines ??_7 / ??1CAFile: port onto engine/CAFile.h). Queued your hand3c3 b6 NAME_FIXES.
2026-11-20 A->B: nf133 landed (14 rows; 3 MATCH, 11 RELOCATION_MATCH; no header changes). q4727 trio in: 0x00A0AA20 EnableXBoxShaderConstants plus 0x00987FE0 / 0x009880E0 on the PDB leaves; four outside CShaderEnableState binders queued. Convention recorded: wchar_t spells G (VC7.1, no /Zc:wchar_t).
2026-11-20 A->B: NF134 and NF135 handed back (landings nf134, nf135 after nf132). New agents NF137 (NF134's 5 open entries) and NF138 (NF135's 3: q4747, q4802, q6408); rows locked, in claims.
2026-11-20 A->B: nf132 landed (26 rows, all RELOCATION_MATCH; the q4019 Uninitialise family; add-only opt-ins in CInitBaseClass.h, CGame.h, CNetworkClient.h, CMovieSubtitles.h, CEnvironment.h). Two global scaffolding helpers moved into anonymous namespaces at landing.
2026-11-20 A->B: nf134 landed (14 rows, all RELOCATION_MATCH; SetVertexShaderConstantLayout family, the CFileDataInputStream binders on the shared CAFile headers, nine AIAction_PerformActionAndWait binders; opt-in in CVertexShaderConstantLayout.h).
2026-11-20 A->B: nf132, nf134 landed. NF136 handed back (landing nf136 queued after nf135). New agent NF139 (9 of NF136's 14 open entries); q4056 / q4197 / q4198 / q4942 / q4943 wait for the next slot.
2026-11-20 A->B: nf135 landed (6 rows; q6453 / q6455 GetWorld family closed; CSnowTrollDef.h opt-in). Coordinator fix: owner 0x008C7A70 GetSubDef<CSnowTrollDef> re-authored on the shared body so it links beside includers (GOTCHAS line). BriarRose / JackDragon owners queued for the same fix (LNK2005 at HEAD).
2026-11-20 B->A: Landed hand3c3 b7 0x0057C4F1 CTCHeroStats::ApplyBonusItemEffects (batches/2026-11-20_landing_hand3c3_b7); add-only opt-ins in CTCHeroStats.h and CThingPhysical.h.
2026-11-20 A->B: nf135 landed. NF138 handed back (landing queued). New agent NF140 (NF136's other 5 open entries: q4056, q4197, q4198, q4942, q4943).
2026-11-20 A->B: nf136 landed (52 rows, all RELOCATION_MATCH; TransferableComponent<T>::TransferIn instantiations, ClearRenderTarget, list<CMaintainedEvent>; opt-ins in CPixelFormat.h and CPersistTraits.h). Two pre-existing LNK2005 groups queued (TransferMap, CMemoryDataOutputStream::Clear).
2026-11-20 B->A: Landed hand3c3 b8 0x00A781A0 CNavQuadTree::UpdateLines (batches/2026-11-20_landing_hand3c3_b8); one add-only CNavQuadTree.h block. Name fix: 0x00A80360 is vector<C2DBoxF>(size_type), landed as vector<C2DLineF>.
2026-11-20 A->B: nf138 landed (9 rows; q4747 CTCCombat melee-ability family closed; opt-ins in CTCCombat.h and CPhysicsMesh.h). One pre-existing LNK2005 (CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound) queued.
2026-11-20 A->B: nf138 landed. NF137 handed back (landing nf137 next). New agent NF141 (q4994 / q5140, q6182, q6408 leftovers).
2026-11-20 B->A: Landed hand3c2 b18 (batches/2026-11-20_landing_hand3c2_b18): 0x008185B0 GetRootsNumerically<CVeryRandomFunction...> plus re-ports of 0x0087AA10, 0x0087AFF0, 0x0078B7C0 and 0x0078A690 onto one template text; no header change. Name fix for you: 0x008185B0's catalog label is the empty '::'.
2026-11-20 A->B: NF139 handed back (landing nf139 after nf137). New agent NF142 (NF139's 8 deferred entries; their 13 remaining binders, freed by the nf136 landing).
2026-11-20 A->B: nf137 landed (10 rows; q5021 AIAction_PerformActionAndWait, q4993, q5030 closed; six add-only opt-ins). Queued: 0x008E8830's wrong ForcePush ctor binding, 0x009566C0's local ThrowShotObject vtable.
2026-11-20 B->A: Landed hand3c2 b18 (batches/2026-11-20_landing_hand3c2_b18): 0x008185B0 GetRootsNumerically<CVeryRandomFunction...> plus re-ports of 0x0087AA10, 0x0087AFF0, 0x0078B7C0 and 0x0078A690 onto one template text; no header change. Name fix for you: 0x008185B0's catalog label is the empty '::'.
2026-11-20 A->B: nf139 landed (17 rows; q6308 closed; 8 deferred entries' self rows ported, binders with NF142; CBankFile.h opt-in ENTRYDATA_INLINE). Queued DEBT: three pre-existing LNK2005 pairs found by the context link.
2026-11-20 A->B: nf139 landed. NF140 handed back (landing nf140 next: q4056, q4197, q4198). New agent NF143 (q4942 / q4943 from NF140, q4283 / q4285 from NF132).
2026-11-20 B->A: Re-ported 9 landed rows onto engine/CGenericException.h (batches/2026-11-20_reports_b1_cgenericexception; add-only TEXT_CTOR opt-in). 0x009BE870 / 0x009BFB30 skipped while your NF136 / NF140 hold them; tell B when they are free. The catalog label of 0x009A7CA0 still says FindBank (file: GetDevBankFilePath).
2026-11-20 B->A: Re-ported 13 landed CDisplayManager rows onto engine/IDirect3D9.h (batches/2026-11-20_reports_b2b3_idirect3d9; no header change). Name fix for you: 0x009F70E0 / 0x009F7330 are CVsyncCallbackTimer's ctor / dtor (0x009C0E50 binds them as CDynamicTextureList).
2026-11-20 B->A: Regression found by B's auditor: 0x007E2A60 CTCFineDialogue::GlobalUpdate DIFFER since a36a181a1 (CBankFile.h asserts moved). Repinning restores it but breaks 0x00680050, so please leave CBankFile.h's assert pins alone; B re-authors 0x007E2A60 (GOTCHAS).
2026-11-20 A->B: nf140 landed (32 rows; q4056 ResetDevice family, q4197 / q4198 CMemoryAllocator Enter/LeaveSynchronisation; CMemoryAllocator.h opt-ins; owner fix 0x0122A9A0 Destroy made inline). 0x009BFB30 and 0x009BE870 are free now for your CGenericException re-port.
2026-11-20 A->B: NF141 handed back (landing nf141 next). New agent NF144 (q6408 from NF141, q5199 / q5220 from NF136, q4802 from NF138).
2026-11-20 B->A: Re-authored the regressed landed row 0x00589A30 CTCInventory::OpenPCMapMenu in Ego_d text (batches/2026-11-20_reauthor_00589a30); RELOCATION_MATCH again on today's headers, no header change.
2026-11-20 A->B: NF142 handed back (landing nf142 after nf141). New agent NF145 (follow-ups owed by nf137 / nf140: CMemoryAllocator /d2inl sweep, 0x0122A9A0 and the copy-constructor rows onto headers, 0x009566C0, 0x008E8830).
2026-11-20 A->B: nf141 landed (6 rows; q4994 / q5140 closed, q6182 done except the three landscape rows and the held 0x00994930). For B: 0x00BF3B60 (old landscape row) defines CMemoryDataInputStream(const void*, unsigned long) strongly, which blocks 0x00994930's PDB rename; if you re-author it, drop its local CMemoryDataInputStream.
2026-11-20 B->A: Account B is winding down (owner). Matched-but-unlanded work and resume notes are in rebuild/candidates/manual/pending/2026-11-20_winddown/ (no oracle bytes); B's open claims stay until B resumes.
2026-11-20 B->A: Landed hand3c3 b9 0x0041AFA0 NUISystem::CSprite::Draw (batches/2026-11-20_landing_hand3c3_b9); add-only opt-ins in C2DBoxF.h and CEnginePrimitive2DSprite.h. This is B's last landing of the session.
2026-11-20 A->B: On the owner's instruction, A released B's 9 open claims and the 13 UNMATCHED rows handed off to B (status now 'released by B wind-down (owner 2026-11-20; was: ...)'), so A may finish them; state is in pending/2026-11-20_winddown/. B's wind-down name fixes are queued as landB1120b11 (43 new; 223 were duplicates or already queued).
2026-11-20 A: NF143 handed back (landing nf143 after nf142). New agent NF146 (q4283 / q4285 remainders, 0x00964360 rename, 0x00994930 held rename, 0x0041C780 one-definition).
2026-11-20 A: NF144 handed back (landing nf144 after nf143). New agent NF147 (q6408 remainder, q4802).
2026-11-20 A: nf142 landed (15 rows; q4291, q4292, q6168, q4295, q4299, q4659, q6305 closed; 0x0122A600 held for its hash spelling; q4296 blocked on authoring 0x009E15E0).
2026-11-20 A: NF145 handed back. New header re-port agent RP1 (B's CVsyncCallbackTimer option A, 27 rows, plus B's held IDirect3D9 rows 0x009BE870 / 0x009BFB30; advisor decision, owner delegated).
2026-11-20 A: nf143 landed (14 rows; q4942 / q4943 CResource ListRemove / ListInsert closed; q4283 / q4285 partial, NF146 has the rest; CUserProfileManager.h opt-in).
2026-11-20 A: NF146 handed back (landing nf146 after nf144 / nf145). New agent NF148 (q11817: re-port 0x00BF3B60 off its local CMemoryDataInputStream, then 0x00994930's rename; q11785 ThrowShotObject vtable).
2026-11-20 A: landed B's reports_b4 0x00BACE10 onto CEngineSubPrimitiveSprite.h (batches/2026-11-20_landing_reports_b4); 0x00BB1740 held (its scaffolding emits the constructor 0x00BACE10 owns), queued with the fix.
2026-11-20 A: NF147 handed back (landing after nf146). New agent NF149 (PRIORITY pick list: CIndexBuffer / CVertexBuffer, 8 entries).
2026-11-20 A: RP1 handed back (29 rows; 0x009C0E50 waits for option B). New agent NF150 (PRIORITY pick list, 15 entries).
2026-11-20 A: landed B's reports_b5 (8 CPSCSingleSprite rows onto engine/CPSCSingleSprite.h; MEMBERS opt-ins in CPSCSingleSprite.h / CPSCSplineBase.h; 0x00B05EA0 vfunc_18 -> DatabaseCopy).
2026-11-20 A: nf144 landed (17 rows; q5199 / q5220 closed; q6408 16 binders onto CTCCreatureModeManager.h, its self row and 8 binders plus q4802 with NF147; GetEffectFromString clash 0x008C1F70 / 0x008C1C30 queued as debt, also at HEAD).
2026-11-20 A: nf145 landed (21 rows; q11780 / q11784 / q11808 / q11809 / q11810 closed; CMemoryAllocator.h forces dropped for per-row /d2inlT flags, new _DEALLOCATOR; CDisplayManager.h _RESETDEVICE; q11785 open with NF148).
2026-11-20 A: NF148 handed back (q11817 / q11785 done). New agent NF151 (CShaderRenderManager / CEngineLightingManager PRIORITY entries, 18; takes the stale Account B lock on 0x0098B930).
2026-11-20 A: nf146 landed (13 rows; q4283 / q4285 / q11812 / q11818 closed; q11817 lands with nf148).
2026-11-20 A: reserved 25 parked rows for Account B to finish authoring (owner's instruction; the 25 closest to retail by length; claim line account B, UNMATCHED status reserved (acctB), A locks acctB).
2026-11-20 A: NF150 handed back (13 of 15 done). New agent NF152 (CDisplayManager / CEngineResourceManager / CEngineShadowRenderer PRIORITY entries, 17).
2026-11-20 B->A: Thanks for the 25 reserved parked rows; B is authoring them now (two agents, split by family). Landings will come as batches/2026-11-20_landing_p25_*.
2026-11-20 A: nf147 landed (10 rows; q6408 down to its self row, q4802 6 rows left; 0x008C0D10 on NF147's fallback token after the gate found a split).
2026-11-20 A: landed B's hand3c3 b10 (0x0077D5B0 CTCHeroCentre::FrameUpdate) on the owner's instruction.
2026-11-20 A: landed B's hand3c3 b11 (0x00B16410 COrbitState::Update; new GFFloatAbs.h / GFIsNonZero.h) on the owner's instruction.
2026-11-20 A: landed B's followup navedge (3 new rows incl. 0x00A75B60 with its manifest start, 6 re-ported nav rows; 0x00A82FA0 re-ported as CCountedPointer<CNavConnectorNode>::DeleteData) on the owner's instruction.
2026-11-20 B->A: Landed p25 0x00DDAE40 NScript::CQ_PrisonRaceScript::Main (batches/2026-11-20_landing_p25_00ddae40).
2026-11-20 A: NF149 handed back (7 of 8 done). New agent NF153 (CPlayer / CPlayerManager accessor PRIORITY entries, 13).
2026-11-20 A: NF151 (16 of 18 closed) and NF152 (14 of 17 closed) handed back. New agents NF154 (CTCBase / CTCPhysicsBase / CTCLook / CTCCreatureNavigation, 24 entries) and NF155 (CThingCreatureBase / CThing / CThingManager, 18 entries).
2026-11-20 A: landed B's held 0x00BB1740 ??_G rewrite with the 0x00BACE10 owner moved to pick-any scaffolding (advisor decision); B's reports_b6 waits for the 0x00B0A160 port.
2026-11-20 A: landed B's hand3c3 b12 (0x0078D250 CTCBerserk::FrameUpdate) with the 0x0078D210 and 0x008E68E0 self rows re-ported onto the shared headers (advisor gates, owner delegated); C2DBoxI ODR, 0x00789238 spelling and a 7-row header port queued.
2026-11-20 B->A: Landed p25 0x00E37E40 NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Main (batches/2026-11-20_landing_p25_00e37e40).
2026-11-20 B->A: Landed p25 0x00D3A4A0 NScript::CQ_FireHeartScript::CFH_Prisoner::Main (batches/2026-11-20_landing_p25_00d3a4a0).
2026-11-20 B->A: Landed p25 0x00EEBDF0 NScript::CExpression_StealScript::Main (batches/2026-11-20_landing_p25_00eebdf0).
2026-11-20 A: landed nf148 (5 rows): 0x00994930 ctor on the PDB spelling with 0x00BF3B60's strong local copy removed, and one ThrowShotObject vtable across 0x009551A0 / 0x009566C0 / 0x00956960.
2026-11-20 A: landed nf149 (27 rows): the CIndexBuffer / CVertexBuffer names, plus 0x00BE91E0 respelled for NF152's GetIndexBufferMemPool and 0x009E2890 moved onto the CVertexBufferWin32 header. Self renames that leave the old leaf unbound land (advisor decision, owner delegated).
2026-11-20 A: landed nf150 (15 rows): CEngine forwarders and CRenderManagerCore draw-call names; explicit-instantiation scaffolding for private header members added to EXTRA_RULES (advisor decision, owner delegated).
2026-11-20 B->A: Landed p25 0x00D1A740 NScript::CQ_BountyHuntScript::CBH_Hostage2::Main (batches/2026-11-20_landing_p25_00d1a740).
2026-11-20 B->A: 0x00D3A4A0's noinline IsValid was labelled // Fidelity: but its COMDAT is not retail's (11v9): relabelled // Inline-budget: (owner decision 2026-11-08), comment-only, line count kept, recheck RELOCATION_MATCH. GOTCHAS Q32 (0x00EB3BA0) is the same kind of inline control.
2026-11-20 B->A: Relabelled 0x0041AFA0's FORCEINLINE_CLEAR note as // Inline-budget: (owner decision 2026-11-08) and its descriptive Fidelity line as a Note; comment-only, recheck RELOCATION_MATCH. Note: the navedge set was held earlier on a stale reading that #pragma inline_depth is banned; the 2026-11-08 owner decision allows it for inline-budget residues, so its auditor may choose between the pragma text and the loop-step text (pending/2026-11-20_winddown/followup_navedge).
2026-11-20 A: landed nf151 (23 rows): CShaderRenderManager / CEngineLightingManager names; the PrepareForRender one-definition debt (also at HEAD) queued.
2026-11-20 B->A: The owner delegated B's open decisions to the expert advisor; results in docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md.
For your items (released at B's wind-down; yours to decide) the advisor recommends: CVsyncCallbackTimer option A then B after the probe, never C;
CSoundEventManager by PDB layout with one owning header; hand3c3 b12's forced-inline admissible if marked // Inline-budget: with a flag attempt
recorded; navedge: the loop-step text over the pragma unless FableWin's line records put the step in the loop header; reports b6 is your call.
2026-11-20 A: fixed the catalog block fused by my b12 merge (0x0078D250 / 0x00DDAE40; both regraded RELOCATION_MATCH) and marked b12's Interpolate forceinline as Inline-budget per B's advisor recommendation.
2026-11-20 A: landed nf152 (8 rows): CEngineResourceManager getters and CEngineShadowRenderer names; 0x00B3B0A0 now has one spelling.
2026-11-20 B->A: Landed p25 0x00E25F70 NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main (batches/2026-11-20_landing_p25_00e25f70). Headers: engine/CQS_GuardianSisterInfoScript.h . Newly flagged: 16 rows (newly_flagged_rows.tsv).
2026-11-20 A: landed nf153 (24 rows): CPlayer / CPlayerManager accessor binders, with NF155's 0x00957F20 GetPos change; the CanFire family and 0x004C7CA0 ports queued.
2026-11-20 B->A: Landed p25 0x00E25F70 NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main (batches/2026-11-20_landing_p25_00e25f70). Headers: engine/CQS_GuardianSisterInfoScript.h . Newly flagged: 16 rows (newly_flagged_rows.tsv).
2026-11-20 A: fixed the catalog block fused by my b12 merge (0x0078D250 / 0x00DDAE40; both regraded RELOCATION_MATCH) and marked b12's Interpolate forceinline as Inline-budget per B's advisor recommendation.
2026-11-20 B->A: Landed p25 0x00EC4A00 NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main (batches/2026-11-20_landing_p25_00ec4a00). Headers: engine/CV_RockTrollFirstEncounterScript.h . Newly flagged: 32 rows (newly_flagged_rows.tsv).
2026-11-20 B->A: Landed p25 0x00E76E90 NScript::CV_ExposeMayorScript::HeroFlashesLamp (batches/2026-11-20_landing_p25_00e76e90). Headers: none.
2026-11-20 A: landed nf152 (8 rows): CEngineResourceManager getters and CEngineShadowRenderer names; 0x00B3B0A0 now has one spelling.
2026-11-20 B->A: Landed p25 0x00D0B3E0 NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main (batches/2026-11-20_landing_p25_00d0b3e0). Headers: engine/CQ_BanditCampBossBattleScript.h .
2026-11-20 B->A: Landed p25 0x00E7C010 NScript::CV_FishermanScript::NPCAttitudeProcess (batches/2026-11-20_landing_p25_00e7c010). Headers: none.
2026-11-20 A: landed nf154 (23 rows): CTCBase / CTCPhysicsBase / CTCLook / CTCCreatureNavigation names; the q11661 CTCLook pair held after three scaffolding forms.
2026-11-20 A: landed nf155 (18 rows): CThingCreatureBase / CThing / CThingManager names; two pre-existing link clashes removed, 30 unattempted port rows queued.
2026-11-20 B->A: Landed p25 0x00DD4AD0 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main (batches/2026-11-20_landing_p25_00dd4ad0). Headers: engine/CQ_PrisonEscapeRescueMotherScript.h .
2026-11-20 B->A: Landed p25 0x00E7C010 NScript::CV_FishermanScript::NPCAttitudeProcess (batches/2026-11-20_landing_p25_00e7c010). Headers: none.
2026-11-20 B->A: Follow-ups on landed p25 rows: 0x00EEBDF0 drops its unneeded MOVIEBASE_OUTOFLINE_DTOR opt-in (ablation; the TU's dtor COMDAT == retail 0x006E7B80), 0x00EC4A00's Inline-budget line now names all its devices; both recheck RELOCATION_MATCH PASS.
2026-11-20 A: landed nf156 (22 rows + 0x00485524): NUISystem names; 0x00485524's ctor tag set to retail's so both pick-any copies agree.
2026-11-20 B->A: Landed p25 0x00DD4AD0 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main (batches/2026-11-20_landing_p25_00dd4ad0). Headers: engine/CQ_PrisonEscapeRescueMotherScript.h .
2026-11-20 B->A: Landed p25 0x00E7C010 NScript::CV_FishermanScript::NPCAttitudeProcess (batches/2026-11-20_landing_p25_00e7c010). Headers: none.
2026-11-20 B->A: Follow-ups on landed p25 rows: 0x00EEBDF0 drops its unneeded MOVIEBASE_OUTOFLINE_DTOR opt-in (ablation; the TU's dtor COMDAT == retail 0x006E7B80), 0x00EC4A00's Inline-budget line now names all its devices; both recheck RELOCATION_MATCH PASS.
2026-11-20 A: landed nf155 (18 rows): CThingCreatureBase / CThing / CThingManager names; two pre-existing link clashes removed, 30 unattempted port rows queued.
2026-11-20 B->A: Landed p25 0x00DE8370 NScript::CQ_RansomVictimScript::CRansommer::Main (batches/2026-11-20_landing_p25_00de8370). Headers: none.
2026-11-20 A: landed nf157 (27 rows): NCreatureMode / NCameraModes names; the Jack of Blades mode moved into NCreatureMode as one family.
2026-11-20 B->A: Landed p25 0x00E17840 NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main (batches/2026-11-20_landing_p25_00e17840). Headers: engine/CQ_WhiteBalverineKnotholeGladeScript.h .
2026-11-20 B->A: Landed p25 0x00E72630 NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main (batches/2026-11-20_landing_p25_00e72630). Headers: engine/CV_DemonDoorsScript.h .
2026-11-20 A: landed nf158 (21 rows): GetParticleEmitterManager and the emitter-manager family on the PDB spellings, CPerItemData counted-pointer destructor from the shared header.
2026-11-20 A: landed nf160 (41 rows): engine resource / texture / input names; q6213 vector<CEnginePrimitiveHandle> on 14 rows; ~CDrawBase pick-any copy of 0x00658240 queued as DEBT (advisor).
2026-11-20 B->A: Landed p25 0x00E54E90 NScript::CV_BookCollectingScript::CBS_Teacher::Main (batches/2026-11-20_landing_p25_00e54e90). Headers: engine/CV_BookCollectingScript.h .
2026-11-20 B->A: Request (no rush): FableWin callee lists of the debug twins of 0x00E97070, 0x00E54E90, 0x00E72630 and 0x00DA0230 (generated quest scripts with an unread entry conditional-temporary flag store); a callee retail never binds would name the dead construct (FINDINGS_LOG 0x00E72630 entry).
2026-11-20 B->A: Comment-only label sweep (advisor): 0x00DA0230, 0x00E72630 and 0x00E54E90 now start their dead-statement label with '// Inferred: dead conditional temporary --' (line counts kept, recheck RELOCATION_MATCH PASS). Flagged-row counts missing from earlier log lines: 0x00E17840 5, 0x00E72630 5, 0x00E54E90 24 (newly_flagged_rows.tsv in each batch).
2026-11-20 B->A: Landed p25 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest (batches/2026-11-20_landing_p25_0055f710). Headers: none.
2026-11-20 B->A: Landed p25 0x00E97070 NScript::CV_HeroDollsScript::CBS_Teacher::Main (batches/2026-11-20_landing_p25_00e97070). Headers: engine/CV_HeroDollsScript.h . Newly flagged: 15 rows (newly_flagged_rows.tsv).
2026-11-20 A: landed nf162 (25 rows): NScript quest script names CQ_/CV_ A-M; LNK2005 9 -> 4 against HEAD.
2026-11-20 A: landed nf159 (47 rows): tavern game / carrying / AI creature names; 18 self renames.
2026-11-20 B->A: Landed p25 0x00ECE7F0 NScript::CV_SickChildScript::CWitch::Main (batches/2026-11-20_landing_p25_00ece7f0). Headers: none.
2026-11-20 A: landed nf161 (19 rows): memory allocator / weather / nav quad tree names; NF161 and NF158 open entries unclaimed for re-picking.
2026-11-20 B->A: Landed p25 0x0047FE83 CTCInGameMenu::GetFileNameList (batches/2026-11-20_landing_p25_0047fe83). Headers: engine/CTCInGameMenu.h .
2026-11-20 A: landed nf163 (10 rows): NScript quest script names CQ_/CV_ N-Z; 0x00DCF6E0 held for a new ??0CTeamSpawn clash.
2026-11-20 A: landed nf164 (16 rows): creature action names; RegisteredActionEvents initializer/destructor pair accepted with DEBT (advisor; EXTRA_RULES).
2026-11-20 B->A: tools 3905d7a8a: reloc_identity._OPS now holds every VC operator code (??B conversion, ??5/6 shifts, ??_0.._6 compound assigns, ...); ??B<Class> used to parse as a function named B<Class> and gave false RELOC_NAME warnings. No grade changes; only warning text.
2026-11-20 A: engine/CSpawnedFunc1.h now includes fable_compiler.h; 5 landed rows that failed to compile at HEAD now match (67/67 includers good).
2026-11-20 A: landed nf165 (19 rows): sound / stream names; GameCODA SDK names queued for the CSalSwBaseVoice family.
2026-11-20 B->A: Reserved 6 more parked rows for B (p25A ran out; 0x0064FDB4 and 0x00D01630 deferred with evidence in FINDINGS_LOG): 0x006495A8 CDrawScreenFade::Draw, 0x0070E8B0 CAnimateThing::UpdateFrameLogic, 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData, 0x005C2118 CPersistTraits<CItem>::TransferIn, 0x00C73050 Audio_PlaySourceAndProcessQueue, 0x00B94540 CEngineSceneMap::ResizeGrid. None is named by a claimed line; tell me if one collides with an NF lock.
2026-11-20 A: landed nf166 (32 rows): game component / world / animation names, ConstantFPS on 14 more rows; 0x00A99610 held for a new FindFirstObject clash.
2026-11-20 A: landed nf167 (23 rows): TC component names CTC A-H; five self renames.
2026-11-20 B->A: Landed p25 0x006495A8 NPlayerGui::CDrawScreenFade::Draw (batches/2026-11-20_landing_p25_006495a8). Headers: engine/CFadeInFadeOutBase.h engine/CTCQuestCompletionUI.h engine/CTCHeroReceiveItems.h engine/CSubtitleRenderer.h engine/CWorld.h engine/CDisplayEngine.h . Newly flagged: 6 rows (newly_flagged_rows.tsv).
2026-11-20 B->A: NAME_FIXES from p25 0x006495A8: 0x005E3E70 is ?IsOverridingScreenFade@CTCQuestCompletionUI@@QBE_NXZ (thiscall member, Ego_r/FableWin PDBs); the landed row is a free __fastcall on a local pad struct. Yours to respell (batches/2026-11-20_landing_p25_006495a8/NAME_FIXES.tsv).
2026-11-20 B->A: (1) 0x006495A8 comment-only amend: its line 2 now names its two header devices (IS_THING_ALIVE_NOINLINE, CTHING_GETTC_EXPANDED); strict --recheck-landed RELOCATION_MATCH. (2) Naming for you: 0x013B89C0 is g_bSaveLoadFailed in three landed rows and in 0x006495A8, but 0x00499D90 CProgressDisplay::StopProgress spells it g_ProgressBlackScreenB.
2026-11-20 A: landed nf168 (31 rows): TC component names CTC I-Z; subtitle rows on the real STLport map, LNK2005 7 -> 3 against HEAD.
2026-11-20 B->A: Landed p25 0x00B94540 CEngineSceneMap::ResizeLevels (batches/2026-11-20_landing_p25_00b94540). Headers: engine/CEngineSceneMap.h . Newly flagged: 6 rows (newly_flagged_rows.tsv).
2026-11-20 B->A: NAME_FIXES from p25 0x00B94540 (batches/2026-11-20_landing_p25_00b94540/NAME_FIXES.tsv): 0x00B94F90 CCountedPointer<CEngineSceneGrid>::DeleteData (possible ICF fold with the CEngineSceneMap one), 0x00B95A30 CCountedPointer<CEngineSceneGrid>::Reset, 0x00B96920 an STLport __destroy_aux (one source, candidate). The row itself is ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z (Ego_r), not the catalog's ResizeGrid; its oracle row is 664 B (the old 672 included 8 B of int3 padding before Update 0x00B947E0).
2026-11-20 A: landed nf169 (26 rows): creature action / AI state group names; seven self renames.
2026-11-20 B->A: Landed p25 0x00E9E420 NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main (batches/2026-11-20_landing_p25_00e9e420). Headers: none.
2026-11-20 A: landed nf170 (22 rows): GetSubDef binders / owners on the shared header, search tools and Transfer<T> names; LNK2005 5 -> 1 against HEAD; 0x007D7EC0 found DIFFER at HEAD (queued).
2026-11-20 B->A: Landed p25 0x0070E8B0 CAnimateThing::Update (batches/2026-11-20_landing_p25_0070e8b0). Headers: engine/CAnimateThing.h engine/CAnimSet.h . Newly flagged: 5 rows (newly_flagged_rows.tsv).
2026-11-20 B->A: NAME_FIXES from p25 0x0070E8B0 (batches/2026-11-20_landing_p25_0070e8b0/NAME_FIXES.tsv), incl. a labels-DB respell 0x0129BA3C g_NormaliseEpsilon -> DELTA_NOTIONAL_ZERO. The row is ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z, not the catalog's UpdateFrameLogic.
2026-11-20 A: landed nf171 (17 rows): CThing / player GUI names; 0x00446B10's self rename follows in nf171b.
2026-11-20 A: landed nf171b (1 row): 0x00446B10 self rename to CGamePlayerInterface::IsGameActionEventInQueue.
2026-11-20 B->A: Landed p25 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn (batches/2026-11-20_landing_p25_005c2118). Headers: engine/CPersistTraits.h engine/NInventory_CTCInventoryBase.h . Newly flagged: 2 rows (newly_flagged_rows.tsv).
2026-11-20 B->A: NAME_FIXES from p25 0x005C2118: 0x005BEC40 is ??0CItem@NInventory@@QAE@JJPBVCPerItemData@1@J@Z (landed as a global ::CItem ctor; batches/2026-11-20_landing_p25_005c2118/NAME_FIXES.tsv).
2026-11-20 A: Landed nf172 (NF172 render / shader / texture / engine names, 21 rows; batches/2026-11-20_landing_nf172). Headers: engine/CEngineStaticMapFileBlockManager.h CEngineCamera.h CEngineResourceManager.h CEngine.h CEngineScreenEffectRadialBlurRenderer.h CGPUScratchBufferTextureManager.h (add-only opt-ins). q5054 / q11918 / q11978 back in the queue.
2026-11-20 A: Landed nf173 (NF173 mixed engine / TC names, 36 rows; batches/2026-11-20_landing_nf173). Header: engine/CNavigatorManager.h (ADDNAVIGATIONMAP_PROCESSREF). 14 entries back in the queue; ??0CAnimInit LNK2005 (HEAD-existing) queued as DEBT.
2026-11-20 B->A: fix in engine/CRenderStateManager.h (found by p25B on 0x00B24850): 524a0abf9 (2026-11-13, SETRENDERSTATE_DESIRED) cut the EARLY_RETURN branch's two closing braces and its #else, so with EARLY_RETURN defined the default SetRenderState was defined inside that one and c1xx hung. Restored as '        } }' + '#elif defined(...SETRENDERSTATE_DESIRED)' on the same two lines (line count kept); hdrcheck2 same on default and every macro. No landed row defines EARLY_RETURN.
2026-11-20 B->A: Reserved 4 more parked rows for B (p25C): 0x007BD2D1 CTCShotDefinitionThrow::OnSubShotTrigger, 0x00859EF0 CCombatActionBase::OnAnimationLoaded, 0x00E5E350 CV_BodyGuardScript::HenchmanCommentary, 0x008DBC80 CModeDragonFlying::FrameUpdate. 0x00A4EFC0 parked (FINDINGS). Tell me if any collides with an NF lock.
2026-11-20 A: Landed nf174 (NF174 script / UI / creature mode names, 6 rows; batches/2026-11-20_landing_nf174). Header: engine/CV_ArcheryCompetitionScript.h (CTOR, declaration only). 14 open entries back in the queue.
2026-11-20 A: Landed nf175 (NF175 creature action / AI / hero names, 11 rows; batches/2026-11-20_landing_nf175). Headers: CScriptedMapBrush.h CCombatActionBase.h CCreatureAction_ThrowShotObject.h CCreatureActionBase.h (add-only opt-ins). GameCODA voice entries deferred under the vendor LIGHT-renaming rule; cut-scene cast-map family handed to NF178.
2026-11-20 B->A: Landed p25 0x00B24850 CEngine::SetDefaultRenderStates (batches/2026-11-20_landing_p25_00b24850). Headers: none.
2026-11-20 B->A: fix in engine/C2DVector.h (found by p25B on the 0x00A1E5C0 draft): with FABLE_RETAIL_NAVIGATORASTAR_PASS21 defined, operator*(float) was declared twice (C2535), because the PASS21 block ended in #endif and the NAMED_RESULT_MUL #ifdef/#else pair followed. That pair is now an #elif of PASS21 (same two lines, line count kept). Default and NAMED_RESULT_MUL preprocess identically; no landed row defines PASS21.
2026-11-20 B->A: NAME_FIXES from p25 0x00B24850: label 0x01396DC8 g_textureUVModeToD3D is ?D3DTextureUVModeTable@CRenderStateManager@@0PAKA (Ego_d public, reloc_bindings; labels_codex_pass13 already has it). batches/2026-11-20_landing_p25_00b24850/NAME_FIXES.tsv.
2026-11-20 B->A: Landed p25 0x007BD2D1 NThingShot::CTCShotDefinitionThrow::OnSubShotTrigger (batches/2026-11-20_landing_p25_007bd2d1). Headers: none.
2026-11-20 B->A: NAME_FIXES from p25 0x007BD2D1 (batches/2026-11-20_landing_p25_007bd2d1/NAME_FIXES.tsv): labels-DB respells 0x00A14040 RotateXY (labels say RotateAboutZ) and 0x004C73D0 GetPos (labels say GetPosition). FYI: Ego_d (method list 0x9B49) marks CRightHandedSet's copy ctor public compiler-generated; landed 0x0049CF80 and the global_E*ORIENTATION rows (0x0121AC30/ACD0/AD20/AD50) model it as a user ctor in local classes.
2026-11-20 A: Landed nf176 (NF176 long-tail engine / sound DSP / navigation names, 18 rows; batches/2026-11-20_landing_nf176). Header: engine/CMeshDataBank.h (CLOSE, declarations only). 15 open entries back in the queue; q4404 deferred under the vendor rule.
2026-11-20 B->A: Landed p25 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity (batches/2026-11-20_landing_p25_00a1e5c0). Headers: none.
2026-11-20 B->A: NAME_FIXES from p25 0x00A1E5C0 (batches/2026-11-20_landing_p25_00a1e5c0/NAME_FIXES.tsv): catalog MoveTowardGoal -> CalculateAndMoveToPosProximity (landed under the PDB name); 0x00A77780 -> ?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z; 0x006CD240 'CGuiControlTreePane::SortTreeRecursively' is a made-up name on an ICF-folded STLport list::size body (this row binds list<CNavWayPoint>::size there) -- yours to name; 0x00A1A1A0 landed as `long ... const`, both PDBs say ?IsCurrentNavigabilityBetter@CSteeringAgent@@QAE_NXZ (bool, non-const). The row's same-TU copy uses the PDB spelling and its COMDAT is byte-equal to retail 0x00A1A1A0 (35 B, cmpcomdat), so fidcheck's 'copy differs' is the landed row's spelling, not the copy.
2026-11-20 A: Landed nf177 (NF177 returned render / particle / TC entries, 32 rows; batches/2026-11-20_landing_nf177). 0x009A80B0 held under the pick-any rule (less<CCharString> copy differs from 0x0042AAB3). Headers: fable_retail_banks.h CBankFile.h CTCWeapon.h CEngineStaticMapFileBlockManager.h landscape_background_lod_compiled.h.
2026-11-20 A: Landed nf178 (NF178 + NF178b cut-scene cast-map family, 30 rows, narrowing partial; batches/2026-11-20_landing_nf178). Header: stlport/fable_map.h (MERASE_DECL_ONLY, DTOR_DECL_ONLY, MAP_CLEAR_DECL_ONLY; add-only). 4 rows held; the rest of the family queued.
2026-11-20 B->A: Landed p25 0x00E5E350 NScript::CV_BodyGuardScript::HenchmanCommentary (batches/2026-11-20_landing_p25_00e5e350). Headers: none.
2026-11-20 A: Landed nf180 (NF180 persist traits / counted pointer / STL container names, 15 rows; batches/2026-11-20_landing_nf180). 0x0083EEE0 held under the pick-any rule (vector<char>::_M_fill_insert copy differs from 0x004110D0). Header: stlport/fable_containers.h (SORT_FINAL_INSERTION_OUTOFLINE).
2026-11-20 B->A: The 25 reserved rows end at 22 landed + 3 deferred (0x0064FDB4, 0x00D01630, 0x00B0DC20; evidence in FINDINGS_LOG; they stay reserved acctB for a return pass). Reserved 4 more parked rows for B (p25F): 0x00EFA4F0 Cbandit_level_cutsceneScript::Main, 0x00714370 CActionMoveTo::OnActivate, 0x005BFD5E CTCInventoryBase::OnSerialise, 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle. Tell me if any collides with an NF lock.
2026-11-20 A: Landed nf179 (NF179 returned script / TC / player GUI / UI entries, 17 rows; batches/2026-11-20_landing_nf179). CTeamSpawn family on one inline header ctor (settles nf163's held 0x00DCF6E0); 0x00437570 const_cast accepted pending the CPlayerGui.h Graphics header claim; 14 entries not reached go back to the queue.
