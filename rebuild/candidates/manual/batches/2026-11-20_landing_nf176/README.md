# Landing nf176 (Account A, 2026-11-20): long-tail engine / sound DSP / navigation names (18 rows)

Agent NF176 (round 10) handed back early (context) with 55 entries: 10 done, 13 done-already, 5 rejected, 8 blocked, 4 deferred,
15 open. Its files are in `naming/2026-11-20_agentNF176/` (with the family drafts and census witnesses under `work/`). The 15 open
entries go back to the queue unclaimed.

- 18 rows. Self renames (4): CMeshDataBank::Close 0x00A26C50, AddLogBookEntry's PDB virtual-const leaf 0x0088FE00, CImaAdpcmCodec::
  CalcDifference 0x00C5F150 (was CalculateError), CCompressorZlib::DecompressStream 0x00A3A840 (was a CCompressorZlib_Retail stand-in).
  Every exe-scan referencer already binds the new spelling or moves here. The EH funclet 0x012119E5 of 0x00C14DD0 was regraded on the
  new parent by the agent (RELOCATION_MATCH).
- Header: engine/CMeshDataBank.h (`orig` equals HEAD) gets FABLE_RETAIL_CMESHDATABANK_CLOSE, declarations only; pin_asserts pinned 0.
- No casts of stand-ins to real types. One new local model, `struct CNavigatorSwitch` in 0x00C14DD0: the class has no shared header,
  and its self row 0x00C2A890 uses the same shape.
- Coordinator calls:
  - 0x00502170 (q3886) makes Close virtual in place on the row-local CBankFileAsync model, so its pick-any stand-in `??_7CWADBank@@6B@`
    grows 4 -> 8 bytes. That stand-in vtable already differed from 0x0050B420's at HEAD (slot 0 `??_ECWADBank` vs `v00`), so it lands
    under the HEAD-counterpart rule for stand-in data COMDATs; porting both rows onto engine/CWADBank.h is queued.
  - Pick-any function copies (advisor rule, EXTRA_RULES 2026-11-20): the only newly defined symbol bound to another address is
    0x00408A70's `?GetAsCharArray@CCharString@@QBEPBDXZ` (owner 0x00403A10). Its 15 bytes equal the owner's body, and its one relocation
    targets the same empty-string data under another name, so it is allowed.
  - q4404 (CAudioContext -> CSalOpenAL, 50 rows) is deferred: GameCODA vendor code gets LIGHT renaming only (owner, 2026-11-12).
  - The AddLogBookEntry category type in engine/CGameScriptInterface.h is queued as a header claim.
- Recheck (`logs/recheck.log`): 18 RELOCATION_MATCH, every test PASS. 0x00495890 warns that its Leaf line matches no symbol: the line
  carries a trailing comment and the private spelling while the row models the member public. That is unchanged from HEAD (queued).
  rebind multi-check 1 (baseline); split gate 0 created; includers 239 good, 0 bad; owner recheck 13 of 13 good. tablecheck: eight
  addresses shrink, none grows.
- Link (`logs/link_landing.log`): 4 LNK2005, the same four as HEAD (`logs/link_control_head.log`).
