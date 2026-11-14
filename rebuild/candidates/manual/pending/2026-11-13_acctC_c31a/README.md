# acctC hand-back c31a (agentC31, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00601310 | CTCInventoryMap::ConstructQuestsScreenPC (catalog: ConstructQuestsScreen_MainMenu) | 6,931 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
No compiler flag, no pragma, no assert, no dead guard.

Evidenced: named `CText* descriptionText` (twin 0x005FE090; retail 0x00602972 / 0x0060297F); `FABLE_RETAIL_CCOUNTEDPOINTER`
(the twin's opt-in; retail threads the null arm at 0x006027F4 / 0x00602806).

REVIEW ITEMS FOR THE OWNER (the row byte-matches; these spellings emit the same bytes and only move c2's caller size into the
window [9347, 9403) in which retail calls the last scope-end ~CCountedPointer<CComponent> at 0x00602E09 and expands the six before it):
- INFERRED STLport texts, as per-row opt-ins in `stlport/fable_containers.h`: new `FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE`
  and `FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR`, existing `FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER`. Their basis is the
  author's recollection of STLport 4.6, not a source tree: check against the STLport sources before landing. If they hold,
  the `FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND` rows of this TU (0x0060C900, 0x0060EEB0, 0x0061B610, 0x0061E6D0) should be
  re-derived with them.
- FITTED: the sixteen `new CActionParamULONG(...)` results are named locals before `push_back` (+16 tuples; any uniform
  +6..+61 would do; no retail byte distinguishes the spelling).
- Name / label fixes: 0x00429F43 is CCountedPointer<NUISystem::CAction>::DeleteData (landed rows bind it under two other
  names); landed 00570332 binds `??1CCountedPointer<CComponent>` to 0x004291DE where this row has 0x00429C53.
