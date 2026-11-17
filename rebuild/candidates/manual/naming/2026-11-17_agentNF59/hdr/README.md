# NF59 header overlays (2026-11-17, land1117fu follow-up slice)

Each file here is a copy of the repo header at the same path under `rebuild/include/`. NF59 added `#ifdef` opt-in blocks only.
With none of the opt-ins defined, every overlay is token-identical to the repo header: comments stripped, opt-in blocks removed,
and each `#else` part kept (`work/tokcheck.py`). `diff` against the repo shows no removed lines. Where an opt-in replaces an
existing inline body, the original body moved into the opt-in's `#else` branch unchanged.

The table lists 13 opt-ins in 8 headers. No `stlport/` header is overlaid (NF58's lane).

| Header | Opt-in macro | Rows that define it | What it adds / evidence |
|---|---|---|---|
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_ISCONFISCATABLE` | 0x005D9790 | Declares the static `__fastcall` `IsConfiscatable(long)`. Ego_r.pdb 0x0055A6F0 / FableWin.pdb 0x01E02690 `?IsConfiscatable@CTCInventoryItem@@SI_NJ@Z`. |
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_GETPDEF_STATIC` | 0x005D97D0 | Declares the static `__fastcall` `GetPDef(long)`, which returns `CDefPointer<const CInventoryItemDef>` by hidden pointer. Ego_r 0x0055A730 / FableWin 0x01E02950. Needs engine/CDefPointer.h. |
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_GETITEMTOSELECTUPONREMOVAL` | 0x005D99B0 | Declares the static `__fastcall` `GetItemToSelectUponRemoval(long)`. Ego_r 0x0055A980 / FableWin 0x01E034C0. |
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_GETMAXNUMBEROFITEMS` | 0x005D99F0 | Declares the static `__fastcall` `GetMaxNumberOfItems(long)`. Ego_r 0x0055A9C0 / FableWin 0x01E03780. |
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_GETTUTORIALCATEGORY_STATIC` | 0x005D9B70 | Declares the static `__fastcall` `GetTutorialCategory(long)`. Ego_r 0x0055AB40 / FableWin 0x01E03E00. |
| libpk/CCodaFrameStreamer.h | `FABLE_RETAIL_CLOCKEDRINGBUFFER_GETHEADINDEX` | 0x00C4A0E0 | Declares `long CLockedRingBuffer::GetHeadIndex()` (0x00C49DD0). The name is descriptive: it is the spelling the landed self row 0x00C49DD0 defines. |
| engine/CTCTalk.h | `FABLE_RETAIL_CTCTALK_GETDIALOGUEVOICE` | 0x005EB3A0 | Adds the inline `CCharString GetDialogueVoice() const { return DialogueVoice; }`. FableWin.pdb 0x01C9F740 only (inline in the release build). No layout change. |
| engine/CThingFilter_OverlappingSphere.h | `FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE` | 0x006689E0 | Declares `operator()(const CThing&) const` only. The repo's inline body moved into `#else`. Ego_r.pdb 0x005451C0 has it out of line, and retail does too (0x006689E0). |
| engine/CTCPhysicsBase.h | `FABLE_RETAIL_CTCPHYSICSBASE_FLAT_GETBOUNDINGINFO` | 0x008E60D0 | Declares `GetBoundingInfo(C3DVector&, float&) const` on the flat layout branch. Ego_r.pdb 0x006EC5B0; out of line at 0x006AF640. |
| engine/CAnimateThing.h | `FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED` | 0x0070C0E0, 0x00661720, 0x00665860 | Declares the private const `IsAnimationFinished()` (Ego_r 0x005752F0 / FableWin 0x01E96330). Makes `IsMainAnimFinished()` its inline public wrapper (FableWin 0x01D92820); the original declaration is kept in `#else`. |
| engine/CAnimateThing.h | `FABLE_RETAIL_CANIMATETHING_ISFINISHED` | 0x00661720 | Declares `bool IsFinished() const` (0x00661720). The name is descriptive (the landed catalog's); no PDB names it. Needs `_ISANIMATIONFINISHED`. |
| engine/CTCPerceiveThing.h | `FABLE_RETAIL_CTCPERCEIVETHING_GETFOV_INLINE` | 0x00745120 | Adds the inline body of `GetFOV() const` (`PDef->FieldOfView / 360.0f`; Ego_r 0x00757070 / FableWin 0x0258DA60) where retail expands it. The landed row used to define the symbol itself, a second strong definition. As a header inline it is emitted as a COMDAT and no longer defines a second strong symbol next to the out-of-line copy 0x007445D0. This is inline-function semantics; it was not link-tested. |
| engine/CFaction.h | `FABLE_RETAIL_CFACTION_CTOR_OUTOFLINE` | 0x006BAED0 | Declares `CFaction()` only (0x006BA420). The repo's inline `CFaction() {}` stays in `#else`. Ego_r.pdb 0x006F64E0 `??0CFaction@@QAE@XZ` is out of line, and retail calls it from `GetDefaultVal<CFaction>`. |

Rows using an opt-in define its macro at the top of the row. Every opt-in is member-specific, named `FABLE_RETAIL_<CLASS>_<MEMBER>`.
