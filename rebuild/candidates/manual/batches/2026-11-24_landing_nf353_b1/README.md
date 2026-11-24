# nf353 b1 / b2 / b3: FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY family port on the map<CCharString,STB>::operator[] emitters (59 rows: 25 + 25 + 9)

Agent NF353 (Account A naming queue; hand-back `pending/2026-11-24_nf353_handback/`). Each row emitted its own NEQ copy of
`map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[]` (retail 0x00CD3D2E). It now defines
FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY (`// Fidelity: scaffolding, not retail`) and the reference resolves to the landed owner 0x00CD3D2E
(nf350), whose `// Leaf:` is the exact symbol. The opt-in is TU-wide: 11 rows also drop their NEQ map<CCharString,CCharString>::operator[]
copy, resolved by owner 0x009AC700 (an extra Fidelity line names it). No header, table or test change.

Coordinator verification (objects rebuilt from the repo after landing, `$S/v353.sh`):
- vland `--recheck-landed`: 59/59 RELOCATION_MATCH PASS (HEAD: the same, agent logs/recheck_head.log).
- pk_all with `--leaf=<// Leaf:>` and the catalog length, HEAD (agent evidence) v new: the leaf line names the row's own function in all 59;
  no copy turned NEQ, no new NEQ copy, no NEQ reach flip n -> y. The only changed verdicts are 26 rows where the NEQ
  `~pair<const CCharString, STB>` copy is no longer reached from the leaf (y -> n). pickany_vs_retail: no new NEQ line in any row.
- Agent: leafrel identical 59/59; only COMDAT drops, none added; every added UND (1 per row, 2 in the 11) is defined under the identical
  string by its owner's object (/FORCE link per batch with a negative control); no reloc_bindings witness loses its symbol.
- self_symbols.tsv refreshed for the two owners (and 0x00E1A990, stale since nf351 b2), measured from rebuilt objects.

Rulings (coordinator, 2026-11-24, on the agent's five questions):
1. The 9 T3 column-2 holes (0x00CBFB7D; 0x00D13580, 0x00D19BD0, 0x00D1B200, 0x00D22F30, 0x00D2D930 (/d2inlS15); 0x00D52E90, 0x00D6D670
   (nf331); 0x00E55CE0 (nf294)) stay out of this landing and are queued (T3 nf353) as a follow-up where each hold is read first.
2. self_symbols: refreshed at landing (above).
3. 0x0041C820: 0x0058A0C7 is not an EQ definer (its copy is 19 B, NEQ); corrected in nf352_b1k's README. Six non-hole EQ definers remain.
4. The drop census is a lower bound (159 compiled of 706 FABLE_RETAIL_STLP_MAP users) and is accepted: every dropped copy with a retail
   address keeps EQ definers outside the batch (0x0041C820: 6; STB copy ctor 0x00905330: its own strong owner row; all others >= 72).
5. The four rows that also drop vftable COMDATs (0x00CE7670, 0x00E6DA80: `??_7CScriptGameResourceObjectScriptedThingBase@@6B@`;
   0x00D45060, 0x00DC1770: that and `??_7CScriptGameResourceObjectBase@@6B@`) land: 50+ outside non-hole objects keep byte- and
   relocation-identical copies of each (evidence/drop_noaddr_copies.tsv), so the link keeps the same data.
