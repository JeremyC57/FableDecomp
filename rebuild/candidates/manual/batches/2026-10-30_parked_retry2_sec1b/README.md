# Parked-row retry sweep, wave 2 section 1b (2026-10-30/31): 20 of 244 unopened section-1 rows landed, 57 improved

Section 1b = the 244 rows section 1's agent never opened (retail 0x00405220-0x00646F67). Two container restarts killed the
agent; the resumed agent finished every row (244 of 244 attempted, 1-5 graded edits each). Landed 20: 19 plain C++ + 1
`retail_asm` (0x00646010 CStatBar text draw: the engine's inline floor helper `fld/fsub/fistp dword` expanded in place at
0x00646044, bytes verified, same block as GFFloatToLongFloor 0x0048D940 and the landed 0x006A68F0). All PASS, no
RELOC_IDENTITY; 5 tests had to be rewritten for VC7.1 (post-C++98 syntax; `T()` does not zero members under VC7.1). 57 improved
sources in `../../pending/2026-10-30_parked_retry2_sec1b/differ/` (5 are retail_asm-classified: ROUND/CeilToLong helpers;
0x005430F0, 0x00428494, 0x004184BD now have partial bodies where there were empty stubs), 4 header-moved in `hdr/`. 167 held with
residues in STATUS.tsv (callee-saved register permutations, frame-slot order, one-instruction scheduling, a hoisted hidden-result push).
Levers (matches): same-TU `__declspec(noinline)` stand-in body for a template callee (`_M_insert_overflow`, map `operator[]`) 5;
local formulation (named/reference locals, shared variables) 5; TU pragmas (`optimize("s",on)` without `("y",off)` gives the biased
frame) 3; over-span bounds fix 2; model rebuilt from retail bytes / PDB 2; shared engine/CCountedPointer.h 2; STLport 4.6 verbatim 1.
BOUNDS_FIX.tsv: 4 rows (0x005E9840 533→180, 0x0055A5D0 697→82, 0x0054EAC0 395→243 ending at a tail-call jmp, 0x0057F70F 54→36).
NAME_FIXES.tsv: 16 rows (incl. CFontBank::GetStringWidth 0x0048DA62, GFACoordToVCoord 0x009E21D0, the static
CTCInventoryItem::GetInventoryCategory 0x005D99D0, CAnimationEntry::TransferBinaryIn) + 6 review rows in the queue (GetSaveDir
0x0041A600 vs 0x0041A540; the `unsigned long` count parameter vs Ego_r's JJ_N; 0x005D99D0 vs 0x005D97B0; stand-in body policy; callee
labels on 0x004AFBB0 / 0x005786B5; 0x00646010's two RELOC_NAME callees).
Review notes: 0x005C7452 and 0x0055D180 (differ) use local CDefPointer / CDiskFileWin32 models (the PDB 0x1c-byte class over
CBaseClassNonCopyable with an inline dtor is not expressible in the generated flat header); descriptive names in differ files
(0x005C7452 CWeaponAugmentationSlots / GetDamage 0x007661C0 with a def-index 2nd parameter; 0x004184BD console init names;
0x00641C30 LiftedByOne; TextColourFlag in 0x00646010); the CDefPointer ENullDef shim is still needed.
Coordinator: vland --land 20/20 (one RELOC_CONFLICT warning: GetSaveDir, queued), bindings_multi_check 0.
