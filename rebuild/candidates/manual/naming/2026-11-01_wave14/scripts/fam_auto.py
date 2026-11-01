# fam_auto.py <ov> <pairs.tsv> [rows.txt]: access / const-only respellings of a member declaration (and of its out-of-line
# definition when the row defines it) inside a row-local class whose name is the PDB class. pairs.tsv: target final old
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, acclib
MEM = re.compile(r'^\?(\w+)@(\w+)@@([AIQ])([AB])E(.*)$')
ACC = {'Q': 'public', 'I': 'protected', 'A': 'private'}
E, R = inputs.index()
P = collections.defaultdict(list)
for l in open(sys.argv[2]):
    if not l.strip() or l.startswith('#'): continue
    t, fin, old = l.split()[:3]
    for a in E.get(old, ()): P[a].append((t, fin, old))
rows = F.rows_from(sys.argv[3]) if len(sys.argv) > 3 else sorted(P)
def tr(t, a):
    msgs = []
    for tg, fin, old in P[a]:
        m = MEM.match(fin); name, cls, acc, cv = m.group(1), m.group(2), m.group(3), m.group(4)
        mo = MEM.match(old)
        rx = r'(?<![\w:~.>])(?:[A-Za-z_][^;{}()\n]*?[\s\*&])?%s\s*\([^;{}]*\)\s*(?:const\s*)?;' % name
        ms = [x for x in re.finditer(rx, t) if (acclib.class_span(t, x.start()) or (0, ''))[1] == cls]
        if len(ms) != 1: return None, '%s: %d declarations in %s' % (name, len(ms), cls)
        d = ms[0].group(0)
        nd = re.sub(r'\)\s*(const\s*)?;$', ')' + (' const' if cv == 'B' else '') + ';', d)
        r = acclib.respell_decl(t, re.escape(d), nd, ACC[acc])
        if r[0] is None: return None, '%s: %s' % (name, r[1])
        t = r[0]
        if cv != mo.group(4):
            def fix(mm):
                head = re.sub(r'\)\s*const$', ')', mm.group(1))
                return head + (' const' if cv == 'B' else '') + mm.group(2)
            t = re.sub(r'(\b%s::%s\s*\([^;{}]*\)(?:\s*const)?)(\s*\{)' % (cls, name), fix, t)
        t = t.replace('Leaf: ' + old, 'Leaf: ' + fin)
        msgs.append('%s %s->%s' % (name, old[len(name) + len(cls) + 3:][:3], fin[len(name) + len(cls) + 3:][:3]))
    return t, ' '.join(msgs)
def ck(S0, S1, a):
    olds = {o for _, _, o in P[a]}; fins = {f for _, f, _ in P[a]}
    return F.diffcheck(S0, S1, lambda s: s in olds, lambda s: s in fins, sorted(fins))
F.run(sys.argv[1], rows, tr, ck, 'auto')
