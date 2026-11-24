# RP1 header overlay

Base: `orig/engine/CVsyncCallbackTimer.h` is the repo working-tree copy at the start of this round, re-checked at hand-back: still
byte-equal to rebuild/include/engine/CVsyncCallbackTimer.h. No other header is overlaid: engine/IDirect3D9.h is used as it is in the tree
(0x009BE870 / 0x009BFB30 follow B's b2 / b3 recipe, which needed no header change). NF146's overlay is empty and NF147's does not touch
this header (read at the start and again at hand-back).

## engine/CVsyncCallbackTimer.h (option A: add-only opt-ins, no layout claim on the class)
- `FABLE_RETAIL_CVSYNCCALLBACKTIMER_PENDINGQUERY`: `struct IDirect3DQuery9;` and the public nested
  `struct CPendingQuery { IDirect3DQuery9* Query; long Frame; double TimeStamp; };` (0x10, Ego_r.pdb; struct as every PDB instance
  mangles it). Users: 19 of the 20 group-1 STL rows (0x009F7BB0 only lost its dead local model and includes nothing).
- `FABLE_RETAIL_CVSYNCCALLBACKTIMERINFO` / `FABLE_RETAIL_CVSYNCCALLBACKTIMER_GETINFO` (either defines the record):
  `struct CVsyncCallbackTimerInfo` with the Ego_r members (CPUTimeStamp +0, CPUPrevTimeStamp +8, CPUFrameCount +0x10, GPUTimeStamp
  +0x18, GPUPrevTimeStamp +0x20, GPUFrameCount +0x28), the ctor declared only (0x009F7020 defines it out of line), and
  `FABLE_STATIC_ASSERT(sizeof(CVsyncCallbackTimerInfo) == 0x30)` inside the opt-in (with `#include "rebuild_abi.h"`). Layout proof:
  struct_layouts_egor.tsv (48 bytes, those offsets) + the retail stores of 0x009F7020 (ctor) and 0x009F7050 (GetInfo).
  `_GETINFO` also declares `CVsyncCallbackTimerInfo GetInfo();` (0x009F7050). Users: 0x009F7020 (_INFO), 0x009F7050 and 0x004166E2 (_GETINFO).
- `FABLE_RETAIL_CVSYNCCALLBACKTIMER_DTOR`: public `~CVsyncCallbackTimer();` (0x009F7330). User: 0x009C01C0. Kept on its own macro because
  declaring the destructor changes what `delete` / boost::checked_delete emit in an includer.
- `FABLE_RETAIL_CVSYNCCALLBACKTIMER_ONPREDEVICERESET`: public `void OnPreDeviceReset();` (0x009F73A0). User: 0x009BF500.
- `FABLE_RETAIL_CVSYNCCALLBACKTIMER_ONPOSTSWAPSCREENS`: public `void OnPostSwapScreens();` (0x009F73C0, Ego_r + FableWin; added beyond the
  brief's method list because 0x009BF2D0 / 0x009BEEB0 call it). Users: 0x009BF2D0, 0x009BEEB0.
- Not declared: the constructor (0x009F70E0; its only binder 0x009C0E50 is blocked, see NOTES) and the private ClearAllQueries
  (0x009F7250; only group-2 rows call it). They wait for option B / the group-2 pass, so the header ships no declaration no finished row uses.
- Token identity: `cpp -P` of orig vs hdr with no macro defined: identical (work/tok_orig.txt == work/tok_hdr.txt).
- pin_asserts.py: pinned 0. It reports the one new assert (line 26, inside the new `_INFO` / `_GETINFO` opt-in), as expected for a new
  assert; no existing assert moved (the header had none).
- Landed includers (`grep -rlE 'CVsyncCallbackTimer\.h' rebuild/src/compiled rebuild/tests`): only 0x009BF7E0 CDisplayManager::CreateDevice.
  It defines none of the new macros; regraded with the overlay: RELOCATION_MATCH PASS, defined / undefined symbol sets identical to HEAD
  (work/g3_vland.log). So the opt-ins change none of them. 0x009BFB30 includes the header plainly (no macro) and is in this hand-back.
