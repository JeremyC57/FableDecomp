# Chunk 680 -- ChatGPT pass 10's 43 unmatched rows retried on our lane: 20 landed, 23 parked (2026-11-01)

Started from ChatGPT's best sources (prior/), the Fable 5.1 agent landed 20 rows byte-exact (1 MATCH, 19 RELOCATION_MATCH; all tests PASS):
TransformForcedOpinion, CStreamingFontBank::FindGlyph, SetupSummonSpell, C3DMeshStats::HasHelperPoint, CTCInventoryBase::OnCreate (/O1 /Oy-),
CThing::IsUnderScriptSupervision, ~PKCProtectionInstance (/EHsc), RemoveMessageEventsAdded (label was CheckAndRemove), the three CFileInstaller
rows (owner-allowed `volatile` ExitThread with `// Retail-volatile:` evidence lines; the behaviour tests mirror the member and carry the same line),
DetectWindowsVersion (/GS), CShape::UpdateSplit, PushUpManualCameraList, UpdateEmitterThingAttachment, RenderOcclusionBox, ClockSprite::Update,
CCampHostageDoor::Main, and two rows whose reserved spans were defective: CRotateAround::OnInit (6 -> 295 bytes) and
CCreatureAction_SpiritAttack::FrameUpdate (21 -> 566 bytes), landed at their true extents in a second pass (`verify_pass2.log`; BOUNDS_FIX.tsv rows,
with the parked 0x00863CA0 at 53 -> 358). Harness note repeated: vland's `vc71()` deletes every test line containing `static_assert`; the token was
renamed in the tests. Labels: `ghidra_out/labels_chunk680_2026-10-27.tsv` (20 rows + 11 corrections from labels_pending.tsv).

Decisions at landing (coordinator, under the owner delegation):
- 0x0129BA3C is `?DELTA_NOTIONAL_ZERO@@3MB`, not EPSILON: the raw Ego_r public + typed data record sits immediately before GD3DFormats (which we
  verified independently at 0x0129BA40), the value 1e-4f is used squared, and the Ego decompile carries the assert text "< DELTA_NOTIONAL_ZERO".
  EPSILON (wave 13) was a style guess. ESTABLISHED_SPELLINGS updated; the 49 TUs are respelled in naming wave 18 (c680 queue row).
- 0x00C6B260 salFrameFloatToInt: retail's loops are the GameCODA SDK's inline asm (SalFrame.obj labels `$sse2FtoiLoop$21115` / `$sseFtoiLoop$21127`)
  -- eligible for grade retail_asm under the owner exception (an instruction VC7.1 cannot emit from C++, `cvtps2dq`), with a `// Retail-asm:
  0x00C6B2B0 cvtps2dq xmm0,[eax]` line. The agent was barred from `__asm`; parked for a dedicated retail_asm retry (a C++ body with those loops in
  `__asm`, no naked/_emit/ret inside), reviewed for purity at landing.
- Items for naming wave 18 (c680 queue): landed 0x00C73770 calls 0x00C73660 as SetDefaultString_helper (wrong signature); landed 0x009C4F20
  declares ExitThread non-volatile; the landed "CrossProduct" 0x00C25860 is probably the PDB Cross (prove first); 0x0063A300 / 0x0063A390 share a
  label; the CODA context global 0x0143BB58 and the libpk names need one spelling each.
- The ChatGPT best-attempt copies (`pending/2026-10-31_chatgpt_pass10_differ/`) are superseded by this chunk's differ/ (every row either landed or
  has a smaller-residue source here) and were removed.

Parked (23, best sources in `../../pending/2026-10-27_linux_hand680/differ/`, line 2 = grade + residue, each with a next step): register / frame-slot /
x87-order residues of 1-14 bytes on 21 rows (GetResourceCounts, CAnimationSet::GetAnim, CMusicManager::PlayMusic, IsResourcesLoaded,
UpgradeTrainableStat, AttackHero, BackplaneFriction, Draw2DLine, HeightAreaChanged, CalcObjectMatrix, GetHeightZBelowAt,
FillBodyReorientationPossibleIntersecting, CMurderTwistAssassin::Main, GetAttitudeSellPriceMultiplier, GetRandomCard, CTCTalk::PlaySpeech,
CancelForCutscene, DatabaseSerialise, ApplyTattooCard, FastBuildFromPoints, MinionGuardReactToBodyFoundMessage::Update, CEditBox::KeyPressed) and
salFrameFloatToInt (retail_asm). NAME_FIXES.tsv: 13 rows (queued c680). Lessons in LESSONS.md.
