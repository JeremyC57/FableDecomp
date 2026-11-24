# 0x00DF5000 NScript::CQ_TentacleKrakenBossFightScript::PhaseOne -- B24S2 (2026-11-24)

## Lessons that apply
- Port: no repo header for the script class; hdr/engine/CQ_TentacleKrakenBossFightScript.h is hand853's header copied whole (new file,
  hdr/README.md). The parked draft hung cl on today's tree (missing include).
- FableWin first (0x038AA1B0): TRetVoid exits, for-scope openers on both for loops, long loop index, PDB local names (ESCALATE.md).
- c2 inline trace kit (inl/, copied from b24a/00d11930/inl): read c1 / f / sc per site before trying spellings.
- Landed precedent for `FABLE_FORCEINLINE CScriptThing::operator=`: 26 quest-script rows; sibling 0x00DEFB10 (same doors loop) uses
  plain inline assign + STLPORT_HTTP mode with plain `for` and plain returns.

## State (B24S2, 12 grades + 9 traces + 2 probes): best DIFFER(3007v3071) = differ/00df5000.cpp (c1/v9.cpp)
- v1/v2 (port, for-scope, no visible dtor) 2665: operator= x4 + push_back called, first-exit ~CScriptThing called.
- v3 (no for-scope) 3007: one operator= called, ~map expanded (retail calls 0x00CDBFB0), first exit called.
- v4 (for-scope + visible ~CScriptThing) 2745: ONLY the 4 operator= + push_back differ (all called).
- v6 RELEASEDATA, v7/v12 STLport HTTP mode, v8 implicit assign, v11 FINISH_LOCAL push_back: no cost change (traces).
- v9 = v4 + FABLE_FORCEINLINE operator= (landed precedent): 3007, only push_back called. v10 = v9 + RELEASEDATA: same.
- Probe p2 (v9 + push_back forced via probe_hdr, diagnostic only): RELOCATION_MATCH. ESCALATE.md has the numbers and the question.
- Best next idea: find why retail's operator= / push_back are cheaper than ~CScriptThing in c2 (header text of CCountedPointer /
  _Construct / CScriptThing copy that retail's register_scripts TU saw); else owner decision on a push_back force-inline opt-in.
- p1/p2 'RELOCATION_MATCH' is fg.py only, not raw-compared.
