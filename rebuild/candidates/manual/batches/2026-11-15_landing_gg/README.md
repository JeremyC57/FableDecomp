# 2026-11-15 landing gg -- 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw (2,223 bytes; agentP6F, hand-authored)

RELOCATION_MATCH, behaviour test PASS, `/Gr`, strict headers, no header change. Codex pass 22 parked it at DIFFER(2217v2223): for the
border height, ours called GetGraphicMaxHeight (vtable +0x60) before GetRenderTargetDimensions (0x009BEDC0); retail calls the dimensions
first and keeps the returned pointer (`imul eax,[ecx+4]`, 0x0064C69F..0x0064C6B7). Swaps, casts and parentheses are canonicalised by c1,
and a const-ref binding of the dimensions costs a frame slot.

Fix (hand rewrite 5 of 6 of the border block, owner: "try hand authoring as well as your usual tools"): the left border's graphic index
is named once by reference, `const long& border_graphic = gui_def.TargetingBorderLeftGraphic;`, and both bank queries take it. VC7.1
then evaluates the dimensions call first for the `.Y` product, as retail does. `border_graphic` is a descriptive local name (no PDB
local is recorded for it).

Warning for audit: RELOC_CONFLICT `CTCGraphicAppearance::ClearMeshEffect` -> 0x004BFFC0 here, 0x004BFE20 in landed 0x004BFF60.
0x004BFFC0 is a 5-byte `jmp 0x004BFE20` stub in front of ClearMeshEffect; reloc_bindings already holds three spellings for the two
addresses (earlier landings). Which callers reach the stub, and what the stub's own name is, is an open naming item.
