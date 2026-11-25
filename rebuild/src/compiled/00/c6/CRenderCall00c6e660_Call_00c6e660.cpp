// Leaf: ?Initialize@CSalRender@@QAEXXZ
// CSalRender::Initialize() -- put the renderer at the centre: SetPosition (slot 4) with azimuth / elevation / distance / gain 0, zero
//   macro-FX gains, not starting, update set
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E660, 45 bytes, `ret`.
// Name (two sources): (1) GameCODA 2.10 SDK CODAApi.lib Release/SalRender.obj defines ?Initialize@CSalRender@@QAEXXZ, 45 bytes (= retail),
//   between ~CSalRender and ClearFilterHistory in this retail slice; (2) its only caller ?Initialize@CSalSw3DVoice 0x00C55680 relocates it
//   in the SDK object (retail call 0x00C556A3 on m_pRender), and slot 4 of vtable 0x012BEDA4 is SetPosition (the 0x00C54C10 CalcParams
//   call on m_pRender). Was the stand-in ?Call@CRenderCall_00c6e660@@QAEXXZ (table spelling ?Prime@CStreamHandle@@QAEXXZ).
// Option S unit (NF444 / NF445): this body lives in its retail TU-mate's row 0x00C6E630 (GameCODA SalRender.obj slice), included below
//   (no code of its own); that object's copy grades against this address.
#include "../src/compiled/00/c6/CVtable012beda4_CVtable012beda4_00c6e630.cpp"
