# NF20 header opt-ins

Full copies of the current repo headers (`rebuild/include/engine/...`), changed only by `#ifdef` additions.
Without the macros, each header is byte-identical in effect to the repo copy. Grade with `FABLE_HDR_OVERLAY` pointing here.

| Header | Macro | Adds | Evidence | Rows that define it |
|---|---|---|---|---|
| engine/NFractal_CNoise.h | FABLE_RETAIL_NFRACTAL_CNOISE_CTOR | public `CNoise();` (0x00F36BA0) | Ego_r.pdb + FableWin.pdb ??0CNoise@NFractal@@QAE@XZ; body stores Seed=0 immediate, plain `ret` | 00f36ba0 |
| engine/CSurface.h (inside FABLE_RETAIL_CSURFACE_BITMAP) | FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE | GetWidth / GetHeight declared out of line (0x00415BD0 / 0x00415BF0) | Ego_r.pdb 0x00414F70/0x00414F90 + FableWin.pdb ?GetWidth/?GetHeight@CSurface@@QBEKXZ | 00415bd0, 00415bf0 |
| engine/CSurface.h | FABLE_RETAIL_CSURFACE_GETWIDTH_FORCEINLINE | the inline GetWidth becomes FABLE_FORCEINLINE (per-TU) | retail 0x004167DA expands GetWidth in place but calls GetHeight 0x00415BF0 | 004167da |
| engine/CSurface.h | FABLE_RETAIL_CSURFACE_GETHEIGHT_OUTOFLINE | GetHeight declared out of line only (per-TU) | same; call to 0x00415BF0 | 004167da |
| engine/CSurface.h (after the CLEAR block) | FABLE_RETAIL_CSURFACE_SWIZZLE | `void Swizzle(unsigned long)` 0x009F4330, `void Unswizzle(unsigned long)` 0x009F4360 (needs FABLE_RETAIL_CSURFACE_CLEAR) | Ego_r.pdb 0x00B48030/0x00B48060 (same order as retail) + FableWin.pdb | 009f4330, 009f4360 |
| engine/CTCActionUseBase.h | FABLE_RETAIL_CTCACTIONUSEBASE_DTOR | `CTCActionUseBase(CThing&)` and `virtual ~CTCActionUseBase()` (slot 0, 0x004DA967); declarations only, no layout change | Ego_r.pdb ??0CTCActionUseBase@@QAE@AAVCThing@@@Z + FableWin.pdb ??1CTCActionUseBase@@UAE@XZ | the 34 ??_G rows 004dbfaf..004dcf8b and 004dcd88 |

Rows also use existing repo opt-ins: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL (stlport/fable_containers.h),
FABLE_RETAIL_C2DVECTOR / _NORMALISE, FABLE_RETAIL_CSURFACE_BITMAP / _CLEAR, FABLE_RETAIL_DEF_CLASS_BASE, and the CTCBase.h retail branch.
