# acctC workflow hand-back wfA1b (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00C271F0 | _global::CTCInventory_OnItemEquippedHelper | 846 | RELOCATION_MATCH (`/EHsc /MD`) | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00C271F0
Verifier audit: None. Grep finds no asm, naked, _emit or volatile, no inline control and no dead calls or locals. Flags are /EHsc /MD, with evidence for /MD: ceil is called through the IAT at 0x00C2738B. Strict headers passed with no HEADER_POLICY result. The local libpk models have no engine header and match the landed siblings' style. hdrdiff is one additive declaration-only block, FABLE_RETAIL_C3DVECTOR_SWAPYZ (C3DVector::SwapYZ, 0x00C1C070), listed in hdr/README.md. The log has no RELOC_IDENTITY, RELOC_CONFLICT or OUTSIDE_MANIFEST, but has 11 RELOC_NAME warnings where libpk callee manifest labels are propagated nonsense. Review items: (1) The class at vtable 0x012B4034 is named CPhysicalPlayer here but CCodaPlayback in landed 0x00C27550, 0x00C27620 and 0x00C276B0; the author flagged it in NAME_FIXES.tsv. (2) The test runs about 15 real checks over 4 scenarios, but on a hand-written portable model of the row, not on the compiled row itself.

Fitted items: Source line 2 (Closer): event position read through a named const reference, const C3DVector& eventPosition = desc->m_position; then position = eventPosition.SwapYZ(). Also descriptive callee names marked FITTED in NOTES: CODA_SetSourceEnvelope 0x00C35B30, CODA_CreateSample 0x00C36080, CODA_SetSourceRangeCallback 0x00C33F20, CODA_SetSourcePitch 0x00C35B10, FullRange 0x00C271B0, UpdateManagerTime 0x00C271E0, CSoundEventManager::GetTime 0x00C11700, CPlayerList::Add 0x00C21E90, CSoundTicker::Update 0x00C1C460, CStreamBuffer::GetData 0x00C26240. The class name CPhysicalPlayer is descriptive and taken from the reloc binding.
