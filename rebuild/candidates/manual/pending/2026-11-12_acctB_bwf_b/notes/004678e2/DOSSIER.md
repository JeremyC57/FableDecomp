# DOSSIER 0x004678E2 -- CInventoryDef::Transfer(CPersistContext&)   (5422 bytes, no draft)

## 1. Retail shape  (python3 $S/w17/xdis.py 004678e2 0x153e; $W/dis_all.txt)
- `__thiscall`, `ret 4` (0x00468E0D). NO frame pointer, NO SEH, NO /GS cookie, no locals, no branches at all.
- Prologue `push ebx ; push esi ; mov esi,ecx ; push edi ; mov edi,[esp+10h]` -- this in esi, context in edi.
  Epilogue `pop edi ; pop esi ; pop ebx ; ret 4`.
- Body: 389 calls, each `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call T` (size-optimised: address via eax,
  no caching). Two exceptions, both natural compiler output:
  * 0x004688DE `lea ebx,[esi+6D8h]` then `push ebx/call 0x004568BC` TWICE -- the source transfers
    TradeShadedCircleTLPos two times in a row (copy-paste bug in Lionhead's list: TradeShadedCircleExtents +0x6E0
    is never transferred). The CSE into ebx is the ONLY reason ebx is saved in the prologue -> write the
    duplicate statement literally.
  * last member: `add esi,0A30h ; push esi` (0x00468DFC) = MainQuickAccessItemFlashTime, as in every sibling.
- Pragmas: `#pragma optimize("s", on)` only (siblings CCameraManagerDef 0x0046A0B0, CQuickAccessItemButtonGuiDef
  0x0046EE6A, CHeroCombatDef 0x004665B6 use exactly this). Do NOT add optimize("y",off) (no ebp frame). No /EHsc need.
- No base-class Transfer call (CDefClassBase::Transfer is pure, slot 18; same in CControlsDef / CFrontEndDef rows).

## 2. Identity  -- manifest label is RIGHT
- Name ?Transfer@CInventoryDef@@UAEXAAVCPersistContext@@@Z  (virtual void CInventoryDef::Transfer(CPersistContext& context)).
- Evidence 1: rebuild/manifest/rtti-vtables.tsv `CInventoryDef 01233404 18 004678e2` -- slot 18 = Transfer in
  rebuild/include/engine/CDefClassBase.h:98 (`virtual void Transfer(CPersistContext&) = 0; // slot 18`); slot 20 of the
  same vtable is 0x004554A4 `mov eax,0A34h ; ret` = GetSizeofClass (retail sizeof CInventoryDef = 0xA34).
- Evidence 2: Ego_r.pdb ghidra_out/egor_pdb_names.tsv 0x0046CAF0 and FableWin.pdb (_fablewin_api.tsv 0x0192B0CA, 8558 b;
  naming_stragglers/FableWin.exe.signatures.tsv) both carry the exact mangled leaf; ghidra_out/def_transfer_targets.tsv
  lists 004678e2 CInventoryDef.
- Callers: only the vtable data ref 0x0123344C (xref.py). Signature: void, one param `CPersistContext&`, non-const, virtual.

## 3. Callees  (all out-of-line COMDAT instances of `template<class T> void CPersistContext::Transfer(T&)`,
##    declared unconditionally at rebuild/include/engine/CPersistContext.h:91)
| addr | count | T (member type proven by offsets) | landed row | landed name / issue |
|---|---|---|---|---|
| 0x004568BC | 130 | C2DVector | 00/45/CEngineLightingManager_UpdateShadowScene_004568bc.cpp | WRONG label "UpdateShadowScene" (body: CheckCRC 0x404500; mode 3 -> 0x993F90 write, mode 2 -> 0x995030 read 8 bytes). Sibling rows already spell it `Transfer(C2DVector&)`. Ego_r has only the named ??$Transfer@VC2DVector@@@CPersistContext@@QAEXPBDAAVC2DVector@@@Z |
| 0x00431020 | 110 | unsigned long | 00/43/CPersistContext_Transferunsignedlong_00431020.cpp | ??$Transfer@K@ ok |
| 0x00431061 | 59 | float | 00/43/CPersistContext_Transfer_00431061.cpp | ??$Transfer@M@ ok (coverage label UpdateShadowScene is stale) |
| 0x00451E11 | 36 | CEngineGraphic | 00/45/CPersistContext_TransferCEngineGraphic_00451e11.cpp | ok |
| 0x00431102 | 32 | long | 00/43/CPersistContext_Transferlong_00431102.cpp | ??$Transfer@J@ ok |
| 0x00466A47 | 8 | std::vector<long> | 00/46/CPersistContext_TransferBinary00466a47_00466a47.cpp | landed as `TransferBinary_00466a47(SValue*)` -- real: Transfer<std::vector<long> > (GFSerialiseVectorBinaryIn<long> 0x00466AA7 per its landed row) |
| 0x004595A3 | 7 | CRGBColour | 00/45/CPersistContext_TransferCRGBColour_004595a3.cpp | ok |
| 0x00456903 | 2 | C3DVector | 00/45/CEngineLightingManager_UpdateShadowScene_00456903.cpp | WRONG label (Transfer<C3DVector>: 0x993FD0 / 0x9950F0) |
| 0x0043314A | 1 | bool | 00/43/CPersistContext_Transferbool_0043314a.cpp | ok |
| 0x0045228F | 1 | CDefString | 00/45/CPersistContext_TransferCDefString_0045228f.cpp | ok |
| 0x00464841 | 1 | std::vector<unsigned long> | 00/46/CPersistContext_TransferBinary00464841_00464841.cpp | landed `TransferBinary_00464841`; real Transfer<vector<unsigned long>> (NUISystemCUIStateDef row 0x00625630 agrees) |
| 0x00468EB4 | 1 | std::vector<CRGBColour> | 00/46/CPersistContext_TransferBinary00468eb4_00468eb4.cpp | landed `TransferBinary_00468eb4`; real Transfer<vector<CRGBColour>> |
| 0x00468E10 | 1 | NInventory::EInventoryIdentifier | 00/46/CPersistContext_TransferNInventoryEInventoryIdentifier_00468e10.cpp | ok (its header cites this call at 0x00467C24) |
Every one is a call; the author only needs the member types complete and the template declared -- no body visible/needed.
NAME_FIXES candidates (coordinator): 0x004568BC, 0x00456903 (UpdateShadowScene -> CPersistContext::Transfer<C2DVector>/<C3DVector>),
0x00466A47/0x00464841/0x00468EB4 (TransferBinary_xxx -> Transfer<std::vector<...> >). One-arg spelling is the lane's
descriptive form (no PDB has it; PDBs hold the named 2/3-arg forms).

## 4. Members  (all 391 Ego_r members; transferred 388 distinct + 1 duplicate = 389 calls)
- Source of names/types: ghidra_out/struct_layouts_egor.tsv `CInventoryDef` (sizeof 2656). Retail offset = Ego_r - 4
  (CParentDefClassBase 0x39 vs 0x3D) - 4 per preceding std::vector (12 vs 16 bytes, 10 vectors) -> end 0xA34 = retail
  GetSizeofClass. Every one of the 389 call sites hits a member of exactly the type its helper transfers (checked by script,
  $W/seq_named.txt).
- Natural (unpacked) layout reproduces ALL retail offsets PROVIDED C2DVector and CEngineGraphic are alignment-1
  (C2DVectors sit at 0x9FE, 0xA06... right after 14-byte CEngineGraphics), floats/longs 4-aligned, CParentDefClassBase
  0x39 bytes then pad to 0x3C. Verified by script: 0 mismatches, end 0xA34 (C3DVector align 1 or 4 both fine).
- Shared header: rebuild/include/engine/CInventoryDef.h -- PARTIAL (members only up to MeshAreaBRPos +0xAC, no sizeof,
  no Transfer declared, no FABLE_RETAIL_* opt-ins). It includes engine/CParentDefClassBase.h, engine/C2DVector.h,
  stlport/fable_containers.h. The author must extend it (overlay in $W/hdr: full copy of current header + an
  `#ifdef FABLE_RETAIL_CINVENTORYDEF_TRANSFER` block -- name to choose) with the remaining members (+0xB4..+0xA30) and
  `virtual void Transfer(CPersistContext&);`. Ready-made member list with retail offsets: $W/members.txt.
  rebuild/include/engine/_quarantine/CInventoryDef.h is a generated pack(1) flattened model (sizeof 0xA34, `_X/_Y` split
  members) -- offsets agree but do not use it as the model (flattened, opaque).
- Needed member-type headers / macros:
  * engine/C2DVector.h: `FABLE_RETAIL_C2DVECTOR` = the byte-packed C2DVector (pack(1); evidence row 0x00631C60). REQUIRED
    for the 2-aligned C2DVectors (0x9FE...). Without it C2DVector is 4-aligned and offsets break after 0x9F0.
  * engine/CEngineGraphic.h: already pack(1), 14 bytes; no macro needed (FABLE_RETAIL_CENGINEGRAPHIC_CTOR not needed).
  * engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR_PACKED optional, offsets 0xE8/0x2E4 are 4-aligned either way).
  * engine/CRGBColour.h (sizeof 4), engine/CDefString.h (class CDefString), NInventory::EInventoryIdentifier: no shared
    header -- declare `namespace NInventory { enum EInventoryIdentifier { }; }` as the landed 0x00468E10 row does.
  * engine/CPersistContext.h: `template<class T> void Transfer(T&);` (line 91, unconditional).
  * vector element types: std::vector<long>, std::vector<unsigned long>, std::vector<CRGBColour> (STLport via
    stlport/fable_containers.h). Mangled callee names are only relocation names; binding is by address.
- Classes that sibling rows model LOCALLY although a shared header exists: C2DVector (CQuickAccessItemButtonGuiDef row),
  std::vector / CPersistContext (CCameraManagerDef, CHeroCombatDef, CControlsDef, CFrontEndDef rows). Use the shared headers.

## 5. Visibility
- Nothing inlines: all 13 helpers are called out of line in retail (they are COMDAT template instances, several
  placed right after this function: 0x00468E10 Transfer<EInventoryIdentifier>, 0x00468E51 its TransferBinaryIn,
  0x00468EB4 Transfer<vector<CRGBColour>>, 0x00468EE2 GFSerialiseVectorBinaryOut -- i.e. first instantiated in this TU).
  Declare them only; do NOT provide bodies (a visible body under /Os might inline the 46-72 byte helpers).
- No STLport / CIntelligentPointer / string expansion anywhere.

## 6. Siblings (same family, same address neighbourhood, all `#pragma optimize("s", on)`, `ret 4`, same per-member shape)
- 00/46/CCameraManagerDef_Transfer_0046a0b0.cpp (vector<long> 0x466A47 + long + float; "last address formed in place (add esi,N)").
- 00/46/CQuickAccessItemButtonGuiDef_Transfer_0046ee6a.cpp (long + C2DVector 0x004568BC: `void Transfer(C2DVector&)`).
- 00/46/CHeroCombatDef_Transfer_004665b6.cpp (605 b, out-of-declaration-order member list; vector<long> 0x466A47).
- 00/46/CPlayerInventoryDef_Transfer_00467553.cpp (the function just before this one in retail).
- 00/45/CCombatDialogueDef_Transfer_00456dfb.cpp (`// Flags: /O1` variant of the same shape).
None use `// Flags:` except CCombatDialogueDef; default lane flags + the "s" pragma suffice.

## 7. Order
Retail order is NOT declaration order (14 breaks, see OUTLINE). Write statements exactly in OUTLINE order.
Never transferred: TradeShadedCircleExtents (+0x6E0), AugmentTitleTLPos (+0x740), AugmentTitleTextOffset (+0x748).
