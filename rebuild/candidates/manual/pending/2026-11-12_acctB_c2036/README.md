# acctB hand-back, chunk 2036 (1 row matched; the optional second row was not started)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x005841F9 `?GetItemDescription@CTCInventory@@UBEXAAVCWideString@@J_N@Z` | RELOCATION_MATCH 2033v2033, test PASS (g++ and VC7.1) | catalog label `CTCShop::BuildItemDescription` is wrong (see below); 4 grades + 1 comment-only regrade |
| 0x005F6E40 CTCInventoryAbilities::OnSerialise | not attempted | optional row; true extent is 1746 bytes (the 2113 estimate includes separate accessor COMDATs after `ret 8` at 0x005F750F). Outline in `NOTES.md`; goes to a fresh agent |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE). Purity and padded-struct greps are clean; the
only pragmas are the usual TU mode pair (`optimize("y", off)` / `optimize("s", on)`) and `auto_inline(off/on)` around the
same-TU `IS_THING_ALIVE<CThing*>` body, as the landed OnItemSelected 0x00583E26 of this TU has it.

## Review items for landing

1. **Name.** 0x005841F9 is `CTCInventory::GetItemDescription` (Ego_r.pdb + FableWin.pdb name sets; its only reference is
   CTCInventory vtable 0x01250DE4 slot 78, the slot of `CTCInventoryBase::GetItemDescription` 0x005BC6B3 and of the landed
   Clothing 0x005B5CF8 / Weapons 0x005C7913 overrides; the landed ConstructItemDescription 0x005819DD calls it through
   +0x138). `functions.tsv` / the catalog carry `CTCShop::BuildItemDescription`: `NAME_FIXES.tsv`, `LABELS.tsv`.
2. **Header opt-ins** (declarations only, default text unchanged; `hdr/README.md`):
   `FABLE_RETAIL_CTCINVENTORY_ITEMDESCRIPTION` (CTCInventory.h, the slot-78 override; needs
   `FABLE_RETAIL_CTCINVENTORYBASE_VTABLE`) and `FABLE_RETAIL_CTATTOODEF_TYPEBALANCED` (CTattooDef.h, the three
   `GetTypeBalanced*` methods at 0x0057FC46 / 0x0057FC71 / 0x0057FC9C).
3. **Direct member read instead of the PDB accessor.** The row reads `GetDefinitionManager().PHeroStatsDef` (a
   descriptive member name in `engine/CGameDefinitionManager.h`) where FableWin has the inline accessor
   `PeekHeroStatsDef()`. Measured by the agent: the accessor's inline return temporary adds one candidate to the outer
   block and flips a 28/28 colouring tie (`text` loses ebx). The bytes of the read itself are identical either way. Your
   call whether that is acceptable or whether the accessor body in the header is what is off.
4. **Other name fixes found** (`NAME_FIXES.tsv`): landed 0x0057FC46 grades under a stand-in class `CTattooDef_Methods`
   (real: `CTattooDef::GetTypeBalancedAttractiveness`, same for 0x0057FC71 / 0x0057FC9C); 0x01231728 is bound as
   `_s_newLine` (a static-array stand-in from 0x00497A60) but is the pooled wide literal for a newline, used as a literal
   by this row and by the landed Clothing rows.

## How to land

`rows/005841f9.cpp` + `rows/005841f9.test.cpp` with the two `hdr/engine/` overlays merged (both add only `#ifdef` blocks),
then `payload.json` through your usual vland. `LESSONS.md` has six c2 measurements worth a GOTCHAS line each (CDefPointer
filled through a hidden return pointer is address-taken; reference accessor v direct read as a colouring tie-breaker; ties
go to the larger key; one re-assigned local is one frame-packer node, three named locals are three).
