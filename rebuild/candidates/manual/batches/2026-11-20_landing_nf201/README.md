# Landing nf201 (Account A, 2026-11-20): returned NF197 entries, radial blur, STLport list / vector instances (17 rows)

Agent NF201 (round 10) handed back early (context) after q11797 with 50 entries: 11 done, 1 done-already, 7 blocked or partial, 5
deferred and 26 open. Its files are in `naming/2026-11-20_agentNF201/`, with the held 0x008E9850 draft (`held/`). Most entries go back
to the queue unclaimed (37): the open ones; the partial families carried from NF197 (q5052, q5064, q5138, q6196) and q11692 / q11793;
and the deferred ones, whose rows NF199 has since landed (q4860, q6309, q11760, q11873) or NF200 holds pending landing (q11772). q11764
stays blocked on an identity decision.

- 17 rows land: 1 MATCH (0x00987D70) + 16 RELOCATION_MATCH, every test PASS. The recheck also regraded the held 0x008E9850 from its
  unchanged HEAD source (RELOCATION_MATCH); no file of it changes.
- Self renames (RENAMES.tsv section 1, 10 rows), each on both PDBs:
  - 0x00890150 `CGameScriptInterface::RadialBlurFadeOut` (`UBE`), the row nf197 held, now on the shared headers with no overlay struct.
  - 0x0079C750 `CTCBulletTime::PlayCreatureOnHitEffect` and 0x00B5AF60 `CEngineScreenEffectColourFilter::Render`.
  - STLport instances: 0x007464A0 / 0x007467D0 `std::list<CEnginePrimitiveHandle>` insert / push_back; 0x00A80360 the
    `std::vector<C2DBoxF>(unsigned)` ctor.
  - 0x00681BA0 `CTCCreatureOpinionOfHero::SerialiseFlagMask` (private), 0x00987D70 `CShaderConstantRange::operator!=`, 0x00F2D660
    `CPersistContext_TransferableComponent<EFeatAttackType>::TransferOut`, 0x009CC270 `CBankFile::GetEntryDataOffset` (re-authored as
    pick-any scaffolding over NF139's header body).
- Binders respelled: seven q11692 rows from `SetGroup` to the self row 0x0073DFF0's `?SetCurrentGroupName@CAnimFilterOfGroup@@`, of
  which six land (0x0073AAC0, 0x008D75D0, 0x008D87C0, 0x008D9220, 0x008D92C0, 0x008D9350); 0x007AB190 respelled in place.
- Held at landing: 0x008E9850. The agent's draft moves its in-class SetGroup body onto the owner's `SetCurrentGroupName` spelling. The
  bytes equal the owner's, but one relocation (+0x23) points at a `""` literal where the owner binds `?NullChar@CCharString@@1DB`. The
  pick-any rule accepts a copy only when its bytes and its relocation targets both match (every earlier acceptance had equal
  relocations); a different target is a different image if the linker keeps this copy. The agent offered the drop. The fix (move the
  row off its local CCharString) is queued.
- Headers: add-only, declaration-only `#ifdef` opt-ins; `orig` equals HEAD for all four; no landed includer defines them
  (`naming/.../hdr/README.md`).
  - engine/CEnvironmentSpellEffectInterpolator.h: RADIALBLURFADETO_VECTOR (the C3DVector overload, 0x006B8550).
  - engine/CEnvironmentSpellEffectInterface.h: SETRADIALBLURCENTERWORLDPOS (0x00885110).
  - engine/CIVCountedPointer.h: COUNTED_COPY, the counted copy constructor without the RETAIL block's Release / Reset bodies.
  - engine/CEngineScreenEffectColourFilter.h: RENDER (the CEngineComponent::Render override, no new slot). pin_asserts rewrote its 20
    asserts to `FABLE_STATIC_ASSERT_AT` with their HEAD line numbers.
- Pick-any (`logs/pickany_check.txt`):
  - `_List_iterator_base` in 0x007464A0 / 0x007467D0 equals the owner 0x0041C780 (11 = 11).
  - The MISSING lines are 0x00681BA0's CPersistContext / CCharString inline copies against owners whose object lacks that spelling. The
    coordinator graded them with q.py, the row's source at each owner address: 0x0040E1E0, 0x00411010 and 0x00403A10 RELOCATION_MATCH;
    0x00403A00 and 0x004AEF50 MATCH. 0x007464A0's CEnginePrimitiveHandle copy ctor grades RELOCATION_MATCH at 0x0043F450, whose
    catalog name (C3DAnimationBlendState) is wrong: NAME_FIXES queues `??0CEnginePrimitiveHandle@@QAE@ABV0@@Z` (both PDBs).
- Table hygiene: three stale lines at 0x00410DE0 are dropped: `?test@CBitset@@QBE_NK@Z` (0x00659104), `?test@CTCBitset@@QBE_NK@Z`
  (0x007890EA) and `?test@CTCBitset@@QBE_NJ@Z` (0x0064A568). All three rows bind `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` since naming wave
  18 (their sources, and the agent's HEAD probes).
- Link (`logs/link_landing.log`): 0 LNK2005, as at HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Gate and referencing rows 74 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink (0x0129AAF4 19 -> 18, 0x006B8550 3 -> 2, 0x006B82E0 4 -> 1, 0x0099F180
  2 -> 1). 0x0073DFF0 grows 2 -> 3: 0x0073AAC0 moves onto the self row's own Leaf (narrowing). The SetGroup line stays until 0x008E9850
  and 0x008D9530 move. No out-of-image oracle line.
