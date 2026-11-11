# NF13 header overlay

Full copies of the current repo headers (`rebuild/include/engine/`) with `#ifdef` opt-in additions only: `diff` against the repo
copy removes 0 lines for every file, and every row that does not define the new macro compiles exactly as before. Graded with
`FABLE_HDR_OVERLAY=$S/w22/agentNF13/hdr`.

| Header | Opt-in | Adds | Evidence | Used by |
|---|---|---|---|---|
| CTCEntityEvents.h (flattened `#else` branch) | FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT (same macro the retail branch already uses) | `void AddEvent(NEntityEvents::CEventBase*)` 0x008113C0 and the `namespace NEntityEvents { class CEventBase; }` forward declaration, on the flattened layout | Ego_r.pdb / FableWin.pdb `?AddEvent@CTCEntityEvents@@QAEXPAVCEventBase@NEntityEvents@@@Z` (both name sets); the retail branch declares the same | 0x008957C0, 0x00859D80 |
| CTCHeroAttachableAppearanceModifiers.h (inside `_COVERS`) | FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_UPLOADCHANGES (wraps the existing EditFrameUpdate line in its `#else`) | `void UploadChangesToGraphics()` 0x00707930 in place of the `EditFrameUpdate` declaration | Ego_r.pdb `?UploadChangesToGraphics@CTCHeroAttachableAppearanceModifiers@@QAEXXZ` (public, non-virtual: the four direct `call 00707930h` sites 0x005B3AD5 / 0x005B3E25 / 0x005B3F81 / 0x005CDA36), Ego_r donor order ... UpdateGraphics 0x0055C3E0, UploadChangesToGraphics 0x0055C710, Copy ...; the landed self row 0x00707930 is already UploadChangesToGraphics (body: clear the Changed bit, UpdateHiddenModifiers / UpdateHiddenMaterials / UpdateGraphics, SetAsChanged). EditFrameUpdate is the CTCBase editor virtual (UAEXXZ, donor 0x0055C9F0 after FrameUpdate), not this address | 0x005B3E4E |
| CMusicManager.h (CState) | FABLE_RETAIL_CMUSICMANAGER_GETACTIVESET_OUTOFLINE (wraps the inline body in its `#else`) | `long GetActiveSet() const;` declaration only, so the row can define the out-of-line COMDAT | FableWin.pdb `?GetActiveSet@CState@CMusicManager@@QBEJXZ`; retail keeps it out of line at 0x006BD760 (InternalUpdate 0x006BD9B0 calls it; landed binding) | 0x006BD760 (renamed row) |

No new headers. CSoundInit.h is used as landed (FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST, agentNF12) by 0x00855430,
0x008D8CE0, 0x0093C600 and 0x008AF850: all four inline-ctor store orders close with that spelling.
