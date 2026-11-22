# 0x008E18D0 AUTH10 log
- baseline b0 (AUTH9 b0 + overlay): DIFFER(2045v2073). Hypothesis set 1: sibling 0x007A1380 opt-ins b0 lacks, one at a time then all.
- s0..s10 (sibling 0x007A1380 opt-ins, one each): all 2045v2073 except STLP_VECTOR_OVERFLOW_OUTOFLINE 1925, OUTOFLINE_UNINIT_COPY 2184, _FILL_N 2189 (all worse). 11 grades.
- loop forms (`!=`, post-inc, unsigned, braces, continue, const bound, member bound) on minis: all identical f (2.5 in / after). No loop-form lever.
- NEW TOOL inl_force.py (tools/): forces c2's inline result per (callee, occurrence). Forcing retail's split on b0 (/Gr T65: loop-1 dtor call,
  loop-2 LowerBound call, the two 3.125 sky __uninitialized_copy expand) gives the retail call sequence exactly but DIFFER(2057v2073), 169 masked diff ins:
  frame slots differ everywhere; the sky's end_pos is copy-propagated in ours, materialised in retail (0x1C..0x24).
- e2: sky `C3DVector end_pos = start_pos + C3DVector(0.0f, 0.0f, 15.0f);` (one statement, fits Ego_d line 3358 = one line 49 B) + forced split:
  RELOCATION_MATCH-equivalent (0 real diff ins). So the residue is ONLY the four inline decisions plus this one statement.
- e3 = e2 + Ego_d dead locals lifetime_in_seconds / lifetime_of_main_beams (lines 3283/3285, 0x008DD7E0 spelling): forced = RELOCATION_MATCH (gobj.py);
  unforced default 2061v2073, /Gr T64..66 2165v2073 (285 masked diff ins), T63 1992, T>=68 2293.
- Ego_d check: CThing::GetTC<CTCDParticleEmitter> exists out of line in Ego_d (74 B) yet lines 3316/3344/3356 are 517/544/534 B, so Ego_d expanded
  the forceinline body at these sites (same as p25R's reading). Ego_d locals: n, num_beams(_ends) are long (t12), all three ptc locals one type.
- c2 switch probe (minis): -inlfcsw/-inlflcsw/-inld/-inlnobr/-inlnild/-inlniln 2 change nothing; -inlld2 lowers scores only. No f lever.
- e4 = b1 (MAKE_VECTOR_PARAM_FROM_BASE) + e3 deltas: forced RELOCATION_MATCH (proof/), unforced /Gr T65 2044v2073. Escalated.
