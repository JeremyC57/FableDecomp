# eval349.py <addr file>: per row -> work/eval.tsv (rewritten)
#   addr class gradeH gradeR gradeN fixed_getdef remain_getdef remain_subdef objsecHN objsecRN pragscan detail
# Needs: pass logs (work/pass_*.log), pk files H_/R_/N_ in evidence/pk, evidence/objsec (H v N) and evidence/objsec_RN (R v N) from
# objsec349.sh, evidence/pragscan_c.txt. class:
#   ACCEPT  scratch grade H == N (MATCH / RELOCATION_MATCH); pkcmp346 H v N gate P (0 turned / newly NEQ leaf-called, both checkers);
#           >= 1 GetDef copy fixed; no leaf-called NEQ GetDef copy left (pk_all and pvr); objsec H v N OK (leaf identical, only GETDEF_EQ /
#           NOADDR SetDynamicCast helper bodies / dropped NOADDR helpers); objsec R v N: only the opted-in size-mode GetDef<T> COMDATs differ
#           (GETDEF_EQ), symbol tables and COMDAT selections equal; pragscan C_OK.
#   LEAF_REREAD  0x005E5F30 / 0x006096E0 (brief: out of every batch; REREAD moves the leaf)
#   GRADE / GATE_FAIL / BAD_OBJSEC / BAD_RN / PARTIAL / NOFIX / PRAG otherwise.
import sys, os, re, subprocess, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); T = A + '/tools/'
OUT = {'005e5f30', '006096e0'}
TYP = {'CTHINGBASEDEF': 'VCThingBaseDef@@', 'CTHINGOBJECTDEF': 'VCThingObjectDef@@', 'CUIMISCTHINGSDEF': 'VCUIMiscThingsDef@NUISystem@@', 'CEXPRESSIONDEF': 'VCExpressionDef@@'}
gr = {}
for p in glob.glob(A + '/work/pass_*.log'):
    for l in open(p):
        m = re.match(r'([HN]) ([0-9a-f]{8})(_r)? (\S+) ', l)
        if m: gr[('R' if m.group(3) else m.group(1), m.group(2))] = m.group(4)
prag = {l.split()[0]: l.split()[1] for l in open(A + '/evidence/pragscan_c.txt') if l.strip()}
rows = ['addr\tclass\tgradeH\tgradeR\tgradeN\tfixed_getdef\tremain_getdef\tremain_subdef\tobjsecHN\tobjsecRN\tpragscan\tdetail']
for a in [l.strip() for l in open(sys.argv[1]) if l.strip()]:
    gh, grr, gn = gr.get(('H', a), '-'), gr.get(('R', a), '-'), gr.get(('N', a), '-')
    pc = subprocess.run(['python3', T + 'pkcmp346.py', A + '/evidence/pk', a], capture_output=True, text=True).stdout
    m = re.search(r'^ROW \S+ gate=(\w) fixed_getdef=(\d+) fixed_subdef=(\d+) remain_getdef=(\d+) remain_subdef=(\d+)(.*)$', pc, re.M)
    hn = open(A + '/evidence/objsec/%s.txt' % a).read(); rn = open(A + '/evidence/objsec_RN/%s.txt' % a).read()
    hv = hn.split()[1] if len(hn.split()) > 1 else 'ERR'; rv = rn.split()[1] if len(rn.split()) > 1 else 'ERR'
    want = set(TYP[x] for x in re.findall(r'^#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_(\w+)_SIZE_BODY', open(A + '/rows/%s.cpp' % a).read(), re.M))
    rnitems = [l.strip() for l in rn.splitlines()[1:] if l.strip()]
    rn_ok = rv == 'OK' and all(i.startswith('GETDEF_EQ :: changed COMDAT ??$GetDef@') for i in rnitems) and \
            set(re.search(r'\?\?\$GetDef@(\S+?@@)@CDefinitionManager', i).group(1) for i in rnitems) == want
    if not m: c, fx, rem, rems, extra = 'GATE_FAIL', 0, 0, 0, pc.strip()[:200]
    else:
        gate, fx, rem, rems, extra = m.group(1), int(m.group(2)), int(m.group(4)), int(m.group(5)), m.group(6).strip()
        pvr_rem = len([l for l in pc.splitlines() if '\tpvr\tREMAIN_NEQ\t' in l and '??$GetDef@' in l])
        if a in OUT: c = 'LEAF_REREAD'
        elif not (gh == gn and gh in ('MATCH', 'RELOCATION_MATCH')): c = 'GRADE'
        elif gate != 'P': c = 'GATE_FAIL'
        elif hv not in ('OK',): c = 'BAD_OBJSEC'
        elif not rn_ok: c = 'BAD_RN'
        elif prag.get(a) != 'C_OK': c = 'PRAG'
        elif fx == 0: c = 'NOFIX'
        elif rem or pvr_rem: c = 'PARTIAL'
        else: c = 'ACCEPT'
    det = ' | '.join(l.strip() for l in hn.splitlines()[1:] if not l.strip().startswith(('HELPER_', 'GETDEF_EQ')))[:300]
    rows.append('%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d\t%s\t%s\t%s\t%s%s' % (a, c, gh, grr, gn, fx, rem, rems, hn.splitlines()[0][9:].split(' leaf=')[0] if hn else 'ERR',
                rn.splitlines()[0][9:].split(' leaf=')[0] if rn else 'ERR', prag.get(a, '-'), det, (' ' + extra) if extra else ''))
open(A + '/work/eval.tsv', 'w').write('\n'.join(rows) + '\n')
print('EVAL_DONE', len(rows) - 1)
