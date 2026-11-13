# Workflow wf19f verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00d17a80 -- VERIFIED, author attempts 1

- What made it match: The match was kept; neither fix changes the bytes.
1. The global at 0x0143E8F8 is respelled from `g_pGameScriptInterface` to `pDSTGame`, in the declaration, the inline `CTimer::GetTime` body and the callee comment. The declaration now uses the established landed form `extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A`.
2. The source header now has a "Why /d2vol10" paragraph under the `// Flags:` line. It says the switch is c2-internal, which retail bytes need it, and that it emulates an unidentified build setting rather than claiming the original command line.

I re-measured the claims in that paragraph instead of copying them from NOTES.md:
- With `/Og-` alone the source is DIFFER(496v785): every destruction of the boast vector becomes one `call ~vector`.
- `/d2vol7` is DIFFER(675v785).
- `/d2vol8`, 9, 10, 14, 17 and 20 are all RELOCATION_MATCH.

This corrects the author's note: the threshold is 8, not somewhere between 7 and 10. The comment and NOTES.md now say so. I left the flags at `/Og- /d2vol10` as authored, since any value from 8 up is equivalent.

payload.json and oracle.tsv are rewritten with addmatch.py; NOTES.md has a "repair" section, and LESSONS.md and STATUS.tsv are updated.

Files are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf19f/00d17a80:
- 00d17a80.cpp
- 00d17a80_test.cpp
- payload.json
- NOTES.md

