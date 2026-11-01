# compose18_steps.py (exec'd by compose18.py): the wave-18 families after the hand stages.
# glob families: fam_glob18.py per data target (the class-static accessor respells); rows = runs/q/q_<t>.rows.txt (+ the rest lists
# are subsets) minus runs/q/excl_<t>.txt (rows whose recheck failed: DEFERRED). The behaviour tests that defined the old stand-in
# follow in the same family (h18_gtests.py: 0x00892F10 for pclass; h18_gtests2.py: the 16 CGameScriptInterface rows for pgui).
GLOB = [('pmgc', '013b86a0'), ('pclass', '013b89fc'), ('dstgame', '0143e8f8'), ('pgui', '013b8790'), ('thingmgr', '013b8a1c'), ('cfps', '01375550')]
for _fam, _t in GLOB:
    _x = os.path.join(RUNS, 'q', 'excl_%s.txt' % _t)
    _X = set(l.split()[0] for l in open(_x) if l.strip()) if os.path.exists(_x) else set()
    _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
    open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
    STEPS.append(Fm(_fam, ['fam_glob18.py', out, _t, 'q/in_q_%s.rows.txt' % _t, 'g' + _t]))
    if _fam == 'pclass': STEPS.append(H('pclass', 'h18_gtests.py'))
    if _fam == 'pgui': STEPS.append(H('pgui', 'h18_gtests2.py')); STEPS.append(H('pgui', 'h18_gtests3.py'))
    if _fam == 'thingmgr': STEPS.append(H('thingmgr', 'h18_thingmgr_tests.py'))
# eps: the DELTA_NOTIONAL_ZERO respell (coordinator update 2026-11-01: 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB`, was EPSILON) + the one test
STEPS.append(Fm('eps', ['fam_eps18.py', out, 'q/q_eps.rows.txt', 'eps'])); STEPS.append(H('eps', 'h18_eps_test.py'))
# bitset: std::bitset<274>::test (0x00410DE0) through the row-local stand-in classes
STEPS.append(Fm('bitset', ['fam_bitset18.py', out, 'q/q_00410de0.rows.txt', 'bitset']))
# gpdm / isqrt: cg10 data globals renamed (same type): ?GPDisplayManager@@3PAVCDisplayManager@@A, ?GFastOneOverSqrtTable@@3PAKA
STEPS.append(Fm('gpdm', ['fam_gren18.py', out, 'q/q_gpdm.rows.txt', 'gpdm', '?PDisplayManager@@3PAVCDisplayManager@@A', '?GPDisplayManager@@3PAVCDisplayManager@@A', 'PDisplayManager', 'GPDisplayManager', 'ChatGPT pass 10 audit, raw Ego_r public']))
STEPS.append(H('gpdm', 'h18_gpdm_test.py'))
STEPS.append(Fm('isqrt', ['fam_gren18.py', out, 'q/q_isqrt.rows.txt', 'isqrt', '?g_InvSqrtMantissaTable@@3PAKA', '?GFastOneOverSqrtTable@@3PAKA', 'g_InvSqrtMantissaTable', 'GFastOneOverSqrtTable', 'ChatGPT pass 10 audit, raw Ego_r public']))
# the generic queue family: one fam_q17 step per target in runs/qtargets18.tsv (target, final symbol, script); rows =
# runs/q/q_<t>.rows.txt (spellu) minus runs/excl_queue.txt
_X = set(l.split()[0] for l in open(os.path.join(RUNS, 'excl_queue.txt')) if l.strip()) if os.path.exists(os.path.join(RUNS, 'excl_queue.txt')) else set()
if os.path.exists(os.path.join(RUNS, 'qtargets18.tsv')):
    for _l in open(os.path.join(RUNS, 'qtargets18.tsv')):
        if not _l.strip() or _l.startswith('#'): continue
        _t, _fin, _scr = _l.rstrip('\n').split('\t')[:3]
        _p = os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)
        if not os.path.exists(_p): continue
        _b = [l.split()[0] for l in open(_p) if l.strip()]
        open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
        STEPS.append(Fm('queue', [_scr, out, _t, _fin, 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
