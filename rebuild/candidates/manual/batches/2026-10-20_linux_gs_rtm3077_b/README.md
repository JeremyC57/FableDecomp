# Second RTM-compiler /GS batch (2026-10-20)

The full re-grade of every parked row under cl 13.10.3077 (the retail compiler) found 19 more rows that differ from retail
only in the DDK 13.10.4035 stack-cookie prologue: 004165e8, 0048dd94, 006294a0, 00688ba0, 0068e2c0, 0068e330, 0068fee0,
006a4040, 007e7c00, 0088e150, 00a4fd70, 00aaf5b0, 00abae50, 00abbe90, 00ac4ec0, and in a third pass 005e3df0, 00629390, 0085ca80, 0085caf0 (all `/GS`; `payload_pass3.json`). Tests were written by an Opus
agent; landed with `verify_and_land.py --land` under `FABLE_VC71=$S/wine/vc71rtm LINK=bufferoverflowu.lib` (no catalog
tag: RTM is the catalog's default toolset). `payload.json` holds the second pass (the first pass, same sources, landed the
other 8; both logs are in `verify.log`). A 16th sweep hit, 009b98b0, was already in the catalog (stale parked copy removed).
Gotcha: a direct verifier call needs the trailing int3 padding trimmed from scratch chunk oracles (land_pending.sh does it).
