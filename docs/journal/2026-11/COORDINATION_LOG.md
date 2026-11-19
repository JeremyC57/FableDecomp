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
