# agentNF15 notes: NF14 proven spellings (tag land1011bs) respelled across landed binders

Scope: QUEUE.tsv (96 NF14 spellings) plus NF14's "Left" items. Every changed row is in `rows/`, graded with
`work/chk.py --all` under FABLE_HDR_OVERLAY=`hdr/`, and carries its catalog PassPattern in both the g++ -std=c++11 64-bit
test and the VC7.1 test. Nothing was landed. BUSY.txt addresses were not touched.

## Method
- `work/find_binders.py` lists every landed row whose reloc binds a QUEUE address under a wrong spelling.
- The bindings are checked against QUEUE.tsv plus `work/extra_correct.tsv`. That file holds callee spellings proven with two
  sources while respelling (Ego_r.pdb / FableWin.pdb name plus a landed binder or the retail body). Each entry names its source.
- Family scripts (`work/*.py`) do mechanical respellings. Each row is then graded with `work/g.sh` and logged in
  `work/gradelog.txt`. The most grades on any row is 6, the final `--all` pass included.
- Two main idioms:
  - Same-TU template stand-in bodies become explicit specialisations.
  - Row-local stand-in classes become the PDB class (or the explicit specialisation of the PDB template).

## Status of QUEUE claims
- Rejected: 0x00415DD0 `?GetString@CDefString@@QBE?AVCCharString@@XZ`.
  - The binders (005c0e81, 008e7040) call 0x00415DD0 where the FableWin donor order puts `operator CCharString` (0x018C20E0) as the twin of 0x00415D70.
  - Byte-identical bodies cannot settle which of 0x00415D70 / 0x00415DD0 is GetString, so the claim does not hold with two sources. Left as landed. See NAME_FIXES (question entry).
- All other QUEUE claims used here were confirmed against Ego_r.pdb or FableWin.pdb names and a landed binder.

## Left, and why
- **0x0040F020 CInterfaceMap::LowerBound family**: 26 of 65 rows changed (work/lb.py). The 39 left use:
  - **Inline helpers inside the stand-in struct** (`find`, `erase`, `copy`, `Find`, `LowerBoundRange`, `LowerBoundInline`): 004c8050, 004c9840, 00518dc0, 006412b0, 006594bb, 0065a1fd, 00929b10. These are not CVectorMap members in the PDB. Moving them into the specialisation would invent members, and turning them into free inline helpers is a per-row reshape.
  - **Other stand-in shapes or names**:
    - 004c9d60: CTCEntryVector Data / Compare / Dirty, close to the real layout.
    - 00689130: ComponentEntry.
    - 0069e980: Entry with both overloads.
    - 006e7820, 006e7920, 0089ae30: a nested CInterfaceMap_Retail.
    - 00713740: CTCBaseAny component.
    - 00857b70, 00859d80: InterfaceMap.
    - 00896e60: Map896e60.
    - 0089dff0: BlockSet2.
    - 00927c80: CVectorMapEntry.
    - 00935160: Map/Pair with both overloads.
    - 007e5230, 0089df70: STCEntry with unsigned long keys.
    - 0089a270: a __fastcall VMap_LowerBound free stand-in.
    - 0089c4c0: EntVec_LowerBound.
    - 005e4860, 00766550, 00896f60, 00896fd0, 00898bb0, 0089b390, 0089e4c0, 0089ec60, 0089ecc0, 00915190, 009547d0, 00954e30, 00972490: other local spellings.
  - **Invented namespaces or address-suffixed structs**: 008385b0, 008af850 (NF14 left).
  - Rows changed for other targets that still keep the LowerBound stand-in: 00898b30, 005bf654, 00783380.
- **00415dd0** (005c0e81, 008e7040): claim rejected, see above.
- **005b43a2**: `CLongVector::push_back_005bba94` → `std::vector<EClothingSuitPart>::push_back`.
  - 6 grades, best DIFFER(946v945): VC7.1 CSEs the inline `end()` across the FindLong (0x005BA7F9) call. Retail reloads it.
  - Reverted, landed row unchanged. The attempt is kept in `work/left/005b43a2_attempt.cpp`.
  - FindLong is also a std::find stand-in (NAME_FIXES).
- **006a23e0, 008af850** (NF14 left): both sit in invented namespaces (NRemap006a23e0 / address-suffixed structs) with no shared-header model of the callers' classes in reach. Left.
- **006e11f0, 007565e0, 007adbf0**: invented namespaces X_006e11f0 / X_007565e0 / X_007adbf0. Respelling the target alone keeps the invented scope and would break the "no invented namespaces" rule, so left for a reshape pass.
- **005dab10**: needs CTCBase::PeekComponent, but the legacy CTCWeapon.h model has no CTCBase base. Left.
- **006d0180, 00700940, 00960760**: the binder calls the protected `CTCBase::GetWorld` (0x00686A70) from outside the CTCBase hierarchy (a friend or derived context is not modelled). Left.
- **0049d4b0**: `IsRoomForFile` takes `const wchar_t*` through out-of-line 0x004071C0. A `const unsigned short*` respelling changes its signature, and there is no second source. Left.
- **007edfd0**: the ICF fold at 0x007EAB90. The call site is `CTCCarriedActionUseBase::OnActionUseInHand`, not `CTCActionUseBase::OnActionUse`, so binding the QUEUE name would put a second name on one address. Left.
- **00880200**: the catalog identity is wrong. The body is `GetWorldMap() const` + `GetGroundSizeZAt`, not the catalog's label, so the row needs re-identifying first. Left.

## Descriptive (made-up) names
None introduced. Every respelling uses an Ego_r.pdb / FableWin.pdb spelling.

## Header debt
- `hdr/engine/CTCBase.h` adds `FABLE_RETAIL_CTCBASE_FLAT_PEEKCOMPONENT` (see hdr/README.md).
- 15 PeekComponent rows still declare CTCBase locally; they should move to the shared header.
- 005f4c8f previously used a non-template opt-in `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF`. It now uses the GetDef<CExpressionDef> member template, so that opt-in may be unused repo-wide (check before deleting).
- 007233b0 now uses the retail `engine/CTCPhysicsBase.h` branch (WORLDMAP, SETASMOVING opt-ins already in the repo header).

## Pre-existing quirks seen (not changed)
- 008df3a0: SELF mangles QAE but the Leaf line says AAE (private). The grade is unaffected.
- 0082de20: the catalog name is CThingBuilding::HasBusyOccupants, but Leaf/SELF are `?CountMoneyBags@CThing@@QBE_NXZ`.

## Process breach (disclosed)
- During a disassembly helper call (rdis) I ran `git -C /dev/null status` once by accident. It had no effect and no output
  (/dev/null is not a repository), but it breaks the brief's "never run git in any form". No other git command was run.
