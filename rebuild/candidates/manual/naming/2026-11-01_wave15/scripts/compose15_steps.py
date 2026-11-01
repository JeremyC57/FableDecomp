# compose15_steps.py (exec'd by compose15.py): the generic queue family -- one fam_q.py step per queue target whose trial
# edited rows (runs/q_active.txt), rows = runs/q/q_<t>.rows.txt minus runs/excl_queue.txt, in runs/qtargets2.tsv order.
_FIN = dict(l.rstrip('\n').split('\t') for l in open(os.path.join(RUNS, 'qtargets2.tsv')) if l.strip())
_ACT = [l.strip() for l in open(os.path.join(RUNS, 'q_active.txt')) if l.strip()]
_X = set(l.split()[0] for l in open(os.path.join(RUNS, 'excl_queue.txt')) if l.strip()) if os.path.exists(os.path.join(RUNS, 'excl_queue.txt')) else set()
for _t in [l.split('\t')[0] for l in open(os.path.join(RUNS, 'qtargets2.tsv')) if l.strip()]:
    if _t not in _ACT: continue
    _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % _t)) if l.strip()]
    open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % _t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
    STEPS.append(Fm('queue', ['fam_q.py', out, _t, _FIN[_t], 'q/in_q_%s.rows.txt' % _t, 'q' + _t]))
# own-row leaf respells (fam_leaf.py; also renames the old declaration in the row's test), rows in runs/leaf_jobs.tsv minus excl_leaf
_LX = set(l.split()[0] for l in open(os.path.join(RUNS, 'excl_leaf.txt')) if l.strip()) if os.path.exists(os.path.join(RUNS, 'excl_leaf.txt')) else set()
open(os.path.join(RUNS, 'in_leaf_jobs.tsv'), 'w').write(''.join(l for l in open(os.path.join(RUNS, 'leaf_jobs.tsv')) if l.strip() and l.split('\t')[0] not in _LX))
STEPS.append(Fm('leaf', ['fam_leaf.py', out, 'in_leaf_jobs.tsv']))
STEPS += [H('leaf', 'h15_leaf2.py'), H('leaf', 'h15_leaf3.py')]   # own rows fam_leaf skipped (types / virtual)
# test stubs of rows whose callee the families respelled (after every source stage)
STEPS.append(H('tests', 'h15_tests.py'))
