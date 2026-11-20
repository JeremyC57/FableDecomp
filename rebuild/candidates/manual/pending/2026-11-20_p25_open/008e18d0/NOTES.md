# 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams: parked (p25N, 2026-11-20)

Best no-device: `8e_u_nodevice.cpp` + overlay `hdr/` = DIFFER(2045v2073) default T; T sweep (`tsweep_u.txt`): T<=63 1976, T64..66 2149
(544 difflines, fewest), T>=68 2277. Device version (`8e_ud_DEVICE_dowhile0.cpp`, do{}while(0) round the sky block, a DEVICE, not
landable): T60 2117, T64/66 2149, default 2277. p25M's device best `8e_p25M_best_DEVICE.cpp` 2028.

## Callee texts proved by bytes (probe `8e_p3.cpp`, inline_depth(0) probe function only, cmpcomdat)
| callee | retail | result |
|---|---|---|
| ~CParticleUpdateParameters (FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR, hdr/engine) | 0x006E2500 | SAME |
| CVectorMap::LowerBound (CVECTORMAP_INLINE_LOWERBOUND) | 0x0040F020 | SAME |
| CCRC::Calc (CCRC_PASS22_BODY; CALC_BODY too) | 0x004014A0 | SAME |
| vector<pair<ulong,C3DVector>>::_M_insert_overflow | 0x0076ADA0 | DIFF with the default STL; SAME once __uninitialized_copy and __uninitialized_fill_n are __fastcall (test opt-in FABLE_RETAIL_STLP_UNINIT_FASTCALL in hdr/stlport/fable_containers.h: the FLARE_ABI fastcall arms; NOT add-only yet, a measurement overlay) |
| __uninitialized_copy (fastcall) | 0x00768FC0 (ecx/edx, ret 8) | SAME |
Retail 0x0076ADA0 CALLS 0x00768FC0 twice and 0x00769000 (fill_n) -- the copies are not part of the overflow body.
With the fastcall texts, uninit_copy c1 51 (was 64): the sky copies expand at default T (the old "sky f >= 88" requirement is gone),
but loop 1's overflow (f 1.25, score 66) now expands too (retail calls 0x0076ADA0).

## Site table (our f / score, trace of 8e_u at T74; retail decision)
| callee | loop 1 | loop 2 | sky | mates (16A0 / 20F0 / 24F0) |
|---|---|---|---|---|
| LowerBound | 1.25/82 call | 6.25/63 CALL | 6.25/63 EXPAND | 0.5/100, 0.625/95, 2.5/70 call |
| Calc | 2.5 always-inline, expand | - | 12.5 expand | 0.25 / 1.25 always-inline in ours; CALLED in retail (constant args, same as the row) |
| _M_insert_overflow | 1.25/66 call | - | 6.25/51 expand | 0.5/93, 0.625/89 call |
| uninit_copy (in overflow) | (called overflow) | - | 6.25/63, 3.125/68 expand | - |
| ~CParticleUpdateParameters | 2.5/65 call | - | 12.5/56 expand | 0.25/117, 1.25/76 call |
Contradiction: LowerBound loop 2 v sky, same instantiation (sky's expanded halving loop = 0x0040F020: stride 8, cdq/sar, cmp 0x4B), same
f, same score, opposite decisions. No T splits them. Calc: mates call at constant args too, so not argument constness.
f facts: expanded-inline loops do not change later f (identical with/without the Calc body); no retail back-edge encloses the sky
(0x008E1D93..); for x5/x5, while x9/x1, guarded do-while x5/x1, do{}while(0) x10. The sky LowerBound sits inside GetTC's HasTC test
(half the block's 12.5) and Ego_d puts GetTC (line 3356) before everything else in the block.
Ego_d / FableWin: both 3407 B for this function (creature_modes.obj #230); the 500+ B lines (3298 pre-loop, 3316/3344/3356 one per
block) are the GetTC lines, the same in all blocks.
TU-mates: same obj (FableWin #217/#230/#274/#284, plus CreateLightningBeams #231 = landed 0x008DDAD0 with PASS22_BODY + /d2inlT65).

FINDINGS-ready: 2026-11-20 (p25N, 0x008E18D0 parked) callee texts proved by cmpcomdat: dtor 0x006E2500, LowerBound 0x0040F020, Calc
0x004014A0 SAME; _M_insert_overflow 0x0076ADA0 SAME only with __fastcall __uninitialized_copy / fill_n (0x00768FC0 ecx/edx ret 8), so the
TU's STL uninitialized helpers are fastcall. With them no T matches (best T64..66 2149v2073); loop-2 and sky LowerBound sites have the same
instantiation, f and score but opposite retail decisions, so the heuristic inliner cannot reproduce retail from this text. Files: parked/008e18d0.
