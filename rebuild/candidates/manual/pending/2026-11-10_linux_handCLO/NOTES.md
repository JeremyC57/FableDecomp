# agentCLO notes -- 0x005B8743 CTCInventoryClothing::ConstructScreenPC

Result: RELOCATION_MATCH (src/005b8743.cpp, 3,838 bytes) + portable test test/005b8743_test.cpp (`005b8743_TEST PASS`).
Graded with `FABLE_HDR_OVERLAY=$S/w22/agentCLO/hdr`. Nothing parked (no differ/).

## Helpers
No helper row had to be matched first: the callees stay calls, so only their names matter. Every call binds a PDB-spelled symbol
(work/rel_final.txt: reloc_identity HARD = [], five RELOC_NAME warnings, all landed labels fixed in NAME_FIXES.tsv).
The "six unnamed helpers" agentSCR counted are named in LABELS.tsv:
CreateMenuDescPC 0x005B3613, InitialiseMannequin 0x005B37F7, SetUpSelectedClothes 0x005B4795, ConstructTitleString 0x005B6881,
ConstructClothingListPC 0x005B6A66, and the map<EClothingSuitPart,bool> ctor / operator[] / tree dtor 0x005BB3D1 / 0x005BBC4E / 0x005BB090
plus the vector<vector<pair<long,long>>> push_back / dtor 0x005BC237 / 0x005BC080. All are already rows in rebuild/manifest/functions.tsv
(no new manifest rows needed). 0x005B6A66 ConstructClothingListPC itself is still unlanded (parked earlier at 1511 vs 1507 bytes);
0x005B3613 / 0x005B37F7 / 0x005B4795 / 0x005B6881 are manifest rows without landed sources.

## Shape devices (all plain C++, no asm)
1. Same-TU bodies, under `#pragma auto_inline(off)`, of `CDefClassBase::GetSubDef<T>` and `CDefinitionManager::GetDef<T>` (as the
   matched CTCInventoryWeapons screens). Without them the CDefPointer locals are captured (address taken by an opaque call) and the
   in-place `dec [ecx+4] ; jne` releases turn into reloads: DIFFER 3,880 lines -> 177. These need the two StatGetTypeName opt-ins.
2. ConstructClothingListPC called in both arms of `if (parts[i] != CLOTHING_SUIT_NULL && fixedParts[parts[i]])` with literal true/false:
   retail pushes `1` / `ebx` per arm and tail-merges the call (0x005B94D5/0x005B94D9). A single call with a bool expression gives `mov al`.
3. STLport `_List_base(const allocator&)` body visible in the TU (still called out of line at 0x0042AB58): VC7.1 then homes the empty
   allocator temporary on the dead high byte of the `parent` arg slot ([ebp+7Fh], 0x005B9038) instead of opening a new frame slot, which
   had shifted every local +0x50..+0x5C by 4.
4. The per-part quantity loop indexes `partLists[i][j]` directly; a `const ItemVector& v = partLists[i]` local makes VC7.1 LFTR it into a
   count-down loop, retail keeps the `inc ecx ; cmp ; jb` counter and recomputes the size (0x005B917A..0x005B919D).
5. `#pragma optimize("s", on)` (biased frame `lea ebp,[esp-74h]`), as the weapons screens.

## Header debt
- CTCInventoryClothing.h `_SCREEN` is the class's second base and should eventually become the default (all clothing screen rows need it).
- CHeroSuitDef / CAppearanceModifierDef StatGetTypeName opt-ins: every def class used through GetSubDef<T> in the same TU needs one.

## Siblings
- Console twin CTCInventoryClothing::ConstructScreen 0x005B76D1 (vtable slot 77) has the same body shape and the same devices should
  carry over (different list template names / no PC frame). Unlanded.
