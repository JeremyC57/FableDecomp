# 0x00666D40 CThingCreatureBase::GetHeightZBelowAt: bracket (p25R, 2026-11-21)
bracket_p25R_r1.cpp + hdr66_overlay (two add-only ctor-body opt-ins, CTHINGFILTER_SOLIDTOTHING_CTOR_BODY /
CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_CTOR_BODY, from Ego_d's ctor line tables) reach RELOCATION_MATCH. The from/to block
(`C3DVector from = pos; from.Z += radius; C3DVector to = from; from.Z += extra_size_area; to.Z -= 50.0f;`) closes the x87 dup and the
frame order, but Ego_d names a to_centre local (locals from, to_centre, to, get_height_below_func) and lines 2782..2786 measure
27/16/12/12/12 B against this text's 12/9/12/9/12. Every to_centre form p25R tried breaks the bytes (best 346; 42-order sweep, ~100
variants). Not Inferred (rule: Ego_d-contradicted text); the row stays reserved. Next checks: where to_centre is read (full-function
/Od-vs-Ego_d line map); a to_centre release never reads; its type/frame offset from Ego_d locals (16 B suggests a by-value-returning
call built into a local); independent revision evidence (FableWin v Ego_d length, Ego_r lines/locals).
