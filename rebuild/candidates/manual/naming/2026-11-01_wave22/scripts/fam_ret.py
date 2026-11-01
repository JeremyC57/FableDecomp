# fam_ret.py <ov> <spec.tsv>: wave 14 queue (c651) -- return / parameter type respells of a member declared in a row-local class
# named as the PDB class. spec.tsv: target, final, old, member name, old declaration regex, new declaration, enum (or -).
# The enum (Ego_r name) is declared at global scope before the class when the row has none (`enum E { };`, values unused: the
# rows compare the result with integer literals, which an enum converts to unchanged). check: exactly old -> final.
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, acclib, f2m
E, IX = inputs.index()
SP = collections.defaultdict(list)
for l in open(sys.argv[2]):
    if not l.strip() or l.startswith('#'): continue
    q = l.rstrip('\n').split('\t')
    for a in E.get(q[2], ()): SP[a].append(q)
def tr(t, a):
    msgs = []
    for tg, fin, old, name, orx, nd, en in SP[a]:
        cls = re.match(r'^\?\w+@(\w+)@@', fin).group(1)
        c = f2m.code_only(t)
        ms = [x for x in re.finditer(orx, c) if (acclib.class_span(c, x.start()) or (0, ''))[1] == cls]
        if len(ms) != 1: return None, '%s: %d declarations in %s' % (name, len(ms), cls)
        m = ms[0]; t = t[:m.start()] + m.expand(nd) + t[m.end():]
        if en != '-' and not re.search(r'\benum\s+' + en + r'\b', f2m.code_only(t)):
            k = acclib.class_span(f2m.code_only(t), m.start())
            cm = list(re.finditer(r'\b(class|struct)\s+' + cls + r'\b[^;{()]*\{', t))
            pos = [x.start() for x in cm if x.start() <= m.start()][-1]
            ls = t.rfind('\n', 0, pos) + 1
            t = t[:ls] + 'enum %s { };   // Ego_r.pdb enum (values unused here)\n' % en + t[ls:]
        msgs.append(name)
    return t, 'ret ' + ' '.join(msgs)
def ck(S0, S1, a):
    olds = {q[2] for q in SP[a]}; fins = {q[1] for q in SP[a]}
    return F.diffcheck(S0, S1, lambda s: s in olds, lambda s: s in fins, sorted(fins))
F.run(sys.argv[1], sorted(SP), tr, ck, 'ret')
