# NF229 header overlay (2026-11-22): one add-only, member-specific opt-in (entry 2, row 0x00599D5C)

Base copy: `orig/engine/CNewFrontendGameComponent.h` (the repo tree as of this run; no other agent's overlay of this header exists).
The opt-in sits under a new macro that no landed row or header defines (grep over rebuild/), so every existing includer compiles the
same tokens as before.

- engine/CNewFrontendGameComponent.h: `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETLASTACTIVEJOYSTICKDEVICENUMBER`, inside the default
  (neither RUN nor INIT) branch: the opaque `_pad_0x6c[0x148 - 0x6c]` is split around Ego_r.pdb's `long LastActiveJoystickDeviceNumber`
  at retail +0x140 (Ego_r +0x148; the two STLport vectors are 0xC in retail, the same shift the RUN opt-in documents), and adds the public
  inline `long GetLastActiveJoystickDeviceNumber() const` (FableWin.pdb ?GetLastActiveJoystickDeviceNumber@CNewFrontendGameComponent@@QBEJXZ
  0x0201D7C8; Ego_r.pdb has the member but not the accessor: one PDB for the accessor name + the retail body 0x00599E1B
  `mov eax,[0x013B871C] ; push dword ptr [eax+140h]` as the second source). No layout change: sizeof stays 0x148 (the pinned
  `FABLE_STATIC_ASSERT_AT(89, ...)` holds; it was already pinned, so the 14 added lines move nothing).
- Proof: pin_asserts.py: 0 pinned. assert_names_check.py orig/ v hdr/: mismatches 0. equiv_check.py (rebuild/include v a copy with this
  header) over the 26 landed rows whose source or test includes the header (work/incl_cnfgc.txt; no header under rebuild/include
  includes it): 0 differing (work/equiv_cnfgc.tsv).
- Landed includers (26, direct): 004069e0 00406ae0 00406c20 00407030 004083e0 004093e0 0040bca0 0040c790 0040cec0 0041cfb2 0041e3f6
  0042ec7c 0042f75e 00497b30 00498490 00541e60 0054f370 0054f410 0054fbc0 00594f36 00594fdb 00595a03 00595cc1 00598463 00598a1c 00599e3f;
  none defines the macro.
- User: rows/00599d5c.cpp (it includes the header itself; NF213 lesson). rows/00446720.cpp and rows/00558170.cpp include the header on
  its default path (no opt-in).
- Scope: the opt-in lives in the default branch only (neither FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN nor _INIT defined); with RUN the
  member is already declared (no accessor), with INIT the opt-in does nothing. A future 0x00599D5C test that sets the static through the
  accepted INIT-stub trick therefore cannot also use the accessor: it would need the accessor in the INIT branch too (a later opt-in).
- New macro check: `grep -rl FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETLASTACTIVEJOYSTICKDEVICENUMBER` over rebuild/ and every
  $S/w22/*/hdr overlay: no hit outside this hand-back.
