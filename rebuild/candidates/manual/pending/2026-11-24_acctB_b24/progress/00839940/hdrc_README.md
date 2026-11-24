# hdrc overlay (B24B5, row 0x00839940) -- diagnostic overlay, NOT yet in landing form
Full copies of the current repo headers with these changes (redo each as an additive `#ifdef FABLE_RETAIL_*` section on the
then-current repo header before any hand-back):
- engine/CThing.h:
  * HasTC(ETCInterfaceType) forceinline guard gains `&& !defined(FABLE_DIAG_ONHIT_PLAIN_HASTC)` (diagnostic only; not used by c21).
  * the ONHIT HasTC<T> template is skipped when FABLE_RETAIL_CTHING_HASTC_CLASSTYPE is defined, and a copy of the CLASSTYPE body
    (proven by retail's out-of-line HasTC<CTCActionUseActivateQuest> 0x006AE680) is added for ONHIT && CLASSTYPE && !GETTC_EXPANDED.
    Landing form: a new opt-in name (e.g. FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT_HASTC_CLASSTYPE) instead of editing the ONHIT guard.
  * FABLE_RETAIL_CTHING_ISDEAD_BITFIELD: `bool IsDead() const { return Dead; }` (FableWin ?IsDead@CThing@@QBE_NXZ 10i/22b SAME by
    fwsize; the default `Dead!=0` is 14i/32b). Additive.
- engine/CCreatureDef.h: FABLE_RETAIL_CCREATUREDEF_STATGETTYPENAME (non-TRANSFER branch, when DIALOGUEVOICES is off): the inline
  StatGetTypeName the DIALOGUEVOICES branch already has (GetSubDef<CCreatureDef> 0x00485815 builds "CCreatureDef"). Additive.
- engine/CHeroExperienceDef.h: FABLE_RETAIL_CHEROEXPERIENCEDEF_STATGETTYPENAME: "CHeroExperienceDef" (GetSubDef<CHeroExperienceDef>
  Ego_r 0x0058AB00 / retail 0x006DAA90). Additive. (Ego_r/FableWin name ?StatGetTypeName@CHeroExperienceDef@@SI?AVCCharString@@XZ
  still to be confirmed in the name lists.)
- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_FRIEND_THINGGAMEOBJECT (c24 only, no byte effect). Additive.
- stlport/fable_containers.h: FABLE_DIAG_BITSET_A/B/C test() bodies (diagnostic only; not used by c21).
Pick-any checker not yet run (row not matched).
