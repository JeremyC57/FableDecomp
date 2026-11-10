# 2026-11-10 landing u -- Codex pass 16 return (37 of 40 rows)

Source: `pending/2026-11-10_codex_pass16/return_2026-11-10/` (START_HERE_CLAUDE.md, STATUS / LABELS / NAME_FIXES, the three parked
sources with PROBES). vland --land: 1 MATCH (0x009E1960 GFStringToInt) + 36 RELOCATION_MATCH, tests PASS.

- Headers: Codex's 115-file overlay merged 3-way onto HEAD (base = the package's include/): 58 applied cleanly, 56 new, 1
  (stlport/fable_containers.h) auto-merged with agentNF's FABLE_RETAIL_STLP_LIST_REMOVE_DECL; hdrcheck2 default=same for all 59 changed files.
- Line 1 of each source normalised from Codex's `// <addr> Name: text` to `// Name -- text`; module names fixed for 0x006E8CE0
  ((anonymous namespace)), 0x0080DB70 operator(), 0x00826C90 CReplaceSerialise<...>, 0x00D259A0 NScript::CQ_DragonBossFightScript::CDragon.
- Landing fix: the payload first carried `flags` as a string, which vland splits into characters; five flagged rows failed and four
  landed with garbage CompilerFlags (`/ G r`). Re-landed / rechecked with list flags (0x008884D0, 0x00A22F80 /d2inlS279, 0x00B396E0
  /d2inlS100, 0x00B92300 /d2inlS92, 0x00BEEA00; rechecked 0x0065B142, 0x00807A90, 0x008F5970, 0x00AC7200) and the ps1 entries corrected.

Parked (differ/ + PROBES): 0x009397E0 CAIStateGroup_SitRoundCampfire::OnReplace 569/571 (one pre-call seed store), 0x00C11520
CSoundEventManager::SetEventPosition (one store/push order), 0x00C6B9D0 CSalSwFilter::ProcessEQ (bare dword x87 conversion -- the
GFFloatToLongNear helper route was not tried). Queued `land1011v`: 101 Codex name fixes.
