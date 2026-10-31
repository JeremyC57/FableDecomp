# Chunk 610 (eighth parked-row chunk from the sweep ranking, 10 same-length rows 275-288 bytes): 3 of 10 landed

Landed (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0): 0x00C071B0 LZO 1.08 swd_accept (the vendor `while (n--)` loop
pasted exactly from lzo_swd.ch:420; the parked `if (n) do..while(--n)` form moved n into esi), 0x0062B7C0
CWorld::EAControlledCreatureSheatheWeapon (nine callee spellings corrected to the PDB; `player = GetPlayer(event->Player)` reassigned
as the landed sibling 0x00629C40 does) and 0x00A10A60 WordWrap_String::TokenizeW (shape of the landed sibling WordWrap_TokenizeW
0x00A10720: inlined WordWrap_CanBreakLineAt plus a `for (remaining = wcslen(); remaining > 0; ...)` loop; /Zc:wchar_t). Parked 7
(pending/.../differ/): 00637c80 DebugDraw 38 -> 8 lines (by-value operator- on the area TL position; TLX/TLY loaded before pos.Y),
00733cf0 now 288v288 (one dead frame word, ebx/ebp swap), 0080db70 x87 operand choice, 004b1a80 this/list-head swap + zero stores
before the pushes, 00934f20 final return turned into a select, 005b6767 g_pMainGameComponent load scheduled late, 00ab7b00 VC7.1
keeps the line width on the x87 stack where retail keeps maxHeight. 24 NAME_FIXES + 5 review rows queued (prefix c610; five catalog
labels wrong incl. 0x00487DC0 CPlayer::GetPControlledCreature and 0x0048C950 ~CCreatureAction_SheatheItemToInventory). Lesson: look
for a landed sibling in rebuild/src/compiled before tuning -- it won two rows written off as register-colouring gaps.
