# NF294 overlay headers

## engine/CAFile.h (base: orig/hdr/engine/CAFile.h = repo HEAD copy, 2026-11-23)
- New add-only opt-in `FABLE_RETAIL_CAFILE_MAKEPATH`. It sits at class scope, just above `#ifdef FABLE_RETAIL_CAFILE`, and holds one comment
  block and the declaration `static CWideString __fastcall MakePath(...)` (0x00997780).
  - Guard: `#if defined(FABLE_RETAIL_CAFILE_MAKEPATH) && !(defined(FABLE_RETAIL_CAFILE) && defined(FABLE_RETAIL_CAFILE_CREATEDIRECTORY))`.
    The existing CREATEDIRECTORY copy, which is nested in the FABLE_RETAIL_CAFILE branch, is used when both are on, so the member is
    never declared twice.
  - Spelling: `?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z`, in both PDB name sets (evidence/pdbq.txt). The self row 0x00997780 defines it,
    and the landed callers 0x004FAFF0 / 0x004FB0A0 / 0x00999620 / 0x00999AB0 bind it.
  - Why not the existing opt-in: `FABLE_RETAIL_CAFILE` pulls engine/CArray.h. In row 0x00997800 that adds `/DEFAULTLIB:"libcp"` to `.drectve`
    (objsec bad 1: 42 v 62 bytes, tried and rejected).
  - User: binder 0x00997800 only.
- No existing line was edited, and there is no layout change (a static member function). CAFile.h has no `FABLE_STATIC_ASSERT`, so nothing moves.
- equiv_check (evidence/equiv_r1.txt) covered the header's landed includers plus every row that includes a relative source path: 2,951 rows,
  0 differ. The planted-token control (a member typedef just above the opt-in, outside it) flags 132.
- Other agents' overlays: NF292's hdr/ holds CMemoryDataOutputStream.h, CTCBuyableHouse.h, CTCDoor.h and CWorldMap.h (no CAFile.h).
  NF293's hdr/ was empty when checked (2026-11-23).
