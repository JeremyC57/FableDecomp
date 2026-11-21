# 0x008E18D0 UpdateLightningBeams -- p25R 2026-11-21
Base p25N src/8e_M.cpp + hdr8e (IMPLICIT_DTOR), /Gr /d2inlT65 (T65: landed TU-mate 0x008DDAD0 carries /d2inlT65): DIFFER(2165v2073).
Trace: tmp/c2/inl_8eM.json (inl_host.py), tools/inlsum8e.py. Decision rule measured: EXPAND iff sc < T (T65: 63 expand, 65/66/68 call;
T74: 68 expand).

## The loop-1 dtor decision record (brief step), old v current
| config | dtor COMDAT's own inner ~vector<pair<ulong,ParamDesc>> | dtor sz / c1 at the sites | loop-1 sc (f 2.5) | sky sc (f 12.5) |
|---|---|---|---|---|
| p25N 8e_u (T74 default) | sc 65 @ f1.0, EXPAND (65 < 74) | 73 / 67 | 65 EXPAND | 56 EXPAND |
| 8e_M (/Gr, T65) | sc 65 @ f1.0, call (65 not < 65) | 35 / 29 | 23 EXPAND | 20 EXPAND |
So the "callee cost" input is the dtor's post-inline size, set by its own inner ~vector decision; that decision (65 @ f1) and the
loop-1 site (65 @ f2.5 with the big dtor) carry the SAME score with opposite retail outcomes if both happen in this TU (retail
0x006E2500 has the vector dtor expanded; loop 1 calls 0x006E2500). Caveat: the linker may have taken the 0x006E2500 COMDAT from another
obj, so its body need not reflect this TU's compile; but with this TU's dtor small (T<=65) both sites expand.
No single T satisfies: inner ~vector expand (T>65), loop-1 dtor call (T<=65), LowerBound loop 2 call / sky expand (same f 6.25, sc 63).
Frequency arithmetic (p25N multipliers): a plain `for` loop 2 body always equals the sky's f (both x5 x5); an `if` wrapper gives
after-factor 3 v body 2.5 (too small); while / do-while give the wrong direction.

## Bounded pass (main 2026-11-21, checks 1-3) -- p25R
Harness: tmp/dt/run.sh <tag> <defines...> (8e_M + defines, hdr8e, /Gr /d2inlT65, inl.py trace) + tmp/dt/sum.py (dtor COMDAT inner
~vector, every dtor / ~vector<PD> / LowerBound / uninit site of the row).
1. Inner site (dtor COMDAT's ~vector<pair<ulong,ParamDesc>>, f1.0): sz 50, c1 47, sc 65 -> call, in EVERY existing shared-header form:
   base, STLP_DESTROY_DISPATCH_FORCEINLINE, STLP_DESTROY_AUX_FASTCALL, STLP_MALLOC_ALLOC_DEALLOCATE, STLP_ALLOC_PROXY_REBIND,
   AUGMENTATION_PARTICLES, STLP_FALSE_TAG_POD (removes the __false_type ctor decision, sz still 50), STLP_VECTOR +
   STLP_VECTOR_OVERFLOW_OUTOFLINE (0x008DDAD0 / 0x008DD7E0 mates), CTHING_GETTC_ORIGINAL_CHAIN, CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN,
   all mate defines together. (VECTOR_BASE_DTOR_FREE_UNTESTED not tried: retail 0x006E2500 keeps both null tests.)
   Score fit over the ~vector<PD> sites (c1 47: f1 65, f2.5 52, f12.5 45, f39.1 43): sc ~= c1 * (0.922 + 0.461/f), so inner < 65 needs
   c1 <= 46, i.e. ONE IL tuple less in ~vector<PD>'s post-inline body (its own decisions: _Destroy 34 / __destroy 36 / __destroy_aux 36 /
   _Destroy(T*) 8 / ??_G pair 33 / ~pair 8 / ~ParamDesc 5 / ~_Vector_base 14 / deallocate 18 / ~_STLP_alloc_proxy 1 call). No header form
   moves it. With the inner expanded the dtor is sz 73 / c1 67 and loop 1 = 65 (call at T65), sky 56 (expand): retail's split, as main said.
   Retail 0x006E2500 lies far outside this TU's range (0x008DAxxx-0x008E2xxx), i.e. the linker took the COMDAT from another obj; a dtor
   compiled at T>65 (default 74) gives exactly retail's body and, seen at this TU's T65 sites, exactly retail's loop-1 call / sky expand.
2. LowerBound loop 2 v sky: every HOOKED field plus an equal final compare operand (sc 63 = 63) is identical (b4e 155 sends both through
   0x10791c78, not disassembled): sz 60, a 23, pen 0, sav
   [8,0,0,0,0,2,0], c0 / c22c / c1 / cE 52, b4e 155, callee-info block (ci_raw) byte-identical, k 7, S 9, T 65, d50 1, flo 6.25e8 / fhi 0.
   Only g (growth 1920 v 2304; used only in `sz > S && g + a > 9000`, far from the cap) and the site pointer differ.
   Ego_d (ref/8e18d0/egod_lines.tsv, 54 lines): GetTC lines 3316 / 3344 / 3356 = 517 / 544 / 534 B (loop 1 / loop 2 / sky; GetTC is
   FABLE_FORCEINLINE so Ego_d expands it). Loop 2 - sky = 10 B = the extra vector operator[] call (ours /Od: 42 v 31 = 11); loop 1 v loop 2
   (same text) differ by 27 B, so the remainder is presumably frame-displacement noise (UNVERIFIED; does not affect loop 2 v sky). No
   evidence of a different accessor chain before GetTC in loop 2.
3. Grade on record 2069v2073 = hand827 n5 (2026-11-07, STATUS.tsv): n2 (CVECTORMAP_INLINE_LOWERBOUND + a LOCAL ~CParticleUpdateParameters
   body + sky block in do{}while(0) = DEVICE) + loop 2 testing size() each pass; md 529 difflines (8e_M: 460). Source not kept (only n4,
   2005). Reconstruction on today's headers (tmp/n5/n5n.cpp, no /Gr): default 2181, T60 2053, T64 / T70 2085; /Gr 2309, /Gr T65 2165.
   Its sites at default T (trace tmp/n5/t.json): inner ~vector EXPANDS (65 < 74) -> dtor sz 73; loop-1 dtor 65 EXPAND (retail call),
   sky dtor 54 expand (right), LB loop 1 82 call (right), loop 2 63 EXPAND (retail call), sky 59 expand (right); no /Gr so the STL uninit
   helpers are cdecl (sz 76). So the 2069 build is 4 bytes short by coincidence: two site decisions wrong, and it uses do{}while(0).
