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
