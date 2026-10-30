# Parked-row retry sweep, wave 1 (2026-10-30): 114 of 237 retested parked rows landed, 46 more improved

Owner request: every parked / skipped row (2,362, `WORKLIST.tsv`, closest-first) retested by one agent with everything the
lane learned since the parks, with correct names and shared headers, and no row skipped for carrying a "lane gap" label.
Wave 1 touched 247 rows (STATUS.tsv is the exact record): **114 landed** here (31 MATCH + 83 RELOCATION_MATCH from the
worklist, 10 of them RELOCATION_MATCH_NEW callee rows a TU needed -- their oracle lines were derived from the retail bytes --
and 18 of the 114 are grade `retail_asm`: the engine CeilToLong/FloorToLong/FloatToLong `fadd; fistp dword` helpers, the
frndint/fscale block of 0x00C42FB0, the DX9 SDK wxutil.cpp `std / rep movsd` memmove 0x00CB0310, every evidence address
byte-checked by the coordinator), **46 improved** (better source in `../../pending/2026-10-30_parked_retry1/differ/` with the
levers tried and the next step; 00b17bd0.cpp there carries a naked FloatToLong helper and is NOT landable as is), 61 differ
unchanged, 11 confirmed gaps (each names the instruction pattern no source form produced), 2 HELD, 3 SKIP.
**Landed 2026-10-30 on the owner's decision (`held_mmx_sse2/`, payload + oracle, 10 rows, all RELOCATION_MATCH + PASS, purity
retail_asm; 124 rows landed in all): the GameCODA mixing kernels 0x00C5BEF0 / 0x00C5D1F0 / 0x00C5D2E0 / 0x00C6AAE0 / 0x00C6D410 / 0x00C6E290 /
0x00C6ED70 / 0x00C6F1A0 / 0x00C6F800 / 0x00C6FC80 run hand-written MMX / SSE2 loops (`pmulhw mm0,mm7`, `pmulhw xmm0,xmm7`,
`cvtps2dq`). Retail ran inline asm there, but VC7.1 has MMX/SSE2 intrinsics, so "an instruction VC7.1 cannot emit" is
arguable; the owner ruled they land as retail_asm (retail ran hand-written asm, and hand-scheduled loops are not reproducible from intrinsics). 0x00C6AC90 / 0x00C6AD10 (SKIP) depend on
0x00C6AAE0. HELD 0x00B05E10 / 0x00B1AD90: bytes match but they are per-TU triplicates of the landed
`CPSCLight::SmoothFadeValue` 0x00AF9000 (one address = one name: needs the duplicate-static convention, queued with the
libvorbis `@ilog2@4` triplicate 0x00C8DB60 / 0x00C8F060 / 0x00CA17A0 and the CDefString pair).
Per-lever tally (matched rows): same-TU static with a private calling convention (static body + its real caller in one TU;
`/Gr` C TUs where retail is fastcall) 88; same-TU callee bodies defined first (VC7.1 tracks the callee's clobbers) 22;
retail_asm helpers verbatim 10; parameter / fastcall-callee declaration order 3; named shared local (x87 order) 1. **The
"inline-asm fistp lane gap" label is disproved on every row tried**: 5 matched outright (00774610 among them) and on 29 more
the rounding block is now byte-exact with other residue left.
Names changed (NOTES.md has the per-row list): 004385F0 CCriminalRecord -> CTCVillage with the PDB fines API
(GetPNearestTCVillage, IsHeroCriminal, GetFineAmountForCurrentCrimes, GetNormalisedTimeLeftOnFines; SetBounty -> SetFine
0x00643FF6); 006CC800 GFGetCurrentFrame -> `?GetFrame@CWorld@@SIJXZ`; 00774610 map copy ctor bound out of line to
0x00778100; 0057B26F GetDefinitionManager / CGameDefinitionManager / CancelMove; 0072AAF0 AddTC const parameter; 00516830 /
00516200 GetBoundingInfo by reference, struct -> class; 005115E0 / 00511A10 the CFilter_Bind3 / CFilter_Bind2
instantiations; 00B84580 _M_insert_overflow bound name; 009C33A0 MakeBackupName -> GetMarkerFileName; word-wrap rows
g_dwWordWrapFlags -> g_WordWrapOptions. Descriptive names still in use: 006B2960 GetNavRegion, TransformHelperPoint_00c1c3f0,
LoadRecordAndReport_00c17260, SProcessorSettings_00c16ce0, CUnprovenMixKernel_<addr>, the 00C37C40 visitor types.
NAME_FIXES.tsv: 31 rows queued (prefix retry1: 005B30BB is `map<long,long>::operator[]`, bound as ??ACLongMap by 005A30A1;
004DA2AC is STLport `_Rb_tree::_M_erase`, bound as CLongLongTree by 006CF1D0; 00C0B180 / 00C0ACC0 fastcall parameter orders;
the libvorbis static spellings) + the triplicate review row. 40 vland RELOC_NAME warnings (bound spelling vs Ghidra label)
are listed in the agent's final report and NOTES.md (e.g. 00516200 PeekAllThings vs the PDB CThingSearchTools::PeekClassList).
Header debt: 00CBE583 (CScriptThing), 005EE77C (CTCHeroStats), 0066CD50 (CRegionMinimap / CSurface) model classes locally
although engine headers exist; SHARED_HEADER_AVAILABLE for CTCPhysicsBase, CCountedPointer, CGameJoystickManager, CManager.
Near misses for wave 2: 00A76E00 298v298 frame packing; 00439BB0 / 004385F0 one-step register rotation (next: PCrime as the
PDB CCountedPointer with an inline operator->); 00A88D10 (a TableCos writing its own parameter); 00B16220 372v372, 00914650
341v340, 0056EF2D 66v66.
Coordinator: vland --land 114/114 (no RELOC_IDENTITY; two RELOC_CONFLICT warnings on the ilog2 triplicate), multi-check 0;
PARKED_INDEX.tsv not regenerated (the chunk records tool is per-chunk; the sweep's STATUS.tsv is the record). Wave 2: the
2,121 untouched rows split into six address-contiguous sections (`$S/retry/sec1..6`), two agents at a time.
