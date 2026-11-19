# NF99P2 header overlay (opt-ins only; default path token-identical)

Seven headers, each a copy of the CURRENT repo header (2026-11-19; base copies in `orig/engine/`, identical to the repo files when
this round started). Five carry NF99's opt-ins (held with its five rows); NF99P2 added two headers and one opt-in to a third.
The CSalSwBuffer.h / CSalSwFilter.h sections of NF99's README are gone: those headers landed in nf99a and are not in this overlay.

## NF99P2 additions
- engine/CFloatReverbEngine.h (new in this overlay): `FABLE_RETAIL_CFLOATREVERBENGINE_COMMIT`, public
  `void Commit(class CHLRvbParams *,bool);` right after `Create` (SDK ?Commit@CFloatReverbEngine@@QAEXPAVCHLRvbParams@@_N@Z, native
  0x00C6AEF0). Users: 0x00C6AEF0 (owner), 0x00C54510, 0x00C54190.
- engine/CReverbConstants.h (new in this overlay): `FABLE_RETAIL_CREVERBCONSTANTS_INLINE_CTOR` replaces the out-of-line
  `virtual ~CReverbConstants();` with the SDK header-inline constructor and virtual destructor (`#ifdef ... #else` the original
  declaration `#endif`, so the default path is unchanged). Source: CODAApi.lib carries ??0CReverbConstants@@QAE@XZ and
  ??1CReverbConstants@@UAE@XZ as COMDATs in five objects; the release ctor stores vptr, 48000 at +8, 2 at +4, zeros at +0xc..+0x18
  in that order; the dtor stores the vptr only. User: 0x00C54510 (its format local).
- engine/CFloatReverbParameterTranslator.h: `FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS`, private static
  `PanToGains(const struct _RVB_VECTOR *,float *,unsigned int,unsigned int)` after NF99's TRANSLATE block (SDK
  ?PanToGains@CFloatReverbParameterTranslator@@CAXPBU_RVB_VECTOR@@PAMII@Z, native 0x00C77D90). Users: 0x00C77D90, 0x00C783B0, 0x00C783F0.

## NF99 opt-ins carried (unchanged; see NF99's records)
- engine/CFloatReverbParameterTranslator.h `FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE` (nine private Update* declarations).
  Users now: 0x00C783F0, 0x00C783B0, 0x00C77C00, 0x00C77FC0, 0x00C78040, 0x00C77AE0, 0x00C78150.
- engine/CFloatFilterParams.h `..._SET_HIGHPASS` / `..._SET_LPBUTTERWORTH2`. Users: 0x00C783F0, 0x00C74BB0, 0x00C74AB0.
- engine/CFloatShortReverb.h (`..._PROCESS`, `..._PROCESSFADE`), engine/CFloatShortReverbModule.h (`..._PROCESSINPUT`),
  engine/CFloatReverbDspEngine.h (`..._PROCESSREVERB`): used only by the held drafts in work/held_drafts/ (the blocked family);
  no row in rows/ defines them.

## Checks
- pin_asserts.py on all seven: `total pinned 0` each (none carries a FABLE_STATIC_ASSERT; the size checks are typedef arrays, untouched).
- Default path: `cpp -P` of each overlay header against the repo header, with no macro, with FABLE_RETAIL_PASS22_FLOATREVERB_CREATE,
  and with that plus the landed opt-ins (CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES, CREVERBFNSSSE_APPLYFILTERORDER2_QUAD,
  CFLOATFILTER_APPLY_PASS23): md5-identical in all 21 cases.
- Landed includers (direct: 0x00C6B050 CFloatReverbEngine.h, 0x00C77800 CFloatReverbParameterTranslator.h, 0x00C88E60
  CFloatFilterParams.h; transitive through CFloatReverbConstants.h / CFloatReverbParameters.h / CFloatFilter.h: 0x00C74510,
  0x00C6B050, 0x00C77800): none defines an NF99 / NF99P2 macro (`grep` of rebuild/src, rebuild/include, rebuild/tests finds none),
  so the opt-ins change none of them.
- The other agents' overlays (agentNF100: NInventory_CTCInventoryBase.h; agentNF101: CTCWeapon.h,
  CCreatureAction_HeroLoadRangedWeapon.h) touch none of these headers.
- Every FABLE_RETAIL_* macro defined in rows/ resolves in rebuild/include or this hdr/ (checked before hand-back).

## Inferred bodies
- CReverbConstants inline ctor / dtor: read from the CODAApi.lib release COMDATs (store order and values), not inferred.
- NF99's MatchToken / GetFilterHistory inlines (held-draft users only) remain Inferred as NF99 recorded.
