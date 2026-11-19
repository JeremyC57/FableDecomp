# Landing hand4b b9 (2026-11-19): NCameraModes::CBase::UpdateTrackMeleeOpponents (0x0084E590)

vland --land with strict headers gives RELOCATION_MATCH 593v593 + PASS, with no flags. The symbol is ?UpdateTrackMeleeOpponents@CBase@NCameraModes@@QBEXABVC3DVector@@AAV3@@Z (public per the Ego_d
method record; FableWin debug 0x02882FD0). About 76 grades. This is hand4b's last row: all 10 of its rows matched, with none parked or escalated.

Closed by porting FableWin's statement order onto the Ego_d local names, then:
- the visible Ego_d rough-area body (existing CTHINGSEARCHTOOLS_ROUGHAREA_EGOD), which keeps begin/end in registers;
- the donor's GetStaticCameraFocalPos() wrapper;
- default_look_pos declared before focus (`// Inferred:`);
- the running-away test spelled `!(a < b)`, which gives retail's `test ah,41h ; je` (`// Inferred:`).
The dead desired_look_vector is kept with a `// Fidelity:` line.

Same-TU helpers graded at their own addresses from this file: 0x0084F850 and 0x0084F1B0 RELOCATION_MATCH, 0x0084D050 MATCH.

New nested opt-ins, default-off (`hdr_README.md`):
- engine/CGameCameraManager.h FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND_WORLD;
- engine/NGraphicAppearance_CTCGraphicAppearance.h FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATICCAMERAFOCALPOS.

NAME_FIXES: 0x0084F850's catalog label (GetPBestThingInRoughArea<CFilter_Bind3..>) is a byte-twin propagation. The real name is CThingSearchTools::GetAllThingsInRoughArea<const
CThingCreatureBase, vector, CFilter_Bind2<IsCreatureThreatTo,IsAwareOfThing>> (Ego_r 0x008469C0, FableWin 0x02889060). This label causes the vland WARN RELOC_NAME.
