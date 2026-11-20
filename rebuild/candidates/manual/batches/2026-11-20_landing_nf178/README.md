# Landing nf178 (Account A, 2026-11-20): cut-scene cast-map family, narrowing partial (30 rows)

Agent NF178 took the cut-scene cast-map family as a dedicated pass (5 queue entries: CCutsceneCastMap ctor 0x00CDBF70 / dtor 0x00CDBFB0,
landed under the CRetailTree_erase00cd344a stand-in, and the roughly 40 rows that bind them under wrong spellings). Follow-up agent
NF178b fixed the pick-any problem before landing. Files: `naming/2026-11-20_agentNF178/` (NF178b's version, with the four held rows in
`held/` and NF178's original census `cutscene_wrong_nf158.tsv`).

- 30 rows land, each moved onto the real STLport `map<CCharString, CScriptGameResourceObjectScriptedThingBase>` from stlport/fable_map.h.
  Their ctor / dtor / operator[] / RunCutsceneMacro bindings move onto spellings that landed rows or the self rows already bind, so
  every touched address's spelling set only shrinks (advisor ruling: land as a narrowing partial). tablecheck: 0x00CDBF70 3 -> 2,
  0x00CDBFB0 5 -> 4, 0x004AA840, 0x012D771C and 0x00DA71E0 shrink; none grows. Seven rows that destroyed the map by hand now use the
  object's lifetime (four with an `// Inferred:` scope line); 0x00DC8C80 / 0x00E1CFD0 / 0x00DF0B70 moved a row-local `std` vector onto
  stlport/fable_containers.h.
- Pick-any copies (advisor rule, EXTRA_RULES 2026-11-20): as first handed back, every row emitted `_M_erase` (0x00CD344A) and the tree
  `clear` (0x00CD3699) with non-retail bodies. NF178b made both declaration-only through two add-only opt-ins in stlport/fable_map.h,
  `FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY` and `FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY` (plus NF178's `FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY`,
  now unused by rows). The default path is token-identical (`orig` equals HEAD; 903 includers). In the landed objects neither function
  is defined; the only newly defined symbols bound to another address are the map ctor (RELOCATION_MATCH against 0x00CDBF70) and the
  vector<CScriptThing> allocator ctor, byte-identical to its owner 0x008AC5A0 (`logs/pickany_check.txt`).
- Held (4): 0x00E12F20, 0x00E1CD40, 0x00E292C0, 0x00E3E720. Their implicit ~map copy inlines the tree destructor and clear, so the clear
  copy stays; they need a header clear that compiles to retail's /Os body, or the 0x00CD3699 debt settled first (queued).
- Left for later (queued): 0x00D246C0 / 0x00DDD030 (hand-destroyed handles at early returns), 0x00DC21F0 (DIFFER, frame slot), 0x00E2AB30
  (really CQS_ScytheInfoScript's ??_G), the two self rows, and eight operator[] / RunCutsceneMacro stand-in callers outside the census.
  Landed 0x00D29880's existing 51-byte clear / 70-byte _M_erase copies are queued as DEBT.
- Recheck (`logs/recheck.log`): 30 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created; includers 981
  good, 0 bad; owner recheck good.
- Link (`logs/link_landing.log`): 10 LNK2005, the same ten as HEAD (`logs/link_control_head.log`).
