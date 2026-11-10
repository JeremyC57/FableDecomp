# agentCLO header overlay (row 0x005B8743 CTCInventoryClothing::ConstructScreenPC)

Each file is a full copy of the CURRENT working-tree header (including agentNF's uncommitted opt-ins) with only `#ifdef`
additions; `diff rebuild/include/<f> hdr/<f>` shows added lines only, so the default path is byte-identical.
Grade with `FABLE_HDR_OVERLAY=$S/w22/agentCLO/hdr`.

## engine/CTCInventoryClothing.h -- `FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN` (requires `_CALCULATE`)
- Adds `NHeroInformationScreens::CBase` as the second base (so the old 4-byte `_pad_0x130` is its vptr; the pad is kept only
  in the `#ifndef` path). Evidence: RTTI `.?AVCTCInventoryClothing@@` bases = NInventory::CTCInventoryBase @0 + NHeroInformationScreens::CBase
  @0x130 (ghidra_out/rtti_bases.tsv), vtable 0x0124356C; the matched CTCInventoryWeapons.h models its screen the same way.
- Declares the screen methods in PDB spelling: `ConstructScreenPC` 0x005B8743 (QAE, Ego_r 0x005641A2), `ConstructTitleString` 0x005B6881
  (QAE, Ego_r 0x00560917), private `CreateMenuDescPC` 0x005B3613 (Ego_r 0x0055EE9C), `InitialiseMannequin` 0x005B37F7 (Ego_r 0x0055F073),
  `SetUpSelectedClothes` 0x005B4795 (Ego_r 0x00560076), `ConstructClothingListPC` 0x005B6A66 (Ego_r 0x00562A81). Access (AAE vs QAE)
  from the Ego_r.pdb mangled names; the RELOCATION_MATCH binds every call to those spellings.
- Types the members `std::map<NUISystem::CComponent*, long> ClothingItemMap` (+0x160; retail clears it through the shared
  map<CComponent*,long> tree clear 0x004DB806, same as CTCInventoryWeapons::WeaponMenuItemMap) and `NUISystem::CComponent* ClothingList`
  (+0x16C; `mov [esi+16Ch],edi` at 0x005B9579), names and types from ghidra_out/struct_layouts_egor.tsv.

## engine/CHeroSuitDef.h -- `FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME`
- Adds `static CCharString __fastcall StatGetTypeName() { return CCharString("CHeroSuitDef", -1); }`.
  Evidence: FableWin.pdb `?StatGetTypeName@CHeroSuitDef@@SI?AVCCharString@@XZ` (inline in release, no Ego_r body) and the
  "CHeroSuitDef" literal the landed GetSubDef<CHeroSuitDef> 0x005935C4 passes to GetPSubDefInfo. Needed so the row's same-TU
  `CDefClassBase::GetSubDef<T>` body compiles (that body is what keeps the CDefPointer locals uncaptured).

## engine/CAppearanceModifierDef.h -- `FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME`
- Same for "CAppearanceModifierDef": FableWin.pdb `?StatGetTypeName@CAppearanceModifierDef@@SI?AVCCharString@@XZ`; literal in the
  landed GetSubDef<CAppearanceModifierDef> 0x0057FF92.
