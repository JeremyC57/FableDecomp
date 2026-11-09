# WORKLOG chunk 883 -- 0x006096E0 CTCInventoryMap::ConstructLogbookListPC

## Identity
?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z (Ego_r donor order; landed GetMenuEntryFromCategoryPC
0x005FAC20 callers are here). Catalog "_global::ConstructLogbookScreen" is descriptive.

## Decoded phases (retail)
1. manager = CManager::GetInstance(); entries = copy of player's CHeroLogBook::LogEntries (map copy 0x00619C50);
   list = CreateComponent("PC_LOGBOOK_CATEGORIES"); frame = list->FindChild("PC_UI_FRAME_LOGBOOK").
2. Four scroll-action groups (parent/frame x LIST/DESCRIPTION): observe/ignore CActions; group 4 runs out of inline budget
   (CAction() 0x42BE50, CCountedPointer<CAction>(T*) 0x42AA29, ~ 0x429B72 out of line).
3. `child = list->FindChild(..); frame->AddPositionChild(child);` x25 (one named local: the nested call form loads frame's vptr
   before the FindChild call); viewport fake children x8.
4. CategoriesList.clear() (out of line tree clear 0x6166E0); count switch over entries (iter++ 0x47B3E0 out of line).
5. Books: GetTC inventory, GetAllItems, IsBook -> books; PC_SUB_LIST; per book: menu entry, PC_MODEL mesh (+ anim), PC_DESCRIPTION
   details, subtitle (+ icon when GetEngineGraphic.BankIndex).
6. Seven category sub-lists (BASICS, OBJECTS, TOWNS, HERO, COMBAT, QUESTS, STORY), CategoriesList[cat] = sub.
7. Tail loop over entries (it++ with _M_increment inline): SetDefaultChild(size-1), one menu entry per log entry.
8. Final loop category = 0..7 in esi (skip 2): find(key copy) in CategoriesList; AddChild sub-list or MakeUnavailable, the
   loop variable (not the address-taken key) passed to GetMenuEntryFromCategoryPC.

## What fixed what (in order)
- STLport `_Construct` in list _M_create_node + alloc_proxy rebind opt-ins: group-4 inline budget exact; /d2inlT71 (smallest; T70 = 591, T71 = T72 = 590): all 590
  calls in retail order.
- Final loop passes `category` (register) not the address-taken `key` to GetMenuEntryFromCategoryPC: size 12832v12832 (p03).
- Position phase as `child = list->FindChild(..); frame->AddPositionChild(child)`: list in edi, whole prologue + 4 action groups
  byte-identical (p04, 303 -> 35 differing segments).
- CEngineGraphic with its two FableWin.pdb ctors (non-aggregate): GetEngineGraphic's hidden result pointer in ecx, result read in
  place (p07).
- `CRightHandedSet orientation = GetStartingRightHandedSet(index); mesh->SetOrientation(orientation);` (p08, 23 segs).
- Visible template bodies of CDefinitionManager::GetDef<T> and CDefClassBase::GetSubDef<T> (header opt-ins, retail logic of the
  landed 0x0042B040 / 0x0042B0A2): def release without the null test / dead store, appearance release `dec [ecx+4]` (p11, p12).

## Current best
p12 + header cleanup = differ/006096e0.cpp, `/d2inlT71` (final2): DIFFER(12832v12832), call sequence identical, 17 differing segments (NOSTACK/NOREG), frame
0x124 vs 0x11C.

## Remaining residue (p12)
- Count prologue: retail `xor ebx,ebx` before the clear call and the 7 counter zero stores through ebx, entries header loaded after;
  ours zero in eax after the call, header (#71, prio -14) loaded first.
- Books loop icon branch: retail subtitleText.Info in ebx, manager from memory; ours manager piece in ebx, Info in memory.
- Books loop end: retail keeps a zero ebx from the description release across the loop exit to `push ebx` (title CreateComponent).
- COMBAT block ring release `cmp [eax],ecx` (retail) vs `cmp [eax],0`; one extra `mov r,r` in the tail loop; alignment pads follow.
- All are const-0 piece / callee-saved piece choices (c2 split pass), plus 8 bytes of frame.

## Probes after p12 (no gain)
p13 counters before clear; p14 separate `it` decl; p15 chained zero init; p16 unnamed sprite cast; p17 orientation in a block
scope; p18 counters before the viewport phase (worse). final/final2 = p12 with experimental opt-ins removed: unchanged.

## Session 2026-11-09 (playbook all-means push)
Regime 0 (5571 IL tuples, c2 gate 0x1077BE49). Method steps 1-5 done; step 6 (natural spelling) not found.

Leaderboard (LEADERBOARD.tsv; fr = frame, ex = exact share, rf = regs+frame-masked share, segs = differing call segments):

| variant | grade | frame | exact | segs | note |
|---|---|---|---|---|---|
| base = differ (p12/final2, /d2inlT71) | DIFFER(12832v12832) | 0x124 | 87.09% | 16 | current best |
| a1 / e7 (subtitle frame call after the cast / cast decl first) | DIFFER(12832v12832) | 0x124 | 87.09% | 15 | code change, not retail |
| what-if #448 (subtitleText.Info) = 60 (diagnostic) | DIFFER(12832v12832) | 0x11C | 99.03% | 8 | lever proven |
| what-if manager piece u794 = 30 (diagnostic) | DIFFER(12832v12832) | 0x11C | 99.03% | 8 | same lever |
| v01-v05 counter decl moves | DIFFER(12816..12832) | 0x124 | 81-87% | 16-21 | worse |
| w1 guarded do-while count loop, no flag | DIFFER(12896v12832) | 0x124 | 86.15% | 18 | ++it expanded, rotation lost |

Proven lever: prio(subtitleText.Info #448, 34) must exceed prio(then-arm manager piece u794, 54); measured threshold 55 (53 fails).
The piece comes from the split of the books-loop manager piece when mesh.Data (#209, ebx-only, 324) takes ebx (TRACE.md Step 2).
Fixing it alone gives retail's frame 0x11C and the icon branch (0x0060B58D..0x0060B718) and leaves 8 segments, all const-0 pieces:
count prologue (0x0060AFA1, retail `xor ebx,ebx` before the clear call), books-loop exit (zero ebx to `push ebx` 0x0060B936,
`cmp [eax],ecx` 0x0060B8DD), COMBAT release `cmp [eax],ecx` 0x0060C0B7, pads.

Next lever: the IL of the then-arm (B1 275-278, B3 278-283 relative to line 108) needs 21 priority points for Info: compare the
candidate list of retail's then-arm against ours (c2prio `--blocks` K per block) -- a 4-candidate difference in B2/B3 or a
different piece shape (a second ebx-only candidate between the two creates) is what retail must have had. Then the const-0
pieces (u797/u1000 count prologue; parent-None pieces from c2.dll 0x107A9B5E, which c2prio's PIECE_SITES lacks).

## Session 2 (2026-11-09, family push session 2)
Goal: find the natural spelling of the proven icon-branch lever (prio(subtitleText.Info) >= 55 over the then-arm manager piece,
54). Not found; the parked differ is unchanged (re-graded s2final: DIFFER(12832v12832), fr 0x124, exact 87.09%, rf 99.36%, 16
segments).

What was measured (TRACE.md "Session 2"):
- The piece's 54 = 4*(K341' + K343') - 2*K342' with K' = 7 / 8 / 3; its K' members besides fixed values are the const -1 piece
  (2 refs: the SUBTITLE / ICON CCharString temporaries), the const 1 piece (2 refs: SetValue's two `push 1`) and the const 0 piece
  (39 refs). Only 1-ref pieces are dropped before the re-sort, so with retail's pushes the -20 is not reachable by spelling.
- 18 then-arm surface spellings (step 1) and 125 code-identical edits over the whole body (step 3: pointer tests, bool args,
  copy-init, Get/Data, x.Get()->, post-increment, const index, swapped compare): byte-identical objects, every one.
- The coordinator's family (declaration first, scope ends earlier, statement order): each changes code; best K3/K6 15 segments
  by reordering calls retail does not reorder, K5 retail's frame 0x11C but releases subtitleText early (24 segments).
- c2's region pass (0x1078B51C) decoded: backward liveness then forward reach, region = live AND reached from a reference.

Leaderboard (session 2; compile-only fitness = tools/srch/cf.sh):

| variant | fitness | note |
|---|---|---|
| differ (p12/final2, /d2inlT71) | DIFFER(12832v12832), fr 0x124, ex 87.09%, rf 99.36%, 16 segs | parked, unchanged |
| C1-C6 counter declaration / clear order | 87.09-87.12%, 16 segs | c2 sinks the zero stores past the call either way |
| 125 code-identical whole-body edits | byte-identical | no allocator change |
| K3 / K6 (then-arm call order) | 15 segs | code change, not retail |
| K5 (subtitleText scope) | fr 0x11C, ex 91.86%, 24 segs | code change (early release) |
| what-if Info = 60 (session 1 diagnostic) | fr 0x11C, ex 99.03%, 8 segs | lever |

Next lever: model which blocks seed each piece bit in the region pass (c2.dll 0x1078B372 builds the pieces; 0x1078B51C is the
dataflow) and replay it on the books loop: the then-arm piece ends at B343 although the manager stays live through the icon
code, so the seed / kill sets decide its shape. The shape retail needs (then-arm manager in memory) then names the source change.
The const-0 residue (count prologue 0x0060AFA1, books-loop exit 0x0060B8F5 / 0x0060B936, COMBAT 0x0060C0B7) is the same pass.
