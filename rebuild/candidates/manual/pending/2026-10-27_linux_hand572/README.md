# Chunk 572 (2026-10-29): 0x00919AE0 CAIStateGroup_BuyFromSeller::Update parked, DIFFER(2217v2220)

Name proven: vtable 0x0128FF0C (RTTI CAIStateGroup_BuyFromSeller) slot 12 (the sibling's slot 12 is landed 0x0093B1D0
Update) + Ego_r `?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ`. All 95 calls, frame, slots, merged fail blocks and the
shared free/dtor/epilogue tail match. Left: (1) participant loop -- retail `mov ecx,[esi+8]; cmp; je; mov ebx,ecx`, ours
loads straight into ebx (the 3 bytes); (2) stock-filter loop node/element registers swapped; (3) the DoNothing()==1 exit
block placed after the reply path instead of after the scratchpad GetTC block. ~70 probe grades; NOTES.md has the next
step. differ/ source + test (test PASS under g++ and VC7.1). NAME_FIXES.tsv: 7 rows queued.

Second round (owner asked for more tries, ~40 more probes): unchanged at 2217v2220. Theory (unconfirmed): VC7.1 orders
out-of-line blocks by creation order, so retail needs the timeout block created before the DoNothing block while still
falling through into the scratchpad code; no natural C++ form tried does that. NOTES.md lists every form tried.

Round 3 (a Fable-model agent, 2026-10-29): now DIFFER(2220v2220), 38 diff lines, all register renames -- instruction
streams identical. Closed: the DoNothing exit placement, by putting the timeout `RemoveMessage(request); break;` INSIDE the
wait loop and making the post-loop else only set the flag. Left: (1) participant loop `*it` in ebx vs retail's
ecx-then-ebx copy, (2) stock-filter loop ebx/ebp swap -- treated as a register-colouring tie; NOTES.md "Round 3" lists
~45 full grades + 60 reduced-model compiles. tools/: skel.py (block skeleton of an .obj or retail bytes), qq.py (q.py
with --qfe), run2.sh + e13.cpp (60-line reduced model, ~2 s per compile).
