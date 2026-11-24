# hdrf overlay (B24B6, row 0x00839940) -- landing form
Full copies of the CURRENT repo headers (2026-11-24) with additive opt-ins only (default branches unchanged):
- engine/CThing.h:
  * FABLE_RETAIL_CTHING_ISDEAD_BITFIELD: `bool IsDead() const { return Dead; }` (FableWin.pdb ?IsDead@CThing@@QBE_NXZ 10i/22b = this
    text at /Od by fwsize; default `Dead!=0` is 14i/32b). Same code.
  * FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT_HASTC_CLASSTYPE: HasTC<T> = the FABLE_RETAIL_CTHING_HASTC_CLASSTYPE body (retail out-of-line
    HasTC<CTCActionUseActivateQuest> 0x006AE680; Ego_r/FableWin ??$HasTC@V..@@@CThing@@QBE_NXZ) for the ONHIT configuration (which does
    not use GETTC_EXPANDED). Replaces B24B5's edit of the existing ONHIT guard; the plain ONHIT branch is untouched (`#elif`).
- engine/CCreatureDef.h: FABLE_RETAIL_CCREATUREDEF_STATGETTYPENAME (non-DIALOGUEVOICES): the inline StatGetTypeName the DIALOGUEVOICES
  branch already has ("CCreatureDef", built in place by GetSubDef<CCreatureDef> 0x00485815; FableWin.pdb ?StatGetTypeName@CCreatureDef@@SI?AVCCharString@@XZ).
- engine/CHeroExperienceDef.h: FABLE_RETAIL_CHEROEXPERIENCEDEF_STATGETTYPENAME ("CHeroExperienceDef", GetSubDef<CHeroExperienceDef>
  retail 0x006DAA90 / Ego_r 0x0058AB00; FableWin.pdb ?StatGetTypeName@CHeroExperienceDef@@SI?AVCCharString@@XZ).
No FABLE_DIAG_* blocks. hdrc / hdrd / hdre are diagnostic overlays and are not for hand-back.
Merge note (B24B7): FABLE_RETAIL_CTHING_ISDEAD_BITFIELD is the same `return Dead;` text as FABLE_RETAIL_CTHING_ISDEAD_DIRECT added by row
0x00975BB0 (b24a overlay) -- one opt-in should be kept when both land.
