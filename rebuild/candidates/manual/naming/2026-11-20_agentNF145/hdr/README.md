# NF145 header overlay

Bases in `orig/engine/` are the repo working-tree copies at the start of this round; re-checked at hand-back: both still byte-equal to
rebuild/include (nf141 / nf142 did not move them). An unmodified copy of CCreatureAction_ThrowShotObject.h was removed (no change needed). NF143 / NF144
overlays were empty when read (no shared header in common).

## engine/CMemoryAllocator.h
Changes (q11810 inline control, q11809 port):
- **Removed** the nf140 opt-in `FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE` (it only put FABLE_FORCEINLINE on Allocate). Its only
  user, 0x009D52D0, now carries `// Flags: /d2inlT78` (EXTRA_RULES 2026-11-08 preference 2) and no longer defines the macro. Grep of
  rebuild/src/compiled, rebuild/tests and the NF14x hand-backs: no other user.
- `FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE`: Deallocate and Destroy are plain `inline` (were FABLE_FORCEINLINE). Of its 12 users, 7 carry
  `// Flags: /d2inlT114` and 5 match with no flag (sweep in NOTES.md). The `#include "fable_compiler.h"` that only these two forces needed
  is gone.
- New `FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR` (row 0x0122A9A0, used with _DEALLOCATE and _CTOR): declares the VirtualFree import, the
  nested `class CDeallocator { public: ~CDeallocator(); }` and `static CDeallocator Deallocator` (FableWin.pdb
  ??1CDeallocator@CMemoryAllocator@@QAE@XZ and the "dynamic atexit destructor for 'CMemoryAllocator::Deallocator'" record), and gives
  the class operator delete its body (VirtualFree MEM_RELEASE; out-of-line copy 0x009F1F90) for that TU, which retail expands there.
  Without the macro the operator delete declaration is the _DEALLOCATE one unchanged.
- Token identity (cpp -P, orig vs hdr): identical with no macro, and with each of _CTOR, _VARIABLESIZE, _ALIGNED, _SAVEBINARY_PASS23.
  Only the two nf140 inline-control macros (all of whose users are in this hand-back) and the new _DEALLOCATOR differ.
- pin_asserts.py: pinned 0 (no FABLE_STATIC_ASSERT; the size typedef under _CTOR is untouched).
- Landed includers (grep rebuild/src/compiled): 00416005 009af7a0 009a4350 009a1660 009a3640 009c6020 009c5310 009c6d60 009d61f0 009d52d0
  009f20b0 009f2000 009f2450 009f2010 00a02220 00a02370 00a02570 00a02190 00a65720 00b482a0 00bd9c40 00bd9af0 00bda360 00bda470 00bda3d0
  00bd9ce0 00bd9b90 00bda150 00f370c0 0122a9a0. The 12 _DEALLOCATE users, 0x009D52D0 and 0x0122A9A0 are in this hand-back (regraded);
  every other includer defines none of the changed macros, so the overlay changes none of them.

## engine/CDisplayManager.h
- New add-only opt-in `FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE` (q11808; rows 0x009BFE70, 0x009BFF10, 0x009BFF40, 0x009C0020; used with the
  existing _CREATEDEVICE for ECreateDeviceSuccess): public declarations of both ChangePresentationInterval overloads, SetDisplayMode and
  SetExclusiveMode, and the private ResetDevice, all spelled as Ego_r.pdb / FableWin.pdb (no existing opt-in declared them; NF142's
  overlay is empty).
- Token identity: removing the two RESETDEVICE blocks gives the base byte for byte, so every other configuration is unchanged.
- pin_asserts.py: pinned 0 (the one assert is already FABLE_STATIC_ASSERT_AT(166, ...), line-independent).
- Landed includers: 112 files under rebuild/src/compiled include CDisplayManager.h; none defines FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
  (grep of rebuild/), so the opt-in changes none of them.
