# Chunk 836 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 836, 0x00D328C0) The final idle loop after a scope whose return pad retail places BEFORE the loop (`jmp test`) is the chunk 817
  else-wait form `if (T) return; for (;;) { NSF; if (T) return; }`. `while (!T) NSF;` and `for (;; NSF) { if (T) return; }` rotate the
  loop and put the pad after it.
- (chunk 836, 0x00ECD9E0) In a long Main the implicit MovieBase ctor can reproduce retail's mixed ctor sites by itself (two sites call
  0x006E7B60, one expands) when MOVIEBASE_OUTOFLINE_DTOR is set. Grade the implicit ctor before parking on the chunk 823 mixed-ctor
  case. The chunk 808 `parent` reference release fixed the last two lines.
- (chunk 836, 0x00E799F0 / 0x00CFBDF0) Removing the row-local inline ~CScriptThing makes the nested predicate case match: outer
  ~CIsThingConscious / ~CIsThingAlive expanded, nested ~CScriptThing called, filter vtable hoisted. But it costs every other expanded
  ~CScriptThing site. So the chunk 822 blocker is exactly "the same ~CScriptThing must be inline for the body and out of line inside the
  filter dtor". A row with no other expanded ~CScriptThing site (0x00D21040) matches its predicate with no inline
  ~CScriptThing.
- (chunk 836, 0x00D21040) The copy-ctor twin of that blocker: retail calls 0x004ABE90 inside the expanded CIsThingAlive ctor but
  expands a by-value `CScriptThing x = vec[i]`. The expanded copy is ONE vtable store, because VC7.1 drops the dead CBaseClass store
  for a stack local but keeps both for an outgoing by-value argument (0x00CF0D20). IMPLICIT_COPY, a row-local inline copy ctor and a
  named filter block all expand both sites.
- (chunk 836, 0x00CF0D20) Grading with STB OUTOFLINE_DTOR as a meter shows the movie dtors start to expand once ~seh_me stops using
  the inline budget. This is another sign of the function-wide budget behind the CQ_ArenaHoldingScriptScript fan parks.

Review at landing:
- 0x00CDBF70 / 0x00CDBFB0 each carry several symbols in reloc_bindings.tsv and placeholder catalog labels (NAME_FIXES.tsv, LABELS.tsv).
- engine/CQ_EndGameBossBattleScript.h is a new header. The landed siblings 0x00D2FB20 (Jack Main) and 0x00D2FC00 (Sister Alloc) still
  carry local models of the same class.
- Chunk 814's pending CEntranceGuard opt-in edits the same CQ_ArenaHoldingScriptScript.h. The blocks are independent.
- The parked rows got fewer than 10 grades (5 / 4 / 6 / 4). Each hits a documented blocker. A non-GOTCHAS probe per row is named in
  STATUS.tsv: no inline ~CScriptThing (a5, c2 / c4), STB OUTOFLINE_DTOR meter (e6), named filter / row-local inline copy (h3 / h4).
