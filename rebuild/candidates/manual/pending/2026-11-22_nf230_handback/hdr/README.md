# NF230 header overlay (2026-11-22): three add-only, member-specific opt-ins (entry 2, rows 0x00597BF2 / 0x0059A238 / 0x0041D0A8)

Base copy: `orig/engine/CNewFrontendGameComponent.h` (the repo tree as of this run, = NF229's landed overlay; no other agent's overlay of
this header is open: `$S/w22/*/hdr` has only NF229 / NF47 / NF76 copies, all landed). Each opt-in sits under a new macro that no landed row
or header defines (grep over rebuild/ and every `$S/w22/*/hdr`: no hit outside this hand-back), so every existing includer compiles the same
tokens as before. All three sit in the public section after the landed STARTGAME block, outside the RUN / INIT / GETLASTACTIVE... layout
branches, so they work on every path. None changes the layout (sizeof stays 0x148; the pinned `FABLE_STATIC_ASSERT_AT(89, ...)` moves down
the file but keeps its pinned number).

- `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RESETATTRACTMODETIMER`: `void ResetAttractModeTimer();` (declaration). Ego_r.pdb and FableWin.pdb
  ?ResetAttractModeTimer@CNewFrontendGameComponent@@QAEXXZ (Ego_d field list: public), out of line at 0x0042DBEE (landed). Users:
  rows/00597bf2.cpp, rows/0059a238.cpp (both bound this spelling at HEAD from row-local models).
- `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT`: `virtual void SetQuit();` (declaration, the PUBLIC override of the protected
  ?SetQuit@CGameComponent@@MAEXXZ). FableWin.pdb ?SetQuit@CNewFrontendGameComponent@@UAEXXZ and Ego_d.pdb's field list (SetQuit, public
  virtual); retail: the RTTI vtable CNewFrontendGameComponent slot 4 holds 0x00430340 (`mov byte ptr [ecx+8],1 ; ret`), not CGameComponent's
  0x00415930. An override adds no slot. User: rows/0059a238.cpp (the 0x128 case `call [eax+10h]` on the static). Note: the landed self row
  0x00430340 still spells QAEXXZ (NAME_FIXES).
- `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETPSOUNDBANK`: `CASoundBank* GetPSoundBank() { return PSampleBank.Get(); }`. Name, access and
  signature from Ego_d.pdb only (LF_ONEMETHOD `GetPSoundBank`, public, non-const, return CASoundBank*; not in Ego_r / FableWin, where it is
  inline with no copy); the second source is the retail body 0x0041D0A8 (`mov eax,[0x013B871C] ... mov eax,[eax+50h]`, PSampleBank's Data
  word). The body is reconstructed from those bytes with the FableWin CCountedPointer<T>::Get (one PDB + the retail body, the nf228 /
  nf229 accessor rule). User: rows/0041d0a8.cpp.

Proof: pin_asserts.py rebuild/include hdr engine/CNewFrontendGameComponent.h: pinned 0. assert_names_check.py rebuild/include hdr:
mismatches 0. equiv_check.py (rebuild/include v a copy with this header) over the 29 landed rows whose source or test includes the header
(work/incl_cnfgc.txt; no header under rebuild/include includes it): 0 differing (work/equiv_cnfgc.tsv is empty).
Landed includers (29, direct): 004069e0 00406ae0 00406c20 00407030 004083e0 004093e0 0040bca0 0040c790 0040cec0 0041cfb2 0041e3f6 0042ec7c
0042f75e 00446720 00497b30 00498490 00541e60 0054f370 0054f410 0054fbc0 00558170 00594f36 00594fdb 00595a03 00595cc1 00598463 00598a1c
00599d5c 00599e3f; none defines the three macros, so none changes. Every user row includes the header itself (NF213 lesson).
