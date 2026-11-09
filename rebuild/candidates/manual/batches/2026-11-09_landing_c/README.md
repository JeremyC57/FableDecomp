# Landing 2026-11-09c: NUISystem::CManager::Action (35,651 bytes), single-row 886, sessions 1-8 + Codex merge

0x0041E6D3 NUISystem::CManager::Action: RELOCATION_MATCH + PASS (28-check C++98 behaviour test under VC7.1 and g++; a mutated copy
fails), flag /GS. The UI action dispatcher: one switch over ~213 action types (menus, inventory, trade, map, photos, options, debug
cheats) with a 1,032-byte jump table. Hand-back: `pending/2026-11-08_linux_hand886_wip/` (WORKLOG sessions 1-8, TRACE.md, tools).

How it got there (owner's all-means push, 2026-11-09):
- Codex's six fixes merged one grade at a time (frame 0x808 -> 0x800: FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR).
- c2's frame packer reversed and simulated (277/284 slots placed before grading).
- Above 10000 IL tuples every block has weight 1, so register priority is reference count x block candidate count: re-reading
  `CMainGameComponent::Get()` at each site (c2 keeps the global itself in esi) instead of a cached `game` local gives retail's
  entry roles; reads right after a call stay on `game` (the call kills the global's register copy).
- A missing `case UI_ACTION_TYPE_GO_BACK:` found from retail's jump table (behaviour fix: shares SEND_BACK_EVENT's SetEvent).
- Cross-jump tail survivors: c2 orders exit edges as breaks in source order, then returns, then edges into `default:`; the last
  matching edge keeps the tail. Six cases end `return;`, CLOSE_TELEPORT_MENU falls into a mid-switch `default:`.
- Inline budget: scoped `#pragma inline_depth(0)` regions per statement retail calls out of line (line 4 of the source lists them).
Headers: 19 overlay headers merged (additive opt-ins, hdrcheck default and every existing opt-in unchanged; new
NUISystem_CActionParamString.h). 10 SHARED_HEADER_AVAILABLE warnings: local models remain for 10 classes (header debt).
Names: labels `ghidra_out/labels_landing_2026-11-09c.tsv` (40 callee names); queue tag land0809c (the row's 4 NAME_FIXES + two
RELOC_CONFLICTs: 0x0040D2F0 SetCameraBowYZInverted in landed 0059697a, 0x0042A2CA = the CText SetStaticCast instance, to verify).
The third conflict (GetDef<CThingBaseDef> at 0x0042B144 here v 0x0042AF3C in 007cbdc0) is identical-COMDAT folding territory:
0x0042AF3C already carries ten GetDef spellings; left for a GetDef ICF audit.
