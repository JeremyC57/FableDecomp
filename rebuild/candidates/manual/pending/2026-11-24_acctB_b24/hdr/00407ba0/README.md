- engine/CUserProfileManager.h (copy of the current repo header, 2026-11-24) + FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT (inside the
  PASSIVEINPUTS block, after RESETASSIGNEDINPUTSWASD): declarations of SetAssignedInputKeyboard 0x00407A70 / SetAssignedInputMouse 0x00407BA0.
  Evidence: FableWin.pdb + Ego_r.pdb mangled names (public, QAE), ordered right before RedefineControlPassive (retail 0x00407CD0).
