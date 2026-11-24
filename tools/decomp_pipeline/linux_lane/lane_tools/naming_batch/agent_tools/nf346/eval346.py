# eval346.py <addr file>: per row, from the pass logs (scratch grade HEAD / new), pkcmp346 (both checkers) and objsec346 (needs the
# pk TSVs in evidence/pk): writes work/eval.tsv  addr  class  gradeH  gradeN  fixed_getdef  remain_getdef  remain_subdef  objsec  detail
# class: ACCEPT (grade same and MATCH/RELOCATION_MATCH, gate P, >=1 GetDef copy fixed, no leaf-called NEQ GetDef copy left, objsec OK)
#        PARTIAL (as ACCEPT but a leaf-called NEQ GetDef copy is left, unchanged by the define)
#        NOFIX (no GetDef copy fixed), BAD_OBJSEC, GATE_FAIL, GRADE.  Objsec text goes to evidence/objsec/<addr>.txt.
import sys, os, re, subprocess, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); T = A + '/tools/'
gr = {}
for p in glob.glob(A + '/work/pass_*.log'):
    for l in open(p):
        m = re.match(r'([HN]) ([0-9a-f]{8}) (\S+) ', l)
        if m: gr[(m.group(1), m.group(2))] = m.group(3)
os.makedirs(A + '/evidence/objsec', exist_ok=True)
out = open(A + '/work/eval.tsv', 'a')
for a in [l.strip() for l in open(sys.argv[1]) if l.strip()]:
    gh, gn = gr.get(('H', a), '-'), gr.get(('N', a), '-')
    pc = subprocess.run(['python3', T + 'pkcmp346.py', A + '/evidence/pk', a], capture_output=True, text=True).stdout
    m = re.search(r'^ROW \S+ gate=(\w) fixed_getdef=(\d+) fixed_subdef=(\d+) remain_getdef=(\d+) remain_subdef=(\d+)(.*)$', pc, re.M)
    if not m: out.write('%s\tGATE_FAIL\t%s\t%s\t-\t-\t-\t-\tpkcmp: %s\n' % (a, gh, gn, pc.strip()[:200])); out.flush(); continue
    gate, fx, fxs, rem, rems, extra = m.group(1), int(m.group(2)), int(m.group(3)), int(m.group(4)), int(m.group(5)), m.group(6).strip()
    pvr_rem = len([l for l in pc.splitlines() if '\tpvr\tREMAIN_NEQ\t' in l and '??$GetDef@' in l])
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    os_ = subprocess.run(['python3', T + 'objsec346.py', A + '/work/obj/H_%s.obj' % a, A + '/work/obj/%s.obj' % a, A + '/evidence/pk', a, leaf], capture_output=True, text=True)
    ot = os_.stdout + os_.stderr; open(A + '/evidence/objsec/%s.txt' % a, 'w').write(ot)
    ov = ot.split()[1] if len(ot.split()) > 1 else 'ERR'
    gsame = gh == gn and gh in ('MATCH', 'RELOCATION_MATCH')
    if not gsame: c = 'GRADE'
    elif gate != 'P': c = 'GATE_FAIL'
    elif ov == 'BAD' or ov == 'ERR' or ov == 'SUBDEF': c = 'BAD_OBJSEC'
    elif fx == 0: c = 'NOFIX'
    elif rem or pvr_rem: c = 'PARTIAL'
    else: c = 'ACCEPT'
    det = ' | '.join(l.strip() for l in ot.splitlines()[1:] if not l.strip().startswith(('HELPER_', 'GETDEF_EQ')))[:400]
    out.write('%s\t%s\t%s\t%s\t%d\t%d\t%d\t%s\t%s%s\n' % (a, c, gh, gn, fx, rem, rems, ot.splitlines()[0][9:].split(' leaf=')[0] if ot else 'ERR', det, (' ' + extra) if extra else '')); out.flush()
print('EVAL_DONE')
