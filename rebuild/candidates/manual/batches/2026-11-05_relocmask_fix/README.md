# 2026-11-05: reloc-masked false matches re-landed (0x009F1720, 0x007E37C0) and one stand-in respelled (0x009F16C0)

Plan phase 5 reported 0x009F1720: the landed row stored a vtable relocation where retail stores literal 0. The grader masks
relocation fields, so the row graded RELOCATION_MATCH with the wrong code. A whole-catalog audit
(`tools/decomp_pipeline/reloc_mask_audit.py`) found one more such row. Both are re-landed as their real functions and now
grade plain MATCH, so no relocation field is involved any more.

| Address | Was | Now | Grade | Evidence |
|---|---|---|---|---|
| 0x009F1720 | `??1T@@QAE@XZ` (stand-in dtor storing `??_7T@@6B@`) | `CGameEventPackageSet::Clear` | MATCH PASS | Ego_r.pdb + FableWin.pdb `?Clear@CGameEventPackageSet@@QAEXXZ`; `NoPackages` (+0x00) = 0; the caller 0x004AEAA0 clears the set before AddPackage |
| 0x007E37C0 | `CVtable_ffffffff::CVtable_ffffffff` (stand-in vtable store) | `CDefString::CDefString` | MATCH PASS | Ego_r.pdb + FableWin.pdb `??0CDefString@@QAE@XZ`; `TablePos` (+0x00) = -1; 35 NScript callers (0x00F2EA95..0x00F2F55B) build the default of CPersistContext::Transfer<CDefString> |
| 0x009F16C0 | `C009f16c0::M` (genuine, generic stand-in) | `CGameEventPackage::Clear` | MATCH PASS | Ego_r.pdb + FableWin.pdb `?Clear@CGameEventPackage@@QAEXXZ`; `NoEvents` (+0x04) = 0; already bound under that spelling by 0x004AEAA0 |

- **Headers:**
  - `engine/CGameEventPackageSet.h` gains `Clear()` behind FABLE_RETAIL_CGAMEEVENTPACKAGESET_CLEAR.
  - `engine/CDefString.h` gains an out-of-line default ctor declaration behind FABLE_RETAIL_CDEFSTRING_DEFAULTCTOR_OUTOFLINE.
  - hdrcheck2 reads `default=same` on both.
- **0x009F16C0** has no shared header, so it is modelled locally with its PDB member names.
- **The caller 0x004AEAA0** is respelled from `Initialise` to `Clear`, and its `reloc_bindings.tsv` line too. It still grades
  RELOCATION_MATCH PASS.
- **Tests:** all three got real behaviour tests. The old ones only printed PASS.
- **Labels:** `ghidra_out/labels_fix_relocmask_2026-11-05.tsv`.
- **The audit rule:** a dir32 relocation whose retail dword lies outside the image is a constant, not an address. It excludes
  `__except_list` and funclet rows indexed as their parent function. Its only other suspect, 0x00A8E160, was checked by
  compiling the row: it is clean.
