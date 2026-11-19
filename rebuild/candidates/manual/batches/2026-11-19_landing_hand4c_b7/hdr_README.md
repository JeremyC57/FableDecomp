# hand4c header overlay (2026-11-19)
Every earlier hand4c opt-in is landed (b1..b5). Batch 6 (0x005542F0) uses existing repo opt-ins only.

## Batch 7: FABLE_RETAIL_CSALSW3D_CALCPARAMS (row 0x00C54C10 CSalSw3DVoice::CalcParams)
Copies of the CURRENT repo headers with one new additive opt-in block each (no existing line moved or edited, no
FABLE_STATIC_ASSERT in any of them), plus two new headers. Names / access / signatures: the GameCODA 2.10 CODAApiDbg.lib
SalSw3DVoice object's CodeView and the CODAApi(Dbg) CalcParams call relocations; bodies of the inline accessors: the release
instruction they expand to (cited per block). The row also defines FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS,
FABLE_RETAIL_PASS22_CCODA3D_CREATE (CCodaContext member tail + `extern CCodaContext CODA`) and
FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23 (the 3D voice members, six-channel history).
- engine/CCoda3DSource.h: `const CODA3DSOURCE& Get3DParams()const` (public).
- engine/CCodaSource.h: `unsigned int GetCaps()const` (public, m_ffCaps).
- engine/CSalBaseVoice.h: protected `unsigned int GetHrtfMode()const` (m_ffParams & 0x3c00) and the protected static
  `CalcAzimuthElevation(float&,float&,const CCodaVector&)` (0x00C599C0); file-scope `class CCodaVector;` in the block.
- engine/CSalSwBaseVoice.h: protected `SetPitch(float,const CODASOURCE&)` (0x00C5A500) and inline `ReleasePlayBuffer()`
  (m_PlayBuffer.Release(); m_pcBuffer=0).
- engine/CSalSwBuffer.h: public non-virtual `Release()` (0x00C5CE40).
- engine/CSalSwFilter.h: public `ClearHistory()` / `SetLP(const _CLPFILTER*,float)` / `SetEQ(const _CEQFILTER*,float)`.
- engine/CCodaContext.h: public `GetEAX()` / `GetDistanceUnits()` / `GetMacroFxFactor()` (needs the member tail).
- engine/CSalPlatform.h: public `CSalBaseReverb* GetReverb()const` (m_pReverb +0x17c).
- engine/CodaCaps.h: `#define C_CAPS_EAX (0x00002000)` (SDK CODA.h).
- NEW engine/CSalRender.h: the renderer's SDK virtual table (slot +0x10 SetPosition); members not projected.
- NEW engine/CCodaEAX.h: the native 0xf0-byte layout (ctor 0x00C4E2A0, allocation 0x00C3DF28, Create 0x00C52840, release of
  m_pEAXListener 0x00C4E2D0) with SDK member names; `IsHardwareEAX()` under the opt-in.
