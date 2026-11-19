# hand4f batch 5 (2026-11-19, Account B): 0x00A0CA90, 0x006CA8C0, both RELOCATION_MATCH + test PASS, landed

**Account B, hand-off continuation. Both rows were authored, then independently audited, then re-verified by the coordinator in strict vland.**

## 0x00A0CA90 `?InitAsFloat@CWParsedItem@@QAEXPBGK@Z` (187 bytes)
- Built from the landed narrow twin 0x009B9770, widened to wide text.
- Locals match Ego_d exactly: `ch`, `top`, `bottom`, `bottom_divide`, `doing_top`, `is_negative`, `c1`.
- Literal types follow retail: `10.0f` for the whole part, `10.0` for the fraction, `-1.0` for the sign.
- Headers are the shared engine/CParsedItem.h and engine/CWParsedItem.h. No opt-ins, no flags.
- `wchar_t` is correct as written. The shared header declares it, and VC7.1 without /Zc:wchar_t decorates it G. That is the owner's 2026-10-29 rule for PDB `_W` names.
- **Coordinator decision:** keep the `if (false) {} else for` spelling.
  - The audit measured that a plain `for` gives the same bytes, so the spelling is not byte-required.
  - It is kept to match the landed twin 0x009B9770, and the source comment says so.
- **Test fix from the audit:** two mutants of the initial `Float = 0` store survived the author's test (store removed; store 1.0f).
  - The landed test adds an aliased-text case: retail clears Float at 0x00A0CAB1 before the first text read at 0x00A0CAB4.
  - Both mutants now fail.
  - This case is VC7.1-only, because it relies on no type-based aliasing.

## 0x006CA8C0 `?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z` (138 bytes, protected)
**Identity.** Two PDBs, Ego_d and the landed caller agree:
- Ego_r 0x0056CD10;
- FableWin 0x01E5F600, in tc_appearance_base.obj;
- the Ego_d record (this, long original_texture);
- the landed caller 0x006CA950.

**Body:** `Morphs.push_back(CEngineTextureMorph(original_texture, 0)); return &Morphs.back();`

**Header opt-ins.** Two new opt-ins in engine/CEngineTextureMorph.h, both additive. The default path is identical: `g++ -E -P` matches under 5 define sets, hdrcheck2 reports `default=same`, and the asserts are pinned.
- FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS:
  - the (long,long) constructor, FableWin 0x01E5F690. Its init list is byte-required: body assignment gives DIFFER;
  - the user default constructor, FableWin 0x02D87870. Its body is unread, so it is declaration only and not byte-required for this row.
- FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDENGINEMORPH.

**Existing opt-ins used, in retail's own TU shape:**
- FABLE_RETAIL_CARRAY_PUSHBACK, FABLE_RETAIL_CENGINETEXTUREMORPHSET.
- FABLE_STLP_OUTOFLINE_UNINIT_COPY and _FILL_N. Graded from this source, the overflow 0x006CB390, 0x006CB0D0, 0x006CB170 and _M_clear 0x006CB340 each give RELOCATION_MATCH at their own addresses.

**Test.** All 14 one-decision mutants were killed.

**Future-row risk:** an opt-in user that value-initialises CEngineTextureMorph would need the default ctor's body, which is still unread.

## Header users
All 28 landed users of the header were rechecked with the overlay; see logs/recheck_header_users.txt.
- Under strict headers, 21 give MATCH or RELOCATION_MATCH.
- The 7 HEADER_POLICY rows are pre-existing local models. Without strict headers they give MATCH or RELOCATION_MATCH.
- So the header changes no landed bytes.

## Names (for Account A; not applied by B)
NAME_FIXES.tsv holds:
- 0x006CA8C0: reloc_bindings still has the old `?AddMorph@CTextureMorphs` spelling.
- 0x005BB346: the `V?$allocator` copy ctor; five wrong spellings are listed.
- Four PROPOSED STLport respells, at one source plus an own-address match: 0x006CB390, 0x006CB340, 0x006CB0D0, 0x006CB170.

The landing added the `IAEX` `_M_insert_overflow` binding for 0x006CB390. The old `QAEX` line 22558 remains for A's naming pass.
