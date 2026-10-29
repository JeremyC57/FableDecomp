# wave 7 family gmc7: the const twin. 0x00449960 is ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (Ego_r.pdb + FableWin.pdb:
# `const CPlayer& GetMainPlayer() const`); rows whose only GetMainPlayer declaration is `CPlayer* GetMainPlayer() const;` emit
# ?GetMainPlayer@CPlayerManager@@QBEPAVCPlayer@@XZ (same bytes). Transform (idempotent; fam_gmp6's rewrite on the const form):
#   `CPlayer* GetMainPlayer() const;` -> `const CPlayer& GetMainPlayer() const;`;  `...GetMainPlayer()->m` -> `...GetMainPlayer().m`;
#   any other use `<chain>GetMainPlayer()` -> `&<chain>GetMainPlayer()`.  Then the row is compiled (lane cl.exe, /Od, timeout
#   120 s): a row whose code needs a non-const CPlayer (non-const CPlayer methods or a `CPlayer*` local) is left alone.
import re, sys, os
W = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, W); sys.path.insert(0, os.path.join(W, 'tools'))
import cc7
sys.path.insert(0, os.path.join(W, '..', 'w6'))
from fam_gmp6 import chain_start
DECL = re.compile(r'\b(?:class[ \t]+)?CPlayer[ \t]*\*[ \t]*GetMainPlayer[ \t]*\([ \t]*\)[ \t]*const[ \t]*;')
def fix(t, tag):
    code = re.sub(r'//[^\n]*', '', t)
    if re.search(r'const[ \t]+CPlayer[ \t]*&[ \t]*GetMainPlayer\(\)[ \t]*const;', code) and not DECL.search(code): return None, 'already'
    ds = list(DECL.finditer(t))
    alld = re.findall(r'\bGetMainPlayer[ \t]*\([^)]*\)[ \t]*(?:const[ \t]*)?[;{]', code)
    calls_semicolon = re.findall(r'(?:->|\.)[ \t]*GetMainPlayer[ \t]*\(\)[ \t]*;', code)
    if len(ds) != 1: return None, 'decl %d' % len(ds)
    if len(alld) - len(calls_semicolon) != 1: return None, 'other GetMainPlayer declarations'
    d = ds[0]
    t = t[:d.start()] + 'const CPlayer& GetMainPlayer() const;' + t[d.end():]
    out = []; pos = 0; n = 0
    for m in re.finditer(r'\bGetMainPlayer[ \t]*\(\)', t):
        ls = t.rfind('\n', 0, m.start()) + 1
        if '//' in t[ls:m.start()]: continue
        if t[ls:m.start()].rstrip().endswith('const CPlayer&'): continue
        rest = t[m.end():]; r2 = rest.lstrip(' \t')
        if r2.startswith('->'):
            out.append(t[pos:m.end()]); pos = m.end() + (len(rest) - len(r2)) + 2; out.append('.'); n += 1
        else:
            s = chain_start(t, m.start())
            if s is None or s < pos: return None, 'chain'
            out.append(t[pos:s]); out.append('&'); pos = s; n += 1
    out.append(t[pos:]); t = ''.join(out)
    e = cc7.errors(t, tag)
    if e: return None, 'compile: line %d %s' % (e[0][0], e[0][1][:100])
    return t, 'ok %d' % n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t, os.path.basename(p)[:-4])
        if new is None: print('skip', p, why); sys.stdout.flush(); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why); sys.stdout.flush()
