# Landing 2026-11-08i: chunk 859 (Fable retry) round-2 match

| Row | Grade | Note |
|---|---|---|
| 0x00C75F80 CCombFilter::Allocate | **MATCH + PASS, landed** | 391 grades; the three ring stores go through `CRingBuffer* ring = &m_ring;` while `m_ring.Clear()` is called on the member (a store-forwarding barrier: VC7.1 forwards a member store into a later load only through the same base expression). The agent's `// Leaf:` was the prefix `?Allocate@CCombFilter`; the coordinator wrote the full bound symbol `?Allocate@CCombFilter@@QAEXIPAPAMPAII@Z` and landed again (same MATCH). |

No headers or names changed. `bindings_multi_check.py` prints 1, the pre-existing NullCoord. Round 2 left 16 rows parked after 100-254 grades each
(`pending/2026-11-08_linux_hand859/`).
