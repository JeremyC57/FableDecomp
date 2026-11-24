# NF302 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF301, hand-back 1

Base: the repo working tree as of today. I never wrote to it and ran no git. "Entry n" = the n-th data line of my ENTRIES.tsv
(file line n+1). Lines 1-5 are NF301's entries 14 / 15 / 44 / 45 / 46; line 18 is NF301's entry 75.

## Summary (counts from STATUS.tsv)
- **74 entries with a verdict:** entries 1-65 in order, plus 72 and 119-126, pulled ahead (E2: in-place or stale-witness shape).
- **Status:** done 12, done-partial 5, done-already 2, skipped-port 35, skipped-held 20. No rejects and no vtable change.
- **14 rows changed** (rows.txt). Tests are unchanged, so rows/ holds only the sources. vland `--recheck-landed` on the new sources
  gives 14/14 RELOCATION_MATCH, test PASS (recheck_final.log). Its WARN lines are local models the rows already had at HEAD; no
  newly modelled class is flagged.
- **Header changes:** six shared headers get add-only, declaration-only opt-ins. Full files are in hdr/engine/; HEAD copies are in
  orig/engine/ (cmp-equal to the repo at hand-back).

## Header batch 1: entries 1, 2, 3, 5 and 18 (NF301 entries 14, 15, 44, 46, 75)
| header | new macro | declaration | binder |
|---|---|---|---|
| engine/CPixelFormat.h (flat branch) | FABLE_RETAIL_CPIXELFORMAT_FLAT_GETINIT | `class CPixelFormatInit GetInit() const;` 0x009E3870 | 0x00993F60 |
| engine/CPlayer.h (class tail, both branches) | FABLE_RETAIL_CPLAYER_PREPAREFORLOAD | `public: void PrepareForLoad();` 0x00487550 | 0x00449C30 |
| engine/CResource.h (flat branch) | FABLE_RETAIL_CRESOURCE_FLAT_SETRESOURCESIZE | `void SetResourceSize(unsigned long);` 0x009FBF20 | 0x009C8470 |
| engine/CResourceList.h (flat branch) | FABLE_RETAIL_CRESOURCELIST_FLAT_UNLOADNEXTRESOURCE | `bool UnloadNextResource(unsigned long);` 0x009FC170 | 0x009FC210 |
| engine/CTCBuyableHouse.h (flat branch) | FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_GETPRICE | `long GetPrice() const;` 0x006C0120 | 0x0072E940 |

- **Header rules.** Each opt-in sits just before its class's closing `};`.
  - The new macro names are fresh: none of them is in rebuild/include, rebuild/src, rebuild/tests or the NF299 / NF300 hdr/.
  - I did not reuse FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE in the flat branch, because that would change an existing opt-in.
  - No shared header already declared any of the five callee addresses.
- **Assert pinning.** The unpinned asserts in CResource.h (lines 68 and 88-94) and CResourceList.h (41 and 81-90) are pinned to their
  HEAD line numbers as `FABLE_STATIC_ASSERT_AT(<HEAD line>, ...)`. The other three headers were already pinned.
- **Binder edits.** Each binder drops the member from its derived stand-in struct; four structs stay, because they type the self
  leaf or a local container. 0x0072E940 drops the stand-in struct and its two casts outright (`house` is a CTCBuyableHouse*). The
  `#define` goes before the include, and the line count is unchanged.
- **Proof:**
  - H and N objects are RELOCATION_MATCH. objsec is ALL_EQ with exactly one UND rename per row, and the DEF sets are identical
    (evidence/proof_hb1.txt).
  - Link (evidence/link_hb1.txt): HEAD leaves the five stand-ins unresolved; the new set resolves all five; 0 duplicates.
  - equiv_check (exact) over 8,772 includers (transitive, plus every `../` includer) finds 0 differing. The planted control
    (`typedef int nf302_planted_<H>;` at line 1 of each overlay header) flags 6,481 lines in 6,218 rows
    (evidence/eq_hb1_summary.txt).
- **Not covered:**
  - Entry 4 (GetUnusedFrames) is not this shape: Decision 1.
  - Entries 3 and 5 are done-partial; their other binders are ports or off-limits.

## Header batch 2: entries 56, 57, 59 (engine/CTCInventoryItem.h, flat branch)
- **New opt-ins.** Three add-only opt-ins declare the static `__fastcall` def-index members, as the retail branch already does:
  - FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETENGINEGRAPHIC (0x005D96A0);
  - FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETSTARTINGRIGHTHANDEDSET (0x005D9730);
  - FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETCONTEXTSENSITIVETYPE (0x005D9B90).
- **Binder edits.** Binders 0x00581CFD, 0x00581FA4, 0x005BCD2D and 0x00581D8E replace their free stand-in declaration with the
  define and the include, and qualify the call.
  - Each gains one line. None of these rows has an assert or uses `__LINE__`.
  - 0x00581FA4 bound the same free stand-in with no table line. I found it with xsites.
- **Proof:**
  - objsec ALL_EQ (evidence/proof_b2.txt).
  - link_b2 resolves all of them.
  - equiv_check over the 1,521 includers of CTCInventoryItem.h (the tree holds all six overlay headers) finds 0 differing; the
    planted control flags 148 lines.

## In-place rows (no header)
- **0x006412B0 (entry 34):** the one-member stand-in struct CTCGraphicAppearance moves into NGraphicAppearance as a class.
- **0x00ED99D0 (entries 122-126 and the rider 0x00EDAAD0):** the stand-in parent struct CV_TalentlessBardScript moves into NScript as
  a class, which respells six calls. Per the prefix ruling these are namespace moves of table-less local types; the objects emit no
  tables.
- **0x005BD4EA (entry 47):** `protected:` + `const` on GetHighlightedItem in its own NInventory::CTCInventoryBase model.
- **0x005BDA1D (entry 50):** `const` on GetInventoryText in its local model.
- **0x005BD273 (entry 50, self):** the owner defined the non-const NInventory form, which only 0x005BDA1D bound. Seven landed callers
  bind the PDB const form, and they were unresolved at HEAD.
  - Change: `const` on the declaration and the definition (the body uses no member), plus a full mangled `// Leaf:` line.
  - RENAMES section 1 has the self rename.
- **Proof:** objsec ALL_EQ, one rename each (proof_b2 / proof_b3 / proof_b3s); link_b2 and link_b3b; vland PASS.
- **Land together:** 0x005BDA1D, the 0x005BD273 self fix and the GetInventoryText table delete land together or not at all. Alone,
  0x005BDA1D would go from resolved (against the owner's old non-const self) to unresolved. 0x005BD4EA (entry 47) is separable: it
  is unresolved before and after.

## Table (TABLE_CHANGES.tsv, 21 lines)
- 10 `replace` (no PDB line existed yet): GetInit, UnloadNextResource, GetContextSensitiveType, RemovePrimitive and the six
  ProcessSongs* calls.
- 10 `delete`:
  - 7 are respelled binders whose PDB line already exists;
  - 3 are stale witnesses (entries 53, 119, 120). Each had its HEAD object read, and every caller was read through xsites and the
    source.
- Dry run: apply_table.py --dry on a scratch copy gives 21 / 21 hits, 0 misses, no new multi-target symbol, and no replace
  creates an existing (symbol, target) pair (evidence/apply_table_dry.txt). The rewitness line carries the NEW witness in its
  witness column, as apply_table.py reads it; the old witness is in the note.
- 1 `rewitness`, the prefix item: `?GetPlayer@CPlayerManager@@QAEPAVCPlayer@@J@Z`'s witness 0x0066FB80 -> 0x00449BF0.
  - 0x00449BF0's HEAD object binds that spelling, and so does 0x00449C10's (evidence/objdump_readouts.txt).
  - The other local-model rows that declare `CPlayer* GetPlayer(long)` are listed in work/getplayer_rows.txt.

## Not done / left for others
- **Entry 72 (0x00704150), attempted and withdrawn.**
  - What I tried: moving its local CTCShotDefinitionBase into NThingShot. That is byte-equal (objsec ALL_EQ), and it also renames a
    pick-any GetTC<> COMDAT.
  - Why I withdrew it: the owner 0x007C21A0 defines the global stand-in, so the new set would leave unresolved a reference that HEAD
    resolves (evidence/link_b4.txt).
  - What it needs: the owner's own port (entry 71).
- **Entry 35 (0x007CC120 RemoveTintColour) and entry 40:** namespace-only in shape, but the owner is an NF299 witness. Not compiled.
- **Entry 121:** the witness 0x00ED4830 fails its own clang precheck (a deleted destructor cannot override; not a toolchain header).
  Not compiled.
- **Off-limits set (work/offwhy.tsv):**
  - the NF299 / NF300 / NF303 / NF304 entries and witnesses (NF303 and NF304 are the continuation agents);
  - live locks of other agents (NF303 / NF304 hold some);
  - the unexpired codex-p28 reservations, the held rows and T3_PORT_QUEUE.
  - The off-limits STATUS lines follow NF301's rule: a target that is another agent's witness means the owner cannot be compiled for
    the link proof, so the entry is held.

## Decisions / questions
1. **Entry 4 (CResource::GetUnusedFrames 0x009FBF70)** has no one-declaration fix.
   - 0x00A69ED0's CManagedTextureData_Calls derives the FLAT engine/CManagedTextureData.h, which has no CResource base. The retail
     branch has a private `Allocated` and a virtual dtor.
   - 0x009FE8E0 is a full local polymorphic ResourceNative model.
   - Should the flat CManagedTextureData take an opt-in CResource base? It is a layout-neutral base change (the flat `_pad_0x04..0x18`
     are the CResource bytes), so it would need an owner decision. Or are both rows queued as T3 ports?
2. **Entry 47:** 0x005BD4EA now binds the PDB spelling. The owner 0x005BCAE3 still defines the global stand-in
   `?GetHighlightedItem@CTCInventoryBase@@QAEJXZ`, so the symbol is unresolved before and after (no regression; 0x00583E26 already
   binds the PDB form). The owner is a placeholder row (InvItem::GetHandle), so its fix is a T3 port. Is a binder move with an
   unresolved-before-and-after link acceptable, or should it wait for the owner?
3. **Owners that define a stand-in** are a recurring blocker: 0x007C21A0 (entry 72), 0x005BCAE3 (entry 47) and 0x005BD273 (fixed
   here, self const). The tracker's `self_leaf` column is empty for some of them, so the split looks like a binder split when the fix
   is on the owner. Could the tracker read the owner's HEAD object symbol when the row has no Leaf line?
4. **0x00ED99D0 and 0x006412B0** keep local models of classes that have namespaced shared headers (NScript_CV_TalentlessBardScript.h,
   NGraphicAppearance_CTCGraphicAppearance.h). The verifier's shared-header test checks only `engine/<Class>.h`, so they are not
   flagged. The move is in place, but the port onto those headers is still debt.

5. **Self rename of 0x005BD273 against NF282 D2** ("binder / owner self names are T3 ports"). I took it as in place: the change is
   `const` only (T1), the body uses no member, objsec is ALL_EQ, and the 7 landed callers bind the PDB const form. The callers were
   checked by source read, not compiled. If D2 covers this, drop 0x005BD273, 0x005BDA1D and the GetInventoryText delete together.

## Rule notes
- **Locks.** Every compiled row was locked before its compile, and every landed test was prechecked before its lock (pre_tests1..6).
  Each source was prechecked after its lock and before its grade (pre_srcH1..6, pre_rows1..5; logs in evidence/).
  - Locks still held: the 14 changed rows.
  - Released: everything else, including the owners and the stale-check witnesses. 0x00ED4830 was released without a compile.
- **Background runs.** Every grade, link, equiv_check and vland run went to the background with `timeout 900` (equiv 3000) on its own
  launch line, and was checked in a later call.
- **Not done:** no sleep, no rm (clean.py only), no git, nothing written under the repo, no `__asm` / `naked` / `_emit`.
- **Retail bytes:** none in hand-back files (hex-run grep). The vland oracle stays in work/pay1.
- **Neighbour headers:** NF303's and NF304's hdr/ folders are empty, and none of my eight macro names appears in their folders. None
  of the rows I compiled is in their ENTRIES.
- **Census:**
  - Binders: xsites for every respelled target (evidence/xsites_*.tsv).
  - New-name census (evidence/namecensus.txt): each new `Class::Member` is defined only by its owner row (plus standalone test
    stubs).
  - No landed source, test or header includes a changed row by path.
- **Reading list (disclosure).**
  - Read in full: my prefix, the standing brief, all of GOTCHAS.md, CLAUDE.md, and NF301's NOTES, LESSONS and every .tsv
    (STATUS read in two passes).
  - Read only in part: docs/COORDINATION.md (first 60 lines, through the start of section 4); NF301's evidence/ (listed, not read).
  - Read in part: EXTRA_RULES.md (the naming, stand-in, quarantine, pick-any, STLport, anonymous-namespace and relocation sections).
  - Not read: ASM_RULES.md (no asm in any change) and the nf281..nf285 landing READMEs.
- **tools/:**
  - NF301's set, plus offl.py (off-limits set), tri5.py (tri4 plus off-limits reasons), hb302.py / hb302b.py / hb302c.py (the exact
    edits, line count checked, assert pinning by HEAD line index), mkpay302.py (recheck payload with the NEW sources) and
    namecensus.py;
  - verdicts302.py + mkstatus302.py (the STATUS generator).
  - flags.sh / srcof.sh / tri4.py are bounded to the row's own catalog block.
