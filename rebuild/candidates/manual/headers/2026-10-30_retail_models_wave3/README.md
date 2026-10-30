# Retail class-model migration, wave 3 (2026-10-30): 838 landed rows onto engine/CBaseClass.h, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (none); HEADERS.md; NEXT.md (wave 4 plan and dry-run counts); LESSONS.md;
logs/copy_experiment/ (the def-chain copy-member experiment: sources, os.log, README).

Rule settled (LESSONS item 1): **CDefClassBase / CParentDefClassBase operator= and copy ctor stay declared out of line.**
Implicit members are indistinguishable at /Os (39/39 callers of 0x00431ED0 unchanged) but refuted at /O2: RTM 3077 inlines
them into 0x00434370 (153v123) and 0x00625350 (145v115) where retail calls; and no declaration (implicit, explicit
out-of-line in the TU, explicit inline, inline_depth 1-3, /Os /Ot /Og, QFE 4035) reproduces retail's expansion in the five
/O2 expanders 0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980, which keep their row-local __forceinline chain.
Standalone 0x00431ED0 / 0x00431F60 match as implicit members; 0x00431F10 does not (implicit, it inlines 0x00431F60 which
retail calls). The CScriptThing copy (wave 1 open item 1) carries the same rule: out-of-line in the header, a row-local
InlineCopy tag at the inline site. /GL not tested.

Rows (all CBaseClass family): exact shapes 194 (cbb21bf1 100, 3ed153c6 53, 9b25d360 29, bbb0d2cf 12), dtor-only 461,
inline-dtor 178 (77992de8 96, 62d3d4ae 82), dtor + copy-init 5 (user copy ctor with no base initializer: on the shared
header it must say `: CBaseClass(other)`, scripts/mig_bc_copyinit.py). Agent recheck 838/838 PASS (42 MATCH, 796
RELOCATION_MATCH); mcheck vs HEAD 832 SAME + 6 same-bytes label/hash renames. Coordinator independent spot regrade after
cherry-pick onto e06f491fc: 62/62 PASS (coordinator_spot_recheck.log). Bindings unchanged, multi-check 0.
Headers: comment-only changes (CParentDefClassBase.h, CDefClassBase.h record the rule). NAME_FIXES: 0x00431F60 is
??4CDefPointeeBase@@QAEAAV0@ABV0@@Z (FableWin 0x01937730; implicit member compiles to MATCH); landed spellings
??4CDefRootBase@@... (row 00432e60) and leaf ??4CResource@@... (row 00431f60) to respell.
Wave 4 ready (NEXT.md): BC remainder 612 (64 whitespace-only, 169 mig_bc_copy, 146 inline-ctor rows for bc_ctor_rule, ~233
v00 shapes), CGSI --rename 693, PDCB 108 (12 = wave 2's reverts), DCB clusters. Regrade cost about 6 rows/min on the shared lane.
