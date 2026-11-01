# compose19_steps.py (exec'd by compose19.py): the wave-19 driver families after the hand stages, in commit order.
#   stools (scope 1): fam_stools19 over the PeekThingSearchTools rows; gif (scope 2, c684): fam_gif19 over the CGraphicInfoFrame rows;
#   c685 (scope 2): fam_q18 over the c685 queue targets with a generic final; xfer / cmp (scope 4): the Transfer<T> and
#   CBasicString::Compare mega families. Rows = runs/q/q_<t>.rows.txt minus runs/q/excl_<t>.txt (recheck failures: DEFERRED).
def _rows(t):
    _x = os.path.join(RUNS, 'q', 'excl_%s.txt' % t)
    _X = set(l.split()[0] for l in open(_x) if l.strip()) if os.path.exists(_x) else set()
    _b = [l.split()[0] for l in open(os.path.join(RUNS, 'q', 'q_%s.rows.txt' % t)) if l.strip()]
    open(os.path.join(RUNS, 'q', 'in_q_%s.rows.txt' % t), 'w').write(''.join(a + '\n' for a in _b if a not in _X))
    return 'q/in_q_%s.rows.txt' % t
STEPS.insert(1, Fm('stools', ['fam_stools19.py', out, _rows('008fd000'), 'stools']))
STEPS.insert(2, H('stools', 'h19_sfix.py'))   # the 0x007FD110 filter-name fix (3 more stools rows + 00972830)
STEPS.insert(4, Fm('gif', ['fam_gif19.py', out, _rows('009fe380'), 'gif']))
STEPS.insert(5, H('gif', 'h19_own_gif.py'))   # the chunk-684 own row 009ff6b0 (h19_own.py under OWN_FAM=gif)
_i = [i for i, s in enumerate(STEPS) if s[1] == 'c685'][-1] + 1
STEPS.insert(_i, H('c685', 'h19_own.py')); _i += 1   # the chunk-685 own rows (00ab3640 0099f560 00a89320 00a89350 00ab1480)
for _t, _fin in (('007748e0', '?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z'), ('009cc240', '?GetEntryDataSize@CBankFile@@QBEKK@Z'),
                 ('00bec680', '?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@K@Z')):
    STEPS.insert(_i, Fm('c685', ['fam_q18.py', out, _t, _fin, _rows(_t), 'q' + _t])); _i += 1
STEPS.append(Fm('xfer', ['fam_xfer19.py', out, _rows('00431102'), 'xfer']))
STEPS.append(Fm('cmp', ['fam_cmp19.py', out, _rows('00411570'), 'cmp']))
