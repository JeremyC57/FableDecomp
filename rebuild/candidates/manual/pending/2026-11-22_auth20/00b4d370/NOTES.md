# 0x00B4D370 NOTES (AUTH20; names and evidence as AUTH19's NOTES.md, unchanged)
- Leaf `?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z` (Ego_r.pdb 0x009F0370; FableWin S_GPROC32 mod 972). Every local
  name is from Ego_d / FableWin S_BPREL32 (AUTH19 NOTES.md). No descriptive names.
- AUTH20 text changes in p9, each matched to FableWin's /Od line size: 408..412 weights order, 546 `1.0f + perspective_shift`,
  532 `Y * max / fabs(X)`, and C2DVector::GetMagnitude default text (FW lists no `squared` local).
- Status: ESCALATED (ESCALATE.md). Best: p9 `best_evidence.cpp` DIFFER(7162v7176) nm2 147; `best.cpp` p5 DIFFER(7176v7176) nm2 110.
- Header patches: AUTH19's three (`hdr_GFLimit.patch`, `hdr_CEngineShadowRenderer.patch`, `hdr_CEngineLightingManager.patch`), unchanged.
  The GFLimit includer restriction is in ESCALATE.md.
- Tools: g.sh (QT=_a20 for a private work dir), od.sh / odl.sh (VC10 /Od line sizes), slots.py / slotmap.py (frame slot use), nm2.py, lc.py.
