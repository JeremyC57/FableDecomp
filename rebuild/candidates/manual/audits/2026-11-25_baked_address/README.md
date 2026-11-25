# Grade `baked_address` (2026-11-25)

**Decision:** advisor decision (owner delegated), 2026-11-25. It answers NF415 RULINGS_NEEDED q1. A landed row that types a retail vftable, global, IAT slot or code address into its code as an integer is counted apart from the genuine figures. It gets one grade, `baked_address`, the same treatment as `baked_string`. It stays there until the row is ported, so that the address becomes the class's `??_7` or the bound symbol. An example of such a row is `self->vtbl = (void*)0x01230BA0;`. The bytes match retail, but the address is a plain immediate with no relocation, so neither the byte gates nor the relocation-identity check can see it. Such code only works linked against retail's exact layout.

**Detector:** `tools/decomp_pipeline/crawl/purity.py` `baked_data_addresses(text)`.
- It strips comments and string literals, then flags any hex token whose value is in `rebuild/oracles/data_vas.tsv`.
- `FABLE_DATA_VAS` overrides the path.
- It raises an error if the list is missing.
- `tools/landed_metrics.py` counts genuine-class rows with a hit as `baked_address`, unless the row is already `baked_string`. These rows are excluded from `landed_genuine_*`.
- The dashboard has a line for the grade.

**Key list:** `rebuild/oracles/data_vas.tsv`, built by `tools/decomp_pipeline/crawl/build_data_vas.py <Fable.exe>`. It holds VAs only, no retail bytes. 5,861 VAs, each verified against the retail image:

| kind | count | proof |
|---|---|---|
| vftable | 2,665 | RTTI: the dword before the VA points to a COL (signature 0), whose type descriptor names a `.?AV`/`.?AU` class, and the first slot is in `.text` |
| iat | 422 | a slot of the image's import address table |
| bound | 2,770 | a `reloc_bindings.tsv` line binds a non-function symbol to a `.rdata`/`.data`/`.idata` VA that is not a string or a function entry. The retail bytes of the witness's catalog function hold the VA as a dword. 38 bindings fail this check and are left out. |
| manual | 1 | 0x013D2E10, the particle-engine seed global. It has no table line; NF415 HOLD_ROWS proved it by hand. |
| code | 3 | from the `.text` rescan (below): the row's retail bytes hold the VA |

A flag constant that only lies in the image range is not listed. Examples are 0x01400000 (`m_dirtyFlags |= 0x1400000`) and 0x00800000, which is a function entry, used as `flags & 0x800000`.

**Control.** NF415's `pending/2026-11-25_nf415_handback/HOLD_ROWS.tsv` has 48 HOLD lines in 47 rows, plus 4 FALSE_POSITIVE lines in 2 rows.
- On HEAD, the detector hits all 47 HOLD rows.
- It hits 0 of the FALSE_POSITIVE rows.
- It hits 0 rows outside HOLD_ROWS (before the `.text` keys were added).
- Hits by kind: 34 vftable, 11 bound, 2 iat, 1 manual.

**`.text` rescan.** The advisor asked for this before the count is published. `TEXT_RESCAN.tsv` lists every hex token in the `.text` range in genuine landed rows. There are 74 rows; 27 of them match a catalog function entry.
- 71 are not addresses: flag masks (0x800000, 0x1000000, 0xC00000), sizes (0x53C000), UID tags (0xFFFFFD), window styles (0xCA0000), and macro arguments that are dropped. Examples of the dropped arguments: `COUNTED_POINTER(Name, ReleaseAddr)`, `FABLE_RETAIL_MAP(Name, OutVa, InVa)` and `ALLOC(Name, Addr)` never expand the address.
- 3 bake a code address:
  - 00864600 pushes the function 0x00643B3B as `(void*)0x643b3b`.
  - 00644627 passes 0x0041BE70 as an int.
  - 009f5de9, a catch funclet, returns its resume address 0x009F5DEF as a constant.
  They are keyed as `code` (CODE_ROWS in the builder).

**Out of scope:** a made-up DATA symbol bound to a vftable VA, for example `?CHairCardDef_vftable@@3QAXA` or `?HHQ_vtbl@@3HA` bound to 0x01230BA0. It is a relocation to a stand-in name, not a typed-in address. That is naming debt (T3 / the relocation table), not this grade. So 50 is not the number of vftable stand-ins.

**Headers:** `rebuild/include/**` and the row-local `.h` files hold no hex token in the data range. In the code range they hold only an unused enum of retail addresses (`fable_video_system.h`) and a macro that drops its address argument (`FABLE_TCE_ENTITY_DECL`).

**Unkeyed data-range scan (HEAD, after NF416–NF424):** across 57,155 genuine rows, every hex token in 0x0122D000–0x0146B47C that is in neither `data_vas.tsv` nor `string_vas.tsv` was listed. There are 4 tokens, all the `0x1400000` flag in 00c41e40 and 00c41fb0, NF415's known FALSE_POSITIVE lines. No `manual` key is missing.

**Count:** 50 rows. `HITS.tsv` lists each row and its baked VAs.

**Fix queue:**
- The 47 NF415 rows are queued in T3 (`nf415 vtable port`, plus the data / IAT lines). CDefPointeeBase is the first port and clears 12 rows.
- The 3 code rows need the callee or funclet spelled as a symbol. For 009f5de9, that means the catch funclet modelled as part of its parent function.
