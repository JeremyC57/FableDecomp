# Chunk 820 hdr/ hand-back

Both files are fresh copies of rebuild/include with chunk 818's opt-ins applied by patch. Each copy is cmp-identical to
`pending/2026-11-06_linux_hand818/hdr/`. Chunk 820 made no header edit of its own. Before hand-back, each file was diffed
against rebuild/include again with plain `diff`. The only differences are the two added `#ifdef` blocks below. No existing line
was touched or reflowed.

| header | change | evidence | path |
|---|---|---|---|
| engine/NScript_CV_ChickenKickingScript.h | chunk 818's nested `CChickenMaster : CEntityScriptBase` (sizeof 0x20), unchanged | see pending/2026-11-06_linux_hand818/hdr/README.md (Ego_r.pdb layout; vtable 0x012E747C stored by Alloc 0x00E634B0) | opt-in `FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER`; one block appended |
| engine/CScriptGameResourceObjectScriptedThingBase.h | chunk 818's `__forceinline operator=`, unchanged | retail CChickenMaster::Main expands the assignment at 0x00E65052 | opt-in `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN`; one block inside the class |

Only parked differ/00e64fb0.cpp uses the overlay. The three matched rows grade RELOCATION_MATCH with the overlay set, and they do not
use either opt-in. No file under rebuild/src, rebuild/tests or rebuild/include defines either macro (grep), so the default path
cannot change any landed includer. Chunk 818's includer recheck (360 includers, g++ -E identical) still applies because the text is
the same.
