# 0x006E3B90 (lip-sync playback update) -- ESCALATE (AUTH30, 2026-11-27)
- c0 (Codex draft with its four add-only overlay opt-ins: CAnimSet.h, CAnimateThing.h, CTCSoundPlayer.h, CTCTalk.h under
  FABLE_RETAIL_TALK_UPDATE_PASS29): DIFFER(828v816), reproduced. 42 non-relocation instruction lines differ.
- Residue: register allocation and layout in the vector-reload / fade loop (retail reloads `esi` from the stack and uses `sub ecx,esi;
  imul ecx`, ours keeps `edx`; retail pads with `lea esp,[esp]`, ours `mov edi,edi`; ours has one more `xor esi,esi`). Residue stated
  only; no cause proven at one grade.
- Not authored further: Codex's PROBES.md has 497 `- ` probe lines and I had no hypothesis outside them. Grades: c0 (1), the reproduction
  only, far below the brief's 25.
