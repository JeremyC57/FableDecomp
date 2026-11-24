# Lessons that applied (from landed sibling 0x00407CD0 RedefineControlPassive and the CUserProfileManager.h opt-in comments)
- Element access through vector operator[] each time (retail reloads [this+54h]/[this+60h] after every store) -> no local reference.
- Use the existing PASSIVEINPUTS + RESETASSIGNEDINPUTS opt-ins for AssignedInputsPassive (+0x54) / AssignedInputsAggressive (+0x60).
- ControllerType values as in the landed IsSameControl: 1 Xbox button, 2 keyboard key, 3 mouse button.

# 00407a70 -- RELOCATION_MATCH 293v293, first grade (stock flags). payload.json + oracle.tsv written; vt.py PASS.
- Names: SetAssignedInputKeyboard 0x00407A70 / SetAssignedInputMouse 0x00407BA0. Sources: FableWin.pdb (0x01886A20 / 0x01886DD0, both
  749 debug bytes) and Ego_r.pdb mangled names, both PDBs order the pair right before RedefineControlPassive = retail 0x00407CD0, which
  follows 0x00407BA0+0x125 directly; the type-2/KeyboardKey(+8) v type-3/MouseButton(+0x10) split names which is which.
- Header: overlay opt-in FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT (two declarations, hdr/README.md).
- vland self-check: stock vland.py stops at OUTSIDE_MANIFEST (the address is an unlisted start in the 0x004075C0 gap, UNLISTED_STARTS.tsv /
  landing gh SPLITS.tsv), so the coordinator must add the manifest start before landing. With the start added in memory only
  (/opt/fable_lane/w22/b24c/vland_split.py) strict vland gives RELOCATION_MATCH PASS, no WARN / RELOC_ / NOT_GENUINE lines (vland_split.log).
- Tool fix: Wine cl segfaults (rc -11) while TMPDIR points at the session scratchpad; unset TMPDIR TMP.
- New lesson: an "unlisted" twin pair with no callers is named by PDB order (both PDBs list members in source order, retail keeps it).
