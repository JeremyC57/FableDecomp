# ESCALATE 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B) -- AUTH1, 2026-11-21
P = /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-20_p25_open/008e18d0

## Best sources (repo, unchanged)
- No-device best: P/008e18d0.cpp (= p25N 8e_u_nodevice + IMPLICIT_DTOR overlay), DIFFER(2045v2073) at default T; T64..66 2149
  (fewest difflines, 544); p25R /Gr /d2inlT65 base 8e_M: 2165v2073. Device (do{}while(0), not landable): P/DEVICE_dowhile0_not_landable.cpp.

## What is left (p25N / p25R site tables)
| site | ours (f / score) | retail |
| LowerBound loop 2 | 6.25 / 63 | CALL 0x0040F020 |
| LowerBound sky    | 6.25 / 63 | EXPAND |
| ~CParticleUpdateParameters loop 1 | 2.5 / 65 (dtor with inner ~vector expanded) | CALL 0x006E2500 |
| dtor COMDAT inner ~vector<pair<ulong,ParamDesc>> | 1.0 / 65 | EXPAND (retail 0x006E2500, a COMDAT linked from ANOTHER obj) |
| _M_insert_overflow loop 1 | 1.25 / 66 | CALL 0x0076ADA0 |
Every hooked field of the two LowerBound records is identical except g (growth, only in the 9000 cap) and the site pointer. Callee
texts are proven by cmpcomdat (dtor, LowerBound, Calc, overflow with fastcall uninit helpers = /Gr TU default).

## Hypotheses
(a) A branch-probability input: retail behaves as if both loop bodies carry an extra x0.5 the sky lacks (FINDINGS 2026-11-21 p25N);
    Ego_d's line table shows no extra `if` in the loops, so it would sit inside an inline helper the loops reach (loop 2's extra
    vector operator[] is the one text difference: Ego_d 3344 v 3356 = +10 B).
(b) Compile order: if UpdateLightningBeams were compiled before LowerBound / the dtor (budget path), decisions follow the running total,
    not the score. But the budget path rejects LATER sites; retail calls the EARLIER (loop 2) one, so (b) alone does not fit.
(c) [closed by p25R's arithmetic] The loop-1 CALL at T65 needs the large dtor (inner ~vector expanded) in THIS TU, which needs the
    inner site to score below 65 here; the one lever p25R left open is one IL tuple fewer in ~vector<pair<ulong,ParamDesc>>'s post-inline
    body (no shared-header form moved it).

## Question for the advisor
Should I (1) hunt the one-tuple-smaller ~vector<pair<ulong,ParamDesc>> text (STLport-shaped destroy / deallocate bodies, checked against
Ego_d / FableWin sizes) and the loop-2 frequency input (inside operator[] / GetTC) with inl.py, bounded, or (2) does the row wait for a
c2 frequency model? Regrade today (AUTH1, overlay re-made in 008e18d0/hdr: the repo folder lacks the IMPLICIT_DTOR opt-in, without it the
best is 1903v2073): P/008e18d0.cpp = DIFFER(2045v2073) default T, DIFFER(2149v2073) with /Gr /d2inlT65.
