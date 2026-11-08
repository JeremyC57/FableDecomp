# Hand chunk 882 (CTCInventoryQuests): 0 of 1 byte-exact

## What matched (0)

## Parked for Fable 5.1 (1)
- 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC DIFFER(12357v12356): register choice -- const 0 cached in ebx and `type` in edi through the per-quest body where retail reloads `[ebp+14h]` and keeps quest_card / state_index in ebx (`lea eax,[ebx+8]`, `inc ebx` 0x00621090); frame 0x42C v 0x41C; inlined CCharString== with this/other in ecx/eax (retail eax/ecx, 0x0061FB20) | tried: 55 source probes (v1..v43; first-loop guard form fixed the first loop + heroStats LowerBound; `+=` for the state position; j declared before the inner loop; switch / ternary / bool-local / loop-shape variants), 40 flag and diagnostic grades (/d2inlS0-40, /d2inlT40-74, /d2inlcrmax / userinl / nlw / fcsa / fcsw, plain-inline CDefPointer and GetTC, extra allocator level, dummy inline statements) | next: find the inline-budget consumer retail spends before the take block (4 dummy inline CCountedPointer<CAction>(new CAction) statements reproduce retail's late out-of-line sites), then drop the six inline_depth(0) stand-ins; then `CTCInventoryMap* inventory_map = 0;` in the loop (retail `xor ecx,ecx`) once the const-0 split keeps the loop-top `xor edi,edi ; xor ebx,ebx`.
  - Decoded phases and every grade: WORKLOG.md. Best source: differ/0061e6d0.cpp (= probe/v42.cpp with header lines).
  - Row extent verified: 12,356 bytes, `ret 14h` at 0x00621711, int3 padding to CloseInventoryPC 0x00621720; one function.

## Skipped
(none)

## Label corrections
- 0x007022A0 is CTCQuestCard::IsExclusive (landed as IsCore); 0x007022E0 is CTCQuestCard::IsVignette (propagated as
  CTCShotDefinitionBase::CanBeHitByMeleeStrike); 0x00411570 is CBasicString<char>::Compare (landed as strcmp_signed). See NAME_FIXES.tsv.
- Review note: the landed CloseInventoryPC (0x00621720) labels +0x20 as quest cards; it is CTCInventoryBase's Categories.
