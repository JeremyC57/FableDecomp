# resolve.py <addr...>: per row, every external reference the new object adds (UND in N_<a>.obj, not UND in H_<a>.obj) must be DEFINED,
# under the exact same string, by a landed owner object compiled from HEAD (owners: work/obj/H_00cd3d2e.obj, work/cobj/009ac700.obj),
# whose own grade is MATCH / RELOCATION_MATCH and whose // Leaf: line (its graded symbol) is that exact string. rebuild/oracles/self_symbols.tsv is
# NOT used: at HEAD it still carries 0x00CD3D2E's old 3-argument stand-in and has no 0x009AC700 line (stale generated table). Writes evidence/resolve/<a>.txt and
# prints `<a> RESOLVE PASS|FAIL n_added`. The batch /FORCE link (nf.py link) is the second half of the test. Agent NF353.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = str(nf.R); os.makedirs(A + '/evidence/resolve', exist_ok=True)
OWN = {'00cd3d2e': A + '/work/obj/H_00cd3d2e.obj', '009ac700': A + '/work/cobj/009ac700.obj'}
grade = {}
for p in (A + '/work/grades_H.log', A + '/work/census_grades.log'):
    for l in open(p):
        m = re.match(r'[HC] ([0-9a-f]{8}) (\S+)', l)
        if m and m.group(2) not in ('PKLEAF',): grade[m.group(1)] = m.group(2)
selfs = {o: (nf.leaf_of(nf.src_of(o).read_text(encoding='utf-8'), o),) for o in OWN}
odef = {o: {x[4:] for x in nf.ext_syms(p) if x.startswith('DEF ')} for o, p in OWN.items()}
for a in sys.argv[1:]:
    h, n = nf.ext_syms(A + '/work/obj/H_%s.obj' % a), nf.ext_syms(A + '/work/obj/N_%s.obj' % a)
    hn = {x[4:] for x in h}
    added = sorted(x[4:] for x in n if x.startswith('UND ') and ('UND ' + x[4:]) not in h)
    lines = []; ok = True
    for s in added:
        own = [o for o in OWN if s in odef[o]]
        good = [o for o in own if grade.get(o) in ('MATCH', 'RELOCATION_MATCH') and selfs.get(o, ('',))[0] == s]
        was_def = s in hn
        lines.append('%s\tADDED_UND\t%s\towner=%s\towner_grade=%s\towner_Leaf=%s\twas_DEF_at_HEAD=%s' % (
            'OK' if good else 'NO_EQ_OWNER', s, ','.join(own) or '-', ','.join(grade.get(o, '-') for o in own) or '-',
            ','.join('SAME' if selfs.get(o, ('',))[0] == s else 'OTHER' for o in own) or '-', 'y' if was_def else 'n'))
        ok = ok and bool(good)
    open(A + '/evidence/resolve/%s.txt' % a, 'w').write('# resolve test (string equality against the landed owner object compiled from HEAD; owner graded; owner Leaf line)\n' + ('\n'.join(lines) or 'NO_ADDED_REFERENCE') + '\n')
    print('%s RESOLVE %s %d %s' % (a, 'PASS' if ok and added else 'FAIL', len(added), ' '.join('%s->%s' % (l.split('\t')[2][:24], l.split('\t')[3]) for l in lines)))
