# compose17_steps.py (exec'd by compose17.py): the generic queue family -- one fam_q / fam_q17 step per queue target in
# runs/qtargets17.tsv (target, final symbol, script), rows = runs/q/q_<t>.rows.txt minus runs/excl_queue.txt (rows whose recheck
# failed or that need re-authoring: DEFERRED).
_X = set(l.split()[0] for l in open(os.path.join(RUNS, 'excl_queue.txt')) if l.strip()) if os.path.exists(os.path.join(RUNS, 'excl_queue.txt')) else set()
for _l in open(os.path.join(RUNS, 'qtargets17.tsv')):
    if not _l.strip() or _l.startswith('#'): continue
    _t, _fin, _scr = _l.rstrip('\n').split('\t')[:3]
    _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
    open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
    STEPS.append(Fm('queue', [_scr, out, _t, _fin, 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
# scope 2 (wave 16 DEFERRED, per-row): NGraphicAppearance namespace (004bfac0 callers), CWillButtonHelperTimer / FreeMemoryPage callers,
# the 0x00404720 own row (+ its test) and 0x009AE150
STEPS.append(H('w16d', 'h17_ns.py')); STEPS.append(H('w16d', 'h17_w16d.py')); STEPS.append(H('w16d', 'h17_sp.py'))
# the cg10 queue rows (ChatGPT naming queue appended by the coordinator): one fam_q step per target that edited a row in the ov_cg
# trial (runs/qtargets_cg17.tsv), run after scope 2 as in the trial; rows minus runs/excl_queue.txt
if os.path.exists(os.path.join(RUNS, 'qtargets_cg17.tsv')):
    for _l in open(os.path.join(RUNS, 'qtargets_cg17.tsv')):
        if not _l.strip() or _l.startswith('#'): continue
        _t, _fin, _scr = _l.rstrip('\n').split('\t')[:3]
        _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
        open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
        STEPS.append(Fm('cg10', [_scr, out, _t, _fin, 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
# the c678 queue rows (appended by the coordinator during the wave): caller-only fam_q targets that passed the ov_c678t trial
if os.path.exists(os.path.join(RUNS, 'qtargets_c678_17.tsv')):
    for _l in open(os.path.join(RUNS, 'qtargets_c678_17.tsv')):
        if not _l.strip() or _l.startswith('#'): continue
        _t, _fin, _scr = _l.rstrip('\n').split('\t')[:3]
        _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
        open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
        STEPS.append(Fm('c678', [_scr, out, _t, _fin, 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
# the c681 queue rows (appended during the wave): CMemoryDataOutputStream PeekData / GetLength caller respells (the c681 ??_G rows
# are in family gdt: h17_gdt.py takes the c678 / c681 ??_G rows too)
if os.path.exists(os.path.join(RUNS, 'qtargets_c681_17.tsv')):
    for _l in open(os.path.join(RUNS, 'qtargets_c681_17.tsv')):
        if not _l.strip() or _l.startswith('#'): continue
        _t, _fin, _scr = _l.rstrip('\n').split('\t')[:3]
        _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
        open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
        STEPS.append(Fm('c681', [_scr, out, _t, _fin, 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
