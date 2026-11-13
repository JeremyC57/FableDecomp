## 0x00A079B0 CRenderStateManager::CDeviceState::CDeviceState() -- MATCH (1 grade, strict headers MATCH, vt PASS)
- What made it match: shared header retail branch (`#define FABLE_RETAIL_CRENDERSTATEMANAGER` + engine/CRenderStateManager.h) plus an
  out-of-class `inline` CStateInfo ctor body in the row file (member-init list CurrentState, DesiredState, BookmarkMask, DirtyListFlag,
  StateType = 0), empty CDeviceState ctor body. VC7.1 unrolls the 22 x 8 array element ctors itself. No flags, no pragmas, no overlay.
- Name fix: manifest label `CPSCOrbit::CPSCOrbit` is wrong -> `??0CDeviceState@CRenderStateManager@@QAE@XZ` (Ego_r.pdb + FableWin.pdb
  symbol, sole caller 0x00A05970 `lea ecx,[esi+0x2818]` = CRenderStateManager::DeviceState; sizeof 0x120C). Real CPSCOrbit ctor 0x00AD7F00.
