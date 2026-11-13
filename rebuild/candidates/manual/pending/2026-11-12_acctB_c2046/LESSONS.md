# Chunk 2046 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00D92380) Entity-script Mains written in the generator's own pattern -- `while (c) { NSF; if (T) return; ... }`, `if (T) return;`
  after every loop and at the top of every for / if body, `bool flag = false; while (!flag) { ... }` for a wait-until-any loop --
  matched block layout, shrink-wrapped pushes and the timer index in ebp at the first full compile; only an inline decision differed.
- (0x00D92380) vector<CScriptThing>::clear() in a script Main: VC7.1 expands STLport erase(first, last) (c1 67) at loop frequencies
  above ~2; retail calls 0x008AC9D0 -- use FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (0x00E74D70 did the same). `|| false`,
  do-while and break forms of the surrounding wait loop moved the site's frequency only from 5.2 to 2.6 (scores 64..72).
- (0x00D92380) `switch (rand() % 12)` with grouped case labels reproduces retail's byte index table + 5-entry jump table; the cases'
  string literals must be checked by hand against the jump-table order (relocations are masked): tools/drel.py pairs every dir32
  relocation (strings, jump table, vtables, globals) with the retail dword at the same offset once the instructions are equal.
- (0x00D77690) A 3 KB three-case cut-scene function matched at its second compile with the FirstFleeCutscene block spelling plus two
  existing devices; read the c2 inline trace first: the CWideScreenMagicPauseEntities ctor (c1 17) is the only site retail expands
  above the threshold while same-frequency MovieBase / handle ctors (c1 20) are called, so FORCEINLINE_CTOR, not /d2inlT.
- (0x00D92380) Catalogue rows that end on a jump table can be followed by an UNCATALOGUED function (0x00D92940 CGranny::Main): a
  reservation estimate that runs to the next catalogued start is no extent; vtable slot 1 of the class's Alloc names it.
- Tool note: a helper named dis.py next to a capstone script shadows the stdlib `dis` module (capstone imports inspect -> dis): name it
  odis.py.

Review at landing:
- New overlay opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT in engine/CQ_HobbeCaveScript.h (hdr/README.md); the landed
  CHobbeTorrent Alloc 0x00D8F3F0 / OnPersist 0x00D91930 rows keep local class text and could be retyped onto it.
- 0x00D92940 CQ_HobbeCaveScript::CGranny::Main is missing from the catalogue (rebuild/manifest/functions.tsv has nothing between
  0x00D92380 and the next rows): add it.
- Row sources contain the PDB method name DeregisterTimer (pDSTGame slot 88), as 145 landed rows do; tests avoid the substring.
- NAME_FIXES.tsv: three callee addresses with invented spellings in reloc_bindings.tsv.
