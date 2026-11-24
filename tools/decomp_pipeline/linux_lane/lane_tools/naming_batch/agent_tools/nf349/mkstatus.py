# mkstatus.py: STATUS.tsv, one line per candidate row (the 104 compiled) and per excluded row of the D3 source list / GLOBAL_BODY holes.
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A)
ev = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/work/eval.tsv')}
rc = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/evidence/d3_recount.tsv')}
bat = {}
for b in ('b1', 'b2', 'b3', 'p1'):
    for a in open(A + '/batches/%s/rows.txt' % b).read().split(): bat[a] = b
RET = {'0042af3c': 98, '0042b040': 98, '0042b312': 98, '005f81be': 98}
def nm(s): return re.match(r'\?\?\$GetDef@V?(.*?)@@@', s).group(1)
out = ['address\tstatus\tbatch\thead_neq_getdef\tfixed\tleft\treason']
for a in sorted(ev):
    e = ev[a]; r = rc[a]; c = e[1]
    ot = open(A + '/evidence/objsec/%s.txt' % a).read()
    if c == 'ACCEPT': st, why = 'done', 'REREAD define: every leaf-called NEQ GetDef<T> copy now EQ (both checkers); grade unchanged; objsec leaf identical (%s)' % e[7]
    elif c == 'PARTIAL':
        st = 'held-partial'
        why = 'define fixes %s of %s NEQ GetDef copies (EQ, both checkers; objsec OK, grade unchanged); left unchanged by the define: %s = the GetDef(const CCharString&, CDefPointer*) overload copy, 112 B v retail 104 B (NEQ at HEAD, byte-identical after; debt). Ruling Q1' % (r[3], r[2], r[6])
    else:
        st = 'skipped'
        bits = []
        for m in re.finditer(r'GETDEF_NEQ :: changed COMDAT (\S+) (\d+)->(\d+) B', ot):
            sym = m.group(1); ad = [k for k in RET if ('V' + nm(sym)) in open(A + '/evidence/pk/N_%s.pkall.tsv' % a).read() and False]
            bits.append('GetDef<%s> %s->%s B still NEQ' % (nm(sym), m.group(2), m.group(3)))
        if 'LEAF ::' in ot: bits.append('the leaf section changes too')
        eqs = re.findall(r'GETDEF_EQ :: changed COMDAT (\S+)', ot)
        why = ('the define leaves a copy NEQ: %s; retail\'s CThingBaseDef 0x0042AF3C / CThingObjectDef 0x0042B040 / CUIMiscThingsDef 0x0042B312 / CExpressionDef 0x005F81BE copies are 98 B, '
               'the size-mode form (frame pointer, `and [esi],0`) that only rows under `#pragma optimize("s", on)` emit (the 18 copies of those four that turn EQ here are all in rows carrying that pragma); this row compiles them at /O2 /Oy, so a second device would be needed (not invented, brief)%s') % (
               '; '.join(bits), ('; it would also fix (not ported): ' + ', '.join('GetDef<%s>' % nm(s) for s in eqs)) if eqs else '')
    out.append('%s\t%s\t%s\t%s\t%s\t%s\t%s' % (a, st, bat.get(a, '-'), r[2], r[3], r[4], why))
EX = [('008f7950', 'excluded: done in batches/2026-11-24_landing_nf341_r2 (REREAD landed)'), ('00703150', 'excluded: done in nf341_r2'), ('007f0300', 'excluded: done in nf341_r2'),
      ('0082d5c0', 'excluded: done in nf341_r2'), ('00839940', 'excluded: Account B (COORDINATION_CLAIMS B claimed, UNMATCHED reserved acctB B24B, pending acctB_b24 staged); landed source already defines REREAD'),
      ('00821a80', 'excluded: already defines REREAD at HEAD; its leaf-called GetDef<CDefClassBase> 0x0042B144 copy is 108 B NEQ (catalog 108) with the define (HEAD object compiled read-only, evidence/pk_x): copy left, not fixable by this define'),
      ('00607330', 'excluded: no GETDEF_GLOBAL_BODY #define (comment only); its one NF341 NEQ copy is GetSubDef<CAppearanceDef> 0x0042B0A2, not a GetDef copy'),
      ('00584b86', 'excluded hole (not compiled, uncounted): T3 column 2 nf307'), ('005d9aa0', 'excluded hole: T3 column 2 nf302'), ('006ac200', 'excluded hole: T3 column 2 nf303 + codex RESERVED'),
      ('007eac10', 'excluded hole: T3 column 2 nf303'), ('007fc210', 'excluded hole: T3 column 2 nf303'), ('00801920', 'excluded hole: T3 column 2 nf307'), ('0082d2d0', 'excluded hole: T3 column 2 nf314'),
      ('0040c790', 'excluded hole: T3 column 3 nf297 (also already defines REREAD)'), ('007bd2d1', 'excluded hole: Account B claim (claimed)'), ('00859ef0', 'excluded hole: Account B claim (claimed)'),
      ('00591b7f', 'excluded hole: newest claim is B\'s (done) - COORDINATION section 3'), ('00774d20', 'excluded hole: newest claim is B\'s (done) - COORDINATION section 3')]
for a, w in EX: out.append('%s\texcluded\t-\t-\t-\t-\t%s' % (a, w))
open(A + '/STATUS.tsv', 'w').write('\n'.join(out) + '\n'); print(len(out) - 1)
