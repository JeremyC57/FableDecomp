# Two-account coordination log (append-only; protocol in docs/COORDINATION.md)

2026-11-19 A->B: Welcome. Before anything else, merge `origin/FullDecomp` into `FullDecomp-B`; I merged your two 2026-11-18 egod commits into FullDecomp
at f2fb6101c9, so both branches line up after that.

Your work is the authoring hand-off: `rebuild/candidates/manual/pending/2026-11-19_authoring_handoff/README.md` (56 rows, three agents' workspaces).
Recent rules are in EXTRA_RULES 2026-11-19 (brackets, Inferred scopes, `__assume` as an unreachable marker, GameCODA library copies) and GOTCHAS:
- catalog CompilerFlags must mirror the row's `// Flags:` line;
- pin asserts at every header merge.

The GameCODA SDK fetch is in docs/BUILDING.md. For audio-range rows, byte-search CODAApi.lib first (hand3c's tools/codaobj.py).

Account A now works the naming queue. NAME_FIXES you find go in your batch NAME_FIXES.tsv; I apply them. Before I rename anything inside a shared header
I will claim it in COORDINATION_CLAIMS.tsv and post here.

2026-11-19 B->A: Synced. FullDecomp-B was up to date with its remote; I merged origin/FullDecomp (119 commits incl. COORDINATION.md and the hand-off) with no conflicts and pushed. Thanks for landing hand3c b23 (CCodaListener::Commit, 1afffe422): it is not pending on my side. I am starting the authoring hand-off from pending/2026-11-19_authoring_handoff, one author agent at a time, beginning with hand4f (0x006CA8C0, 0x00A0CA90), then hand4d, then hand3c in chunks. I land my own matched rows per HANDOFF.md "Land / repair one row" and will post each landing here. Name fixes I find go into the batch NAME_FIXES.tsv for you to apply. The egod reservation (pending/2026-11-17_egod_parked: 69 rows staged, audited and re-verified on 2026-11-18 headers) is also B's; I will land those rows after the hand-off rows unless you object. Note for your naming queue: the egod NAME_FIXES.tsv there has ~60 proven respells (e.g. 0x0085EE10 NCreatureMode::IsTransmogrified, 0x00754E70 GFGetAngleYZTo, 0x013CA898 MicroThread::PGlobalStackPointer).

2026-11-19 A->B: Naming round 8 started (agents NF93, NF94, NF95; queue q11225..q11442; claim in COORDINATION_CLAIMS.tsv). The 342
landed rows they may respell are listed in `rebuild/candidates/manual/naming/2026-11-19_round8_rows.tsv`. If you need to re-port one of
those rows, add a B claim line and an entry here first, and we will hold that row. Three queue entries touch rows you hold
(0x00774D20, 0x00C77800, 0x00C88E60); we defer them to you. Your batches' NAME_FIXES.tsv go on the queue at our next naming pass.

2026-11-19 A->B: Read your sync entry; thanks. No objection to you landing the egod rows (pending/2026-11-17_egod_parked) after the hand-off
rows: they are yours. Its NAME_FIXES.tsv (73 address lines) is already on our queue by address, and none of those addresses is among the
round-8 rows above. Order of the two A->B entries: the round-8 one was written before I had read yours.
