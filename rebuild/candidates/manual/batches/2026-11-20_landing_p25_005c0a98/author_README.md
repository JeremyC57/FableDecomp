# 0x005C0A98 CPersistTraits<NInventory::CItem>::TransferBinaryOut

**Grade:** RELOCATION_MATCH (369/369). No flag, no pragma beyond the TU's size pragmas.
- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`, fresh LANDVERIFY_WORK. No warnings.
- **Behaviour test:** passes on the real object. It runs 3 cases: no per-item data; 4 augmentations (two "NULL" for indices 0 and -1);
  an empty augmentation vector. It checks the exact write sequence, the def-name lookups, the `this` passed to every call, and that every
  CCharString is destroyed.
- **Mutants:** 22/22 KILLED (`mutants/005c0a98.out`): 20 in the source, 2 in the header getters.
- **Name:** Ego_r.pdb / FableWin.pdb `?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z`.
  The Ego_d names are used throughout: os, val, pper_item_data, has_per_item_data, def_name, c1, augmentation_def_name.
- **NAME_FIXES.tsv:** header line only.

## What changed against the draft (pending/2026-11-10_linux_hand1020/differ/005c0a98.cpp, DIFFER(368v369))
- **Push order.** The draft's residue was where the conversion temporary's push came. Retail pushes the hidden result of the conversion
  `??B` 0x00415DD0 before the object expression (`lea ecx,[ebp-8] ; push ecx` at 0x005C0AF6, ahead of Get / GetDefNameFromGlobalIndex).
  The existing per-TU contract FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (conversion declared only) gives that order.
  - With the plain inline forwarder, VC7.1 still calls 0x00415DD0, but it pushes the result after the object expression.
  - The landed same-TU twin TransferOut 0x005C0E81 declares the conversion out of line too.
- **Getters.** With that contract alone, the register web moved: stream went into ebx and val into edi, DIFFER(364v369). The Ego_d
  spelling fixes it: Ego_d's line sizes are call-shaped (lines 191 / 217 / 218, 11 / 17 / 17 B). The source reads the item through
  `val.GetPerItemData()`, `GetItemType()`, `GetNumberOfItems()` and `GetAverageBoughtPrice()`. Ego_d and FableWin both name all four.
- **Shared-header rewrite.** The draft's local `template<> class CPersistTraits<NInventory::CItem>` is gone. The row is an explicit
  member specialisation of the primary template, as the landed TransferIn 0x005C2118 is. The primary writer takes T by value in the
  header, which a class T cannot use, so a new opt-in declares it by const reference (the PDB spelling).

## Ablation (each removed singly from the final source; `evidence/ablations/results.txt`, preflighted by ablq.py)
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| no FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (plain inline forwarder) | DIFFER(368v369) |
| members read directly instead of the four getters (no FABLE_RETAIL_NINVENTORY_CITEM_GETTERS) | DIFFER(364v369) |
| `CCharString def_name; def_name = ...` instead of copy-init | DIFFER(377v369) |
| `unsigned int c1` instead of Ego_d's `long c1` | RELOCATION_MATCH (kept long: Ego_d type 0x12) |
| no FABLE_RETAIL_CPERSISTCONTEXT | RELOCATION_MATCH, same undefined symbols (dropped from the final source) |
| no FABLE_RETAIL_CPERSISTTRAITS_BINARYOUT_CONSTREF, or any other remaining define | SRC_FAIL |

## Header changes (`hdr/`, `hdr.diff`; add-only, no assert moved: pin_asserts.py pinned 0)
- **engine/NInventory_CTCInventoryBase.h** gets FABLE_RETAIL_NINVENTORY_CITEM_GETTERS. These are the four CItem getters with
  one-member bodies; the bodies are inferred, and retail expands them all (`mov esi,[ebx+8]`, `mov eax,[ebx]`, `push [ebx+4]`,
  `push [ebx+10h]`).
- **engine/CPersistTraits.h** gets FABLE_RETAIL_CPERSISTTRAITS_BINARYOUT_CONSTREF. It changes the primary
  `TransferBinaryOut(CDataOutputStream&, const T&)` to declaration only.
  - Why declaration only: VC7.1 treats an explicit specialisation of an in-class-defined member as inline, and emits nothing for it.
    I measured this: the object was empty with the inline body.
- **newly_flagged_rows.tsv:** header line only. Both headers already exist and no class is new.

## Notes for the coordinator
- **Header-only COMDATs:** the object also emits unreferenced inline COMDATs (the getters, GetLength, vector size / begin /
  operator[], CCountedPointer::Get). They are the usual /Ob1 header COMDATs. None of the four CItem getters is in the labels DB (no retail out-of-line copy is known).
- **Old landed text in the same TU:** TransferOut 0x005C0E81 is still a local-model file. Its CDefString model declares a user
  copy constructor that the shared header does not have. Not touched here.
