# Chunk 527 lessons

## GOTCHAS one-liners
- VC7.1: `if (!X) return;` hoists the following `lea ecx,local` above the branch; the `if (X) { ... }` block form does not -- pick by where retail's lea sits.
- VC7.1: an assignment in a ctor body (`{ Opened = false; }`) stores after the base/member ctor calls; an init-list entry stores before.
- VC7.1: an implicit `const char*` -> `const CCharString&` conversion temporary is passed by `lea [frame]`; an explicit `CCharString("")` argument passes the ctor's eax.
- CArray<char>::GetAsCArray is `size() != 0 ? &(*this)[0] : GetZeroSizeArray()` (FableWin.pdb names both); the empty branch is visible in retail.
- Virtual-call argument order shows in push order; NotifyEntryUpdated is (dataType, index, const CArray<char>&), slot 13, protected.
- STLport vector _M_insert_overflow: put the helper bodies (__uninitialized_copy/_fill_n, noinline __fastcall) in the TU or VC reloads the tag/pair temporary; the helpers bind PAV/PAV (not PBV) from _M_start.
- STLport 4.6 `pair` declares an explicit copy ctor; omitting it changes the insert_unique return copy.
- A chained `f(x).GetAsLowerCase()` destroys the intermediate temporary at end of full-expression; a named local of the final value destroys it earlier -- this is the whole InitEntry residue.
- `__declspec(align(8))` on CCriticalSectionLock produces retail's `and esp,-8` frame.
- `C3DVector a(p.X, p.Y, p.Z); C3DVector b(a);` reproduces the FPU fld/fstp copy pattern; a struct copy gives mov pairs.
- Retail bare `fistp dword` inside GFFastCos/GFFastSin is the engine's inline-asm float-to-long; C++ cannot emit it, so the row is retail_asm, not DIFFER.
- xref.py exits silently when `S` is not exported in the same shell.

## Review at landing
- 0x009CE050 InitEntry next step: combine probe ce050_b's named local `CCharString name = ConvertPathToFilename(filename).GetAsLowerCase();` with ce050_d's in-TU `_M_insert_overflow` body (each fixed one half; never graded together, grade budget spent).
- 0x00411910: spell the out-of-line ctor as `vector<char>(unsigned int)` or a CArray<char> ctor -- check every landed caller binds the same one.
- 0x009C5110: FreeBuffers (invented) vs ClearBuffers (Ego_r.pdb, probable) -- respell landed callers if accepted.
- 0x00A14F70: many conflicting landed spellings and `CANavQuadTreeNode_xxx` placeholders; Ego_r.pdb has `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` as the likely identity -- not proven here, needs a second source.
- 0x009C5170 ~CFileInstaller: residue looks like a real `volatile` thread-flag member (cmp byte,bl from an ebx zero); purity forbids that keyword, so the row stays DIFFER unless the rule changes.
- vland RELOC_NAME warns (GetAsLowerCase vs ToLower, ~CBaseClassNonCopyable vs ~CBaseClass, UninitCopy_* labels) are DB label errors, not our spelling; NAME_FIXES.tsv covers them.
