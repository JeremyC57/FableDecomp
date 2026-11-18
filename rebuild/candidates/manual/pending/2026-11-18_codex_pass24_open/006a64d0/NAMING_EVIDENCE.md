# 0x006A64D0 naming evidence

The selected RTM object emits `?DrawPreparePrimitivesForRendering@CThingPhysical@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z`. Ego_r donor 0x006859F0 and FableWin donor 0x01F21410 carries this exact decorated spelling.
The Ego_d module record identifies `CThingPhysical::DrawPreparePrimitivesForRendering` in `thing_physical.obj`. CThingPhysical::DrawPreparePrimitivesForRendering (debug length 1355).
Retail vtable 0x01244E84 slot 36 resolves to this address under `CThingPhysical`.

## Shared headers

The selected source uses row opt-ins in `CMainGameComponent.h`, `CThingDrawInformation.h`, `CThingPhysical.h`, `CThingPhysicalDebugInfo.h`. See the history receipt for the exact leaf and the relocation report for operand evidence.

## Behavioral fixture

The selected body exercises renderer gating, hidden state, near and far debug-name checks, generator delegation, and the physics flag string and its placement. Graphics, renderer, fonts, and the thing component registry are fixtures.
