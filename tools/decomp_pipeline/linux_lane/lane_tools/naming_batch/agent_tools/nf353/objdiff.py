# objdiff.py <addr...>: HEAD v new object (work/obj/H_<a>.obj, N_<a>.obj, compiled at ONE path): (1) leafrel on the row's Leaf (section-name
# keyed relocations, data / relocation equality); (2) every non-debug section paired by its stable key (COMDAT: first external symbol;
# other: name + ordinal): SAME / CHANGED / DROPPED / ADDED; (3) external symbol diff: DEF dropped / added, UND dropped / added.
# Writes evidence/objdiff/<a>.txt and prints one summary line per row. Agent NF353.
import sys, os, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); os.makedirs(A + '/evidence/objdiff', exist_ok=True)
def keyed(p):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    out = {}; cnt = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        if x['ch'] & 0x1000 and k in first: key = ('COMDAT', first[k], x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key = ('SEC', x['name'], cnt[x['name']])
        out[key] = x
    return out
for a in sys.argv[1:]:
    H, N = A + '/work/obj/H_%s.obj' % a, A + '/work/obj/N_%s.obj' % a
    leaf = nf.leaf_of(open(A + '/rows/%s.cpp' % a, encoding='utf-8').read(), a)
    lr = subprocess.run(['python3', A + '/tools/leafrel.py', H, N, leaf], capture_output=True, text=True); lrt = (lr.stdout + lr.stderr).strip()
    kh, kn = keyed(H), keyed(N); rep = []; ch = []
    for k in sorted(set(kh) | set(kn), key=str):
        if k not in kn: rep.append('DROPPED %s %s' % (k[0], k[1])); continue
        if k not in kh: rep.append('ADDED %s %s' % (k[0], k[1])); continue
        x, y = kh[k], kn[k]
        if x['data'] != y['data'] or x['ch'] != y['ch'] or [(o, s, t) for o, s, t in x['rel'] if not s.startswith('$L')] != [(o, s, t) for o, s, t in y['rel'] if not s.startswith('$L')] or len(x['rel']) != len(y['rel']):
            ch.append(k); rep.append('CHANGED %s %s %s' % (k[0], k[1], k[2]))
    sh, sn = nf.ext_syms(H), nf.ext_syms(N)
    sd = ['%s %s' % (('DROP ' if x in sh else 'ADD  ') + x[:3], x[4:]) for x in sorted(sh ^ sn)]
    # a symbol that is DEF in H and UND in N counts as DROP DEF + ADD UND
    leafok = lrt.startswith('data_equal True relocs_equal True')
    nonleaf = [k for k in ch if not (k[0] == 'COMDAT' and k[1] == leaf)]
    open(A + '/evidence/objdiff/%s.txt' % a, 'w').write('# leafrel %s\n%s\n# sections (non-debug, stable keys)\n%s\n# external symbols HEAD v new\n%s\n' % (leaf, lrt, '\n'.join(rep) or 'ALL_SAME', '\n'.join(sd) or 'NONE'))
    print('%s leaf=%s changed_sections=%d dropped_sections=%d added_sections=%d dropDEF=%d addDEF=%d dropUND=%d addUND=%d' % (
        a, 'SAME' if leafok else 'DIFF', len(ch), sum(r.startswith('DROPPED') for r in rep), sum(r.startswith('ADDED') for r in rep),
        sum(x.startswith('DROP DEF') for x in sd), sum(x.startswith('ADD  DEF') for x in sd), sum(x.startswith('DROP UND') for x in sd), sum(x.startswith('ADD  UND') for x in sd)))
