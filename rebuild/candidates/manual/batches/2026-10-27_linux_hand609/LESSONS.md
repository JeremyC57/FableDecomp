# Chunk 609 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A by-value vector operator whose sums retail evaluates X first and then copies with `fld st(1) ; fstp X ; fst Y ;
  fstp st(0) ; fstp st(0)` is `T result; result.X = ...; result.Y = ...; return result;` over a class with a user copy
  ctor (VC7.1 has no NRVO, the copy is real); `return T(X + o.X, Y + o.Y)` evaluates Y first (0x0048F40A, after three
  sweeps called it scheduling). The same form on CPlane's cross product (0x00A42170) made things worse: it fits the
  "sums then param-copy" shape only.
- "Pure register permutation" residues can come from a reference local that ends a pointer's live range early: accessing
  a map record through the node (`it->second.X` at every use) instead of `T& r = it->second` kept the node live and gave
  retail's this/map/node colouring at once (0x005785B3, 40 diff lines -> match).
- Check PDB access and const on every callee before tuning: four of 0x005785B3's callees were private or const in the PDB
  (GetTCHeroExperience/GetTCInventoryAbilities AAE, GetAbilityLevel/GetAbilityStatType QBE) and the setter is
  SetAbilityLevelTo, not the bound SetAbilityLevel; 0x0048F40A's callees were GFVXToAX/GFVYToAY, not GFAXToVX/GFAYToVY.
- An out-of-line twin names an inlined helper: 0x005812AE (`ret 8`, insert(pair(key, m)) then 0x00800020) is
  CTCHaste::SetActionSpeedMultiplier, which 0x006A02B0 inlines; its catalog label (GetBlockData) is propagated junk.
- The landed CActionUseDef::Transfer 0x006773F0 shows that retail's scalar CPersistContext::Transfer expansions rotate
  eax/edx/ecx; 0x0067E290's retail does not, so its residue is not in the Transfer template (five template spellings inert).
- `q.py` grades of the SAME address share $S/wine/q/<addr>: never grade two probes of one row in parallel (the second
  dies in objdump or grades the other's object). Different rows in parallel are fine.
- Residues no lever moved (park after 2-3 grades): frame packing that puts a full-expression pair temporary on the
  key/string word (0x006A02B0, 8 forms), x87 commutative operand choice with two members of `this` (0x00C6B9D0, 5 forms),
  esi/edx tap-loop colouring (0x00C77C80), this-register colouring (0x005C4671, 0x00957DB0, 0x007C6460).

Review at landing:
- 0x005EE01A SetAbilityLevelTo is "probable" (ForceAbilityLevelTo shares the signature); the landed body there is
  bound as ?SetAbilityLevel@CTCHeroAbilities@@ -- respell after deciding.
- NAME_FIXES.tsv: respell the landed callers of 0x009E1E10 / 0x009E1E40 (0x0048F230 uses __stdcall + address-suffixed
  names), 0x0048F1F4 (landed body public + CFontBank*), 0x00438570, 0x0056FF3D / 0x0056FFC5 (landed BuyStat 0x005786B5
  public), 0x004C9D60, 0x00800020, 0x005BE8F3, 0x004CC340. 0x00A01B50 GetPItem returning CBaseObject* is fleet-wide:
  decide before respelling.
- Both matched rows view an opaque header member through a local STL model (header debt in NOTES.md); 0x0048F40A also
  emits the COMDAT accessor ?GetEmbeddedHighlights@CSubtitleRenderer@@ (only the DrawTextEmbeddedHighlights leaf is graded).
- 0x005785B3's map find/insert keep the landed twin's descriptive CAbilityUndoMap model (vland RELOC_NAME warnings are
  the wrong catalog labels listed in NOTES.md).
- Rows that took more than 6 grades: 0x006A02B0 (10).
