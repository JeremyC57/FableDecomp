# Chunk 106 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Fable statically links Microsoft's **DirectShow BaseClasses** (the "CTextureRenderer::vfunc_N" rows, 0x00CA4xxx-0x00CACxxx): the SDK
  source reproduces byte-exact once slots/offsets line up (CUnknown 0-3, CBaseFilter StreamTime 4, CBaseRenderer SourceThreadCanWait 10,
  OnRenderStart/End 14/15, ScheduleSample 21, GetSampleTimes 22, ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44;
  m_pClock +0x18, m_tStart +0x20, m_clsid +0x28, m_ThreadSignal +0x58, m_bStreaming +0x64). STDMETHODIMP overrides of a method that exists
  only in a non-primary interface take `this` = that sub-object (`add ecx,1Ch` for m_clsid from IBaseFilter at +0xc; IQualProp at +0xe0 ->
  `lea ecx,[eax-0E0h]`). Landed "CVideoPlayer_00ca9a90" / "CAudioDevice_00ca4d80" names are probably BaseClasses classes too.
- A local model that introduces a vptr in a class that also holds an `__int64` gets its vptr padded to 8 bytes by MSVC (members shift +4):
  pad from +8, not +4 (0x00CA77A0).
- VC7.1 (no /Op) re-associates float sums: `X*X + Y*Y + Z*Z` over x87 temporaries came out (z*z + x*x) + y*y; retail's (z*z + y*y) + x*x
  is the grouping `X*X + (Y*Y + Z*Z)` (0x00805920). Try groupings before parking an x87 order diff.
- A zero-fill loop compiled to `test ecx,ecx ; lea edi,[..] ; jbe ; xor eax,eax ; rep stosd` needs the count AND the array pointer as
  named locals; against the member count the stores alias it (0x00A698B0).
- `push 1 ; push 0 ; call [vt+N] ; ... push eax ; call [vt+M]` is `Outer(Inner(), 0, 1)`: the trailing constants of the OUTER call are
  pushed before the inner virtual call that builds its first argument (right-to-left evaluation); reading them as the inner call's
  arguments makes the stack offsets look impossible (0x0054F580).
- The wave-25 STLport `_Vector_base` + `_STLP_alloc_proxy` model also fixes an unrolled array-of-vector-pairs constructor (0x006B5FB0):
  the plain three-pointer CArray ctor advances the element pointer before the second element's first stores.
- PDB bitfields `unsigned long a : 28, b : 4` copied member by member give retail's `((src ^ dst) & mask) ^ dst` merges (0x009F9D00);
  engine headers flatten bitfields into byte arrays, so such rows need a local model.
- A function reached only by `jmp` from an unwind funclet region (0x0121xxxx) is a stack object's destructor (0x00C098F0); a body that is
  `call`ed only by a `??_G`-shaped wrapper is the `??1`, not a constructor (0x00C202F0).
- Retail CTextureManager is 0x40 bytes smaller than the PDB (pool 0x54 vs 0x58); retail CAppearanceModifierScalingDef is 0x48 (floats at
  +0x3c/+0x40/+0x44, PDB +0x40..); retail CTemporaryBuffer / CTeamSpawn / CTestQuest match the PDB.

Review at landing:
- No row needed more than 4 grades. 0x00C202F0 parked after 4 (vptr store order of an inlined two-vptr base destructor).
- Local models next to existing engine headers: CTexture (009f9d00, 009f9e00, 009f9e80, 00b3b1f0, 00b3b5b0 -- header flattens the
  bitfields and has no ctor/dtor), CSurface (009f9e00 -- header has `__vftable` data member), CTextureManager (00a696d0, 00a698b0 --
  header carries PDB offsets, retail differs), CText (0054f580 -- only the vtable is used). CTattooDef rows use engine/CTattooDef.h.
- Already-landed sources: `rebuild/src/compiled/00/47/CAppearanceModifierScalingDef_CAppearanceModifierScalingDef_004784b3.cpp`
  names the three PDB floats Attractiveness/Scariness/Goodstrength as `long m_scaleX/Y/Z`; landed "CVideoPlayer_00ca9a90" and
  "CAudioDevice_00ca4d80" sources (DirectShow region) should be re-checked against the BaseClasses names.
- Rows renamed away from the catalog (module/name differ from chunk106.tsv): 009f9e00, 00b3b1f0, 00b3b5b0, 00c098f0, 004e1537, 0057fc71,
  0057fc9c, 006aed80, 00757640, 00780070, 009f8690, 00546aa0, 00546cd0 (NUISystem namespace), 00546d30, 0054f580, 00c2e270, 004d1eb0,
  0081b2a0, 0081d7d0, 00ca49f0, 00ca4b20, 00ca5cf0, 00ca6020, 00ca6050, 00ca6580, 00ca7620, 00ca77a0, 00cacd20, 004a8f50 -- each carries a
  `// Leaf:` line; the lander must use it.
- No catalog row in this chunk holds two functions.
