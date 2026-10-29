# fam_nga9.py <ov> <rows.txt>: NGraphicAppearance::CTCGraphicAppearance (Ego_r.pdb + FableWin.pdb: every CTCGraphicAppearance
# member is in namespace NGraphicAppearance, e.g. ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z 0x004BFAC0).
# Rows declared the class at global scope. Transform: the class definition is wrapped in `namespace NGraphicAppearance { }`
# + `using NGraphicAppearance::CTCGraphicAppearance;`. Kept only when every CTCGraphicAppearance symbol the object then names
# is an exact Ego_r/FableWin spelling (no intermediate stand-in spellings).
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hand'))
import famlib9, inputs
from he import ns_chain, block_end
NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
def tf(t, a):
    ms = list(re.finditer(r'\b(class|struct)\s+CTCGraphicAppearance\b[^;{]*\{', t))
    if len(ms) != 1: return None, 'CTCGraphicAppearance defined %d times' % len(ms)
    m = ms[0]
    if ns_chain(t, m.start()): return None, 'not at global scope'
    s = m.start(); e = block_end(t, s)
    if m.group(1) == 'struct':   # PDB key is class (V): `class X : public B { public:`
        hdr = t[m.start():m.end() - 1]; rest = hdr.split('CTCGraphicAppearance', 1)[1]
        if rest.strip().startswith(':'):
            bl = rest.split(':', 1)[1]
            rest = ' : ' + ', '.join(('public ' + x.strip()) if not re.match(r'\s*(public|private|protected)\b', x) else x.strip() for x in bl.split(',')) + ' '
        nh = 'class CTCGraphicAppearance' + rest + '{ public:'
        t = t[:m.start()] + nh + t[m.end():]; e = block_end(t, s)
        t = re.sub(r'\bstruct(\s+CTCGraphicAppearance\s*;)', r'class\1', t)
    fw = re.search(r'\b(?:class|struct)\s+CTCGraphicAppearance\s*;', t[:s])
    t = t[:s] + 'namespace NGraphicAppearance {\n' + t[s:e] + '\n}\nusing NGraphicAppearance::CTCGraphicAppearance;' + t[e:]
    if fw: t = t[:fw.start()] + 'namespace NGraphicAppearance { class CTCGraphicAppearance; }\nusing NGraphicAppearance::CTCGraphicAppearance;' + t[fw.end():]
    return t, 'ok'
def ck(S0, S1, a):
    new = [x for x in S1 - S0 if 'CTCGraphicAppearance' in x]
    bad = [x for x in new if x not in PDB]
    if bad: return 'non-PDB spellings: %s' % bad[:3]
    if any('CTCGraphicAppearance@@' in x for x in S1): return 'global spelling remains'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'nga9')
