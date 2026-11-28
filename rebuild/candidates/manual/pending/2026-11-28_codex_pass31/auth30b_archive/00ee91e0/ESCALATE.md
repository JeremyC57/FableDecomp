# 0x00EE91E0 NScript::CGlobal_DebugCycleThroughSpeechScript::Main -- ESCALATE (AUTH30, 2026-11-27)
- c0 (Codex draft, six overlay opt-ins: CLevelScriptNoMasterBase.h reference member, stlport/fable_map.h four SPEECH_*26 opt-ins, the NEW
  header engine/CGlobal_DebugCycleThroughSpeechScript.h; the static-assert edit replaced by pin_asserts' FABLE_STATIC_ASSERT_AT pins):
  DIFFER(593v613), reproduced.
- Residue (18 lines): retail duplicates the first early return's cleanup in place (`je` over: destroy `entries` 0x004D0740, destroy
  `targeted_thing` 0x004AA840, pop x4, add esp, ret); ours branches (`jne`) to a shared exit. Plus retail's `lea esp,[esp]` loop pad (absent
  in ours) and one extra `lea ecx; call` destructor call late in ours. (Residue stated only; no cause proven at one grade.)
- Source shape: `bool stop` is a real local (FableWin.pdb S_BPREL32 `stop`, bool, in the block with `seh_target`; `it` in a nested block),
  so the local is evidence-backed. The `if (!stop) if (false) { } else for (...)` wrapper is Codex's spelling; no Ego_d procedure record
  exists for this row (ROW.md), so there is no line data to confirm or refute it.
- Not authored further: Codex's PROBES.md has 1,109 `- ` probe lines over the return / scope spellings. Grades: c0 (1), the reproduction only.
