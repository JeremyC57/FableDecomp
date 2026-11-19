# Landing p989 b3 + hand3c b24 with the CCoda3DSource Calc* owners + hand4d b12 BuyItem (Account B, 2026-11-19)

Eight rows, all RELOCATION_MATCH with their tests passing under strict vland `--land`, after an independent audit (`audit_REPORT.md`).
They also regrade on the merged tree (`logs/grade_merged_tree.log`).

| address | row | kind | notes |
|---|---|---|---|
| 0x00989230 | CShaderRenderManager::UpdateObjectSpaceTextureTransform | new | p989 b3; see below |
| 0x00C41FB0 | CCoda3DSource::Commit | new | hand3c b24; same-TU strong copies of the five Calc* rows, each verbatim with its Fidelity line |
| 0x00C418F0 | CCoda3DSource::CalcDirectionAndDistance | re-port | was CLibpkObject1D8::NormaliseCopy (a stand-in model) |
| 0x00C41990 | CCoda3DSource::CalcDistanceGain | re-port | was CCombatant::ComputeDamageResistance |
| 0x00C41A20 | CCoda3DSource::CalcDoppler | re-port | was CCombatant::ComputeLightingAdjustment |
| 0x00C41A70 | CCoda3DSource::CalcConeGain | re-port | was CCombatant::ComputeEffectiveRange |
| 0x00C41BB0 | CCoda3DSource::CalcEnvFxGain | re-port | was CCombatant::ComputeSpellPower |
| 0x00774D20 | CTCShop::BuyItem | new | hand4d b12; see below |

## 0x00989230: advisor decision (owner delegated)
A row-named nested opt-in `FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230` inside `..._EGOD` (engine/CMatrix4x4AlignedSSE.h)
adds `const CMatrix4x4& r = rhs;` to the x87 arm. It is marked `// Inferred:`: VC7.1 orders the four x87 products of each element by a
mod-4 count of live symbols, and the row's Ego_d text is one unit short. The unit table and the candidates tried are in
author_README_p989_b3.md and ESCALATE_00989230.md.
- The audit reproduced the unit evidence and corrected two overclaims in the comment.
- No other file defines the macro. All 11 landed expanders preprocess raw-identical and regrade RELOCATION_MATCH.
- Account A's FableWin callee list (0x030381B0: ctor, AlignedSetMultiplyTransposed, SetVSConstants with only error-path callees)
  confirmed the inline layers the row already uses.

## 0x00C41FB0 Commit and the Calc* owners
The landed owner rows modelled CCombatant / CLibpkObject1D8 stand-in classes, so GOTCHAS 455 ("owner body verbatim") could not hold
for Commit's same-TU copies. The five owners are re-ported onto CCoda3DSource using exactly the copies' text, with locals from the
CODAApiDbg CodeView. The SDK release COMDATs are byte-identical to retail for four of the five. Commit's two required copies are
CalcDoppler and CalcConeGain (1574 / 1568 v 1570 without them). The other three are kept as retail TU-mates under the same Fidelity lines.
New header engine/SalMath.h; additive opt-ins in CCoda3DSource.h, CCodaContext.h, CCodaListener.h, CCodaObject.h and CCodaVector.h
(today's NF95 OrthogonalRight block preserved). ppcmp: 7,678 transitive includers preprocess raw-identical.

## 0x00774D20 BuyItem
The row closed on Ego_d.pdb's per-line table, carved from the raw file because the DBI is broken (GOTCHAS, last line).
`egod_buyitem_lines.tsv` has 52 statements with debug sizes. Two changes closed it:
- a single four-term `if` at Ego_d lines 842..844 flips c2's K25 allocation decision;
- `value -= Min - 1` written first removes the MinGoldForDonation CSE.
The rest of the record:
- **Bracket:** one byte-required grouping bracket on `morality = (morality_adjustment * log_value)`, marked `// Bracket:` per the owner's
  2026-11-19 bracket rule. It fixes `mov ecx,esi` at 0x007750E5. The alternative `1.0f - (ptc_stats->GetMoralityPercentage())` also matches.
- **Unreproduced:** Ego_d lines 894 (10 bytes) and 898 (17 bytes) are noted as `// Unreproduced:` in the row; they may be debug-only code.
- **Same-TU copy:** GetFreeSaleItemPlaces (0x00774B40) is now the landed owner's body verbatim, with its Fidelity line. The audit fix was
  needed because the author's copy was a different text; without the copy the row gives DIFFER 1153v1149.
- **Opt-ins:** the two stlport/fable_map.h opt-ins it uses already exist; no header change.

## Purity tag lines (comment-only; audit option b)
- libpk/FtoL.h `// Retail-asm-helper:` adds `sal_ftoi, sal_round`, so Commit (four fistp sites via SalMath.h) grades retail_asm.
  The 35 FtoL.h includers are unchanged.
- engine/CMatrix4x4AlignedSSE.h adds `AlignedSetMultiplyTransposed, AlignedPreMultiplyTransposed`. 0x00989230 and the five landed users
  (00a9e1e0, 00adefc0, 00ae1320, 00ae29d0, 00bd2d90) move from `genuine` to `retail_asm`, which is honest: they expand the SSE asm arm.

## For Account A (NAME_FIXES.tsv)
- The six CODA symbols, plus sal_gainToLevel 0x00C40F50 and sal_ftoi 0x00C3FF70.
- The globals 0x0143BBF8 / 0x0143BBF0 (CODA m_fRolloffFactor / m_fDopplerSpeed); their two binder rows went away with repair_remove.
- BuyItem (catalog CTCShop_SellHighlightedItem).
- Class-level: the CLibpkObject1D8_* setters and 0x00C41E40 SetCombatParameters are CCoda3DSource-layout rows.

## Follow-ups (Account B queue)
- 0x009FDE80 (e17) is audited (fixed set ready). It is held because its new engine/C3DExtentsI.h needs nine landed local-model rows
  re-ported first: 00a67a20, 00a685e0, 00a67860, 00a67950, 00a67530, 00a699d0, 00a677d0, 00a68910 and 009fd9b0.
- Re-port 0x0098B930 SetVSConstants (a view-struct stand-in) in Ego_d spelling, then make the in-TU copies in 0x00989CB0 and 0x00989230
  verbatim.
- 0x00774C60 StockDisplay carries a different GetFreeSaleItemPlaces text with no Fidelity line (pre-existing; GOTCHAS 455).
