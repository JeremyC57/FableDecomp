# NF109 overlay headers (FABLE_HDR_OVERLAY=$S/w22/agentNF109/hdr)

Base copies (byte-identical to rebuild/include at the start of the slice) are in ../orig/. Five headers. Every change is an add-only `#ifdef` opt-in
with a member-specific macro; with no macro defined each header is token-identical to the repo copy.
pin_asserts.py over all five headers: `pinned 0` (no FABLE_STATIC_ASSERT moved or added).

| header | opt-in macro | adds | used by |
|---|---|---|---|
| engine/CFloatFilterParams.h | FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER | `friend class CFloatFilter;` (the CODAApiDbg.lib CFloatFilter::Process bodies read m_fGain / m_fA / m_fB directly) | rows 0x00C74CE0, 0x00C74D30 |
| engine/CFloatLongReverb.h | FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER | the private non-fading ProcessInputFilter declaration (SDK CodeView spelling; the fading overload is already in the repo header) | rows 0x00C756C0, 0x00C75A20, 0x00C75AB0 |
| engine/CFloatLongReverbModule.h | FABLE_RETAIL_CFLOATLONGREVERBMODULE_TOKEN | the SDK public inline `int Token() const` | row 0x00C75AB0 |
| engine/CFloatAllpassSection.h | FABLE_RETAIL_CFLOATALLPASSSECTION_CTOR | out-of-line ctor declaration `CFloatAllpassSection();` (native 0x00C74F60) | rows 0x00C74F60, 0x00C75410 |
| engine/CFloatAllpassSection.h | FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE | `void Release() { m_pBuffer = 0; m_length = 0; }`: SDK 2.10 CodeView public ?Release@CFloatAllpassSection@@QAEXXZ, a CODAApi.lib release member (body from its bytes) that the release CFloatDiffuser::Release expands; the in-class inline placement is NF109's (it lets the rows expand it as retail does) | rows 0x00C75000, 0x00C75400, 0x00C75470, 0x00C75440 |
| engine/CFloatDiffuser.h | FABLE_RETAIL_CFLOATDIFFUSER_CTOR | out-of-line ctor declaration `CFloatDiffuser();` (native 0x00C75410) | rows 0x00C75410, 0x00C6A650 |
| engine/CFloatDiffuser.h | FABLE_RETAIL_CFLOATDIFFUSER_RELEASE | `void Release();` (native 0x00C75000) | rows 0x00C75000, 0x00C6A3D0, 0x00C6A780, 0x00C6A8B0 |

NF107 / NF108 overlays (read before adding): neither touches these headers (their hdr/engine holds CTC* / CThing* / CQuestManager /
CWorld / NGraphicAppearance headers only).

Landed includers (grep -rlE '<header>' rebuild/src/compiled); none of them defines any of the three new macros, so the opt-ins change
none of them:
- engine/CFloatFilterParams.h: 0x00C89750, 0x00C891A0, 0x00C88E60, 0x00C89350, 0x00C89B60, 0x00C74BB0, 0x00C74AB0
- engine/CFloatLongReverb.h: 0x00C75670, 0x00C75820
- engine/CFloatLongReverbModule.h: 0x00C891A0, 0x00C89350, 0x00C89110
- engine/CFloatAllpassSection.h: no direct landed includer (reached through CFloatDiffuser.h)
- engine/CFloatDiffuser.h: 0x00C75010, 0x00C75140 directly, and the 8 landed includers of engine/CFloatReverbDspEngine.h through it
  (none defines FABLE_RETAIL_CFLOATDIFFUSER_CTOR / _RELEASE or FABLE_RETAIL_CFLOATALLPASSSECTION_CTOR / _RELEASE: grep of rebuild/src/compiled)

The other opt-ins the rows use (FABLE_RETAIL_PASS22_FLOATREVERB_CREATE, FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23,
FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS, FABLE_RETAIL_CFLOATPITCH_PROCESS, FABLE_RETAIL_CFLOATLONGREVERB_PROCESS,
FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER_FADING, FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS(_FADING)) already exist in
rebuild/include (NF105's landing). An unused CFloatPitch.h copy from the first run (identical to the repo) was removed.
