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
