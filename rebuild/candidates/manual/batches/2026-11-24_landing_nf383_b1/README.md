# Landing nf383 b1: the CList destructor thunk, ClearAllMaintainedEvents for three input devices, and the vector<C3DVector> copy constructor

Agent NF383 (pending/2026-11-24_nf383_handback).
- 0x00429D8E is `??1?$CCountedPointer@VCList@NUISystem@@@@QAE@XZ`, the 5-byte `jmp ReleaseData`; 0x00429288 is ReleaseData<CList> (FableWin ShareData<CList> calls ReleaseData; retail ShareData calls 0x00429288). Table: the destructor line moves from 0x00429288 (witness 00570332) to 0x00429D8E (witness 0056ad40, whose leaf relocation maps there).
- 0x009E42C0 / 0x00A66C00 / 0x00A66860 are `ClearAllMaintainedEvents` of CJoystick / CMouse / CKeyboard (FableWin CInputManager::ClearEvents calls them; Ego_r lib_mouse.obj / lib_keyboard.obj 26 B); binder 0x009F5540 respelled; 3 table replaces. CJoystick.h gains the additive opt-in FABLE_RETAIL_CJOYSTICK_MAINTAINEDEVENTS_LIST (pin_asserts 0 pinned, whole-tree equiv_check 57,158 rows, 0 differing).
- 0x006F0810 is the vector<C3DVector> copy constructor.
- T3 nf292 0x00996B60 (CMemoryDataOutputStream::Clear defined by four landed rows) is closed (T3_CLOSED.tsv): the three device rows no longer define it.
- Gates: vland HEAD and new RELOCATION_MATCH PASS, objsec ALL_EQ, pick-any and link clean, census by compile, apply_table 0 misses.
