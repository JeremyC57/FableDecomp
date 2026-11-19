# hand4b header overlay (copies of the CURRENT repo headers + new opt-ins only)

Batch-4 opt-ins (C3DANIMATIONSTATS_TIMINGEVENTS, C3DANIMATION2_INSTANCELIST, CSIMPLEMEMORYPOOL_RESET) were merged with 86163b82c1 and
removed from here. Batch 6-8 opt-ins (STLP_LIST_CREATE_NODE_NOINLINE, NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO,
CTCSPECIALABILITIES_ISABLETOUSEANYABILITY, CTHINGAICREATURE_SUSPENDERS, CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD) were merged and removed too. Each file below is a copy of the repo header with one NEW opt-in; no default path changes.

| file | new opt-in | used by | what it adds |
|---|---|---|---|
| engine/CGameCameraManager.h | `FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND_WORLD` (nested in `..._LOOKAROUND` + `..._GETOBSERVEDTHING`) | 0x0084E590 (batch 9) | public inline `const CWorld& GetWorld() const { return World; }` in the LOOKAROUND branch (the `..._WORLD` getter exists only in the default branch, whose class has no GetObservedThing). FableWin.pdb ?GetWorld@CGameCameraManager@@QBEABVCWorld@@XZ (debug 0x021F5FF0); inline in retail 0x0084E62D..0x0084E643 (`mov eax,[ebp+40h] ; mov ecx,[eax+74h] ; mov ecx,[ecx+20h]`). No layout change. |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | `FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATICCAMERAFOCALPOS` (nested in `..._CAMERAFOCALPOS`) | 0x0084E590 (batch 9) | inline `C3DVector GetStaticCameraFocalPos() const { return GetCameraFocalPos(1.0f); }`: Ego_d.pdb / FableWin.pdb ?GetStaticCameraFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ (Ego_d debug 34 bytes; FableWin 0x017DB16C, called by the row donor 0x02882FD0), absent from Ego_r.pdb (inline in release). Retail then computes the focal position before pushing AddPoint's weight (0x0084E69F..0x0084E6B1). Same body as the UPDATE_LIST branch. Default unchanged. |
