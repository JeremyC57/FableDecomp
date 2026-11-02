# Codex pass 11 -- retry agent b: 3 of 23 unfinished rows landed (2026-11-02)

An in-house agent retried 23 of the 46 rows Codex could not finish (10+ own attempts per row, from Codex's best source and probe log). Landed
(`verify.log`: 3 WINS, all PASS):
- 0x0099DCB0 `_DisplayErrorDialog@8` RELOCATION_MATCH 1455/1455 (/GS). Extent fix: 1455 bytes, not 1454 (the int3 VC7.1 emits after the
  call to the no-return `exit()`). Name in both PDBs (Ego_r 0x00AEB890, FableWin 0x02F6C0A0). Its globals take Codex's raw-PDB data names
  (pass 10 precedent: single-source raw-PDB data names land when the address identity is proven): 0x0138E060 `?gApplicationLanguage@@3GA`,
  0x013BD6EC `_hStrings`, 0x013BD6E8 `?pConfig@@3PAVConfigDetection@@A`; the older landed stand-ins are queued PRIORITY (cx11r).
- 0x00C0F440 MATCH 8/8: libpk, no PDB or RTTI: descriptive `CVtable_012b2308::GetFirst_00c0f440` (slot 11 of vtable 0x012B2308).
- 0x00631C60 `NUISystem::CUIDef::Transfer` RELOCATION_MATCH 1524/1524 (vtable 0x01259F8C slot 18, RTTI, Ego_r 0x006758F0). Members are
  transferred in retail's order through the one-argument Transfer<T> / TransferVector<T> templates.

Coordinator decision (owner principle 2026-11-02, "if that is how retail did it then you should too"): retail's C2DVector is byte-packed.
Three sources agree: retail reads CUIDef's C2DVector member at +0x142 / +0x146; Ego_r places C2DVectors at CUIDef+0x15E and
COldFileFormatThingBrushInfo+0x11, which 4-byte alignment cannot produce; the raw PDB type record carries the packed property. So
`FABLE_RETAIL_C2DVECTOR` (engine/C2DVector.h, opt-in) declares the class under `#pragma pack(push, 1)` as retail did, the same device the
generated CDefClassBase / CParentDefClassBase headers already carry. `FABLE_RETAIL_CUIDEF` (engine/NUISystem_CUIDef.h) types every member at its
offset. Both default paths unchanged (hdrcheck); the 23 landed includers of the two headers rechecked 23/23 (2 MATCH + 21 RELOCATION_MATCH,
includer_recheck_grades.tsv). New: engine/ErrorDialog.h, ConfigDetection.h (debt: a fourth `struct _GUID` definition), TRetVoid.h, Mass.h.

Not authored: 0x006286F0 GFPlayFullScreenXBoxMovie (naming blocker resolved: CASoundManager is forward-only in Ego_r; the two unnamed bool
setters 0x009915E0 / 0x00991620 take descriptive names; the 1864-byte body needs shared video / XMV / engine headers that do not exist yet).
Parked (19, best attempts in pending/2026-11-02_codex_pass11/retry_differ/, residues in STATUS.tsv): several are pure scheduling ties at equal
size (008eec70 1129v1129, 0087aa10 1504v1504, 00a8f1b0 1469v1469, 00803540 1108v1108, 00875fb0 1315v1315); 0049124e, 00762620, 006159a0,
00509a20 and 00763060 still lean on Codex's second CCharString model or feature macros and need rebasing before they could land.
NAME_FIXES (18): the CPersistContext vector instances 0x00464841 / 0x00464CBF / 0x00466A47 are `TransferVector<T>`, ten CUIDef callee
instances, 0x0099D470 `@RetrieveString@16`; queued as cx11r. Labels: ghidra_out/labels_codex_pass11d_2026-11-02.tsv.
