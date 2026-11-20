# 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn (p25A)

**Grade:** RELOCATION_MATCH (1569/1569). The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`; `vland.log`), and so does the real-object behaviour test. Mutants: 24 of 24 killed. Revised after review: resize(n, 0) with no device, and line 2 now names every opt-in.

`payload.json` and `oracle.tsv` carry module `CPersistTraits<NInventory::CItem>` and name `TransferIn`, set by hand: mkpayload's line-1 regex does not parse a template class. That is the catalog style of the landed siblings (`CPersistTraits<COpinionDeed*>::TransferIn` became `CPersistTraitsCOpinionDeed_TransferIn_006c9490.cpp`), so sanitize() gives `CPersistTraitsNInventoryCItem_TransferIn_005c2118.cpp`. The first vland run with an empty catalog name gave a STANDIN_LEAF warning; the final log is clean.

## Source

`src/005c2118.cpp` replaces the hand773 draft. The draft modelled every class locally and was DIFFER(1571v1569) on frame-slot order. The new source uses shared headers only: CStringParser, CPersistContext, CPersistTraits, CGameDefinitionManager, NInventory_CTCInventoryBase (CItem / CPerItemData with the existing CONFISCATED / PERITEMDATA / PERITEMDATA_CTOR opt-ins), CCountedPointer, fable_string and the STLport containers.

**Identity:** `?TransferIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCItem@NInventory@@@Z` (Ego_r.pdb / FableWin.pdb). It is written as the explicit specialisation of the generic class's member; FableWin.pdb also names the generic ??0 / ??1 for this T.

**Structure:** follows Ego_d.pdb (`egod_proc` / `egod_locals_all`, the 7714-byte copy): the locals, their scopes, and the C13 line table `egod_lines/005c2118.tsv`. The source header has the line-by-line reading. In Ego_d's order, with its names, the frame comes out as retail packs it:
- the CCharString traits default at [ebp-8];
- item_type_def_name -0xC, augmentation_def_name -0x10;
- symbol / item_type -0x14, number_of_augmentations / number_of_items -0x18;
- c1 / pper_item_data -0x1C, the bool traits -0x20;
- per_item_data -0x5C.

The draft's 2-byte residue (slot order, `i` kept in memory) does not occur.

Points that decide bytes:
- **Three traits objects, as Ego_d has them (lines 223–225).** The bool and CCharString readers are out of line with `this`; the long traits reader is the generic inline body, which reads into a long and copies. That inline body is why retail copies Kills / Wielded / Trophy / RefCount out of [ebp-24h]: per_item_data's address is taken.
- **Separator checks.** Each 480..519-byte Ego_d line is one `if (!(parser.ReadAsSymbol() == "x")) return;`. The debug diagnostic emits no retail code.
- **The item.** `val = NInventory::CItem(item_type, number_of_items, pper_item_data, average_bought_price);`. The temporary's destructor is the implicit one: retail calls ~CCountedPointer<CPerItemData> 0x005C04BF on temp+8, not a ??1CItem.

## Inline budget (line 2) and ablation

There is no flag: /d2inlT50, /d2inlT100 and /d2inlT200 change nothing in this /Os TU. The `#pragma optimize("y", off)` / `("s", on)` frame matches retail's ebp frame and `leave`; without `"s"` the build is DIFFER(1676v1569). Line 2 names one forceinline contract and two body / declaration opt-ins:

| device | without it |
|---|---|
| FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (existing forceinline contract) | DIFFER(1115v1569): operator==(const char*) is called out of line at each separator. |
| FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO (existing; the visible CBasicString<char>::operator==(const char*) body) | DIFFER(1447v1569) |
| FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN (new). Besides the constructor declaration, it leaves ~CItem implicit, so the temporary's destruction expands to the ~CCountedPointer<CPerItemData> call (0x005C270E). | With the out-of-line `~CItem();` still declared: DIFFER(1569v1569), a ??1CItem call. |

FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY was tried and is not needed: the build matches without it, because VC7.1 calls the TU's own ~CCountedPointer COMDAT. It is not defined.

**resize spelling (revised after review).** The source is `per_item_data.Augmentations.resize(number_of_augmentations, 0)`. No device is needed: VC7.1 calls resize(n, const long&) 0x004428F0 by itself.

Ego_d line 240 (13 B) supports this spelling once the VC10 /Od encoding is calibrated on line 238. Line 238 is `slong_traits.TransferIn(persist_context, parser, number_of_augmentations)`, 18 B = lea / push, two direct `push [ebp+x]`, lea ecx, call; so this build pushes memory operands directly. On that encoding:
- the two-argument by-value call (Ego_r / FableWin Dinkumware `resize(unsigned, long)`, `?resize@..QAEXIJ@Z`) is push 0 (2) + push [n] (3) + lea ecx (3) + call (5) = **13 B**;
- the one-argument form would be 11 B.

The earlier draft of this batch used resize(n) with a new FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_FORCEINLINE device; its "about 29 B" estimate assumed a by-reference temporary. That device and its fable_containers.h overlay are withdrawn. For the record, resize(n) without a device gives DIFFER(1562v1569) (`?resize@..QAEXI@Z` called out of line); /d2inlT does not change that.

## Header overlay (`hdr/`, `hdr.diff`)

The overlay is add-only: 15 `+` lines, 0 `-` lines; pin_asserts pinned 0.

**engine/NInventory_CTCInventoryBase.h**, opt-in `FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN`:
- adds FableWin's `CItem(long, long, const CPerItemData*, long)` (0x005BEC40);
- puts the existing out-of-line `~CItem();` declaration in the `#else` branch, so this TU uses the implicit destructor (retail 0x005C270E).

**engine/CPersistTraits.h**, opt-in `FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_TRANSFERIN_DECL`: the declaration of the member specialisation `CPersistTraits<CCharString>::TransferIn` (0x0040E510). The generic body, a long cast to T, does not compile for CCharString.

`newly_flagged_rows.tsv` lists two landed local-model rows: 0x005BEC40 and 0x0040E510.

## Test (`test/005c2118.cpp`, links the real row object)

The test links the row with the TU's vector<long>::resize(n, const long&) COMDAT (`_M_fill_insert` is a stub) and its ~CCountedPointer<CPerItemData> COMDAT. The CItem stub hands the temporary a count block at RefCount 2, so the release must leave it at 1. A token-stream parser model drives the text format, and every callee is a native-convention stub:
- ReadAsSymbol with a hidden result; a `~` token models an empty symbol, so the null-data path is taken.
- Compare, CCharString ctor / dtor / !=.
- The bool / CCharString / SLONG readers. The bool reader leaves the value unchanged on an unknown token, as the real one does.
- Peek / ReadAsInteger, Get / GetDefGlobalIndexFromName.
- The CItem constructor, which snapshots the per-item data it is handed; operator=.

**Runs** (18):
1. A full item: augmentations are an integer, a name, "NULL" and "", followed by the quest pair.
2. No per-item data.
3. Per-item data closed by `)` with no augmentations, so the defaults are kept (quest true, ref -1).
4. An unreadable has-data flag, so the no-data layout follows.
5. Fourteen failures, one at every separator, including the empty-symbol path.

**Checks per run:**
- the CItem arguments and the snapshot (augmentation slots, kills / wielded / trophy / quest / ref);
- operator= into `val`, and one release of the temporary's counted pointer;
- `val` untouched on failure;
- tokens consumed;
- def-index calls;
- every CCharString built is destroyed.

**Mutants:** `mutants/005c2118.tsv` holds 24 row-source mutations; all 24 are killed.

## NAME_FIXES

- **0x005BEC40:** `??0CItem@NInventory@@QAE@JJPBVCPerItemData@1@J@Z` (FableWin). It is landed as the constructor of a global `::CItem` on local models.
