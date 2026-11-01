# h17_ns.py: naming wave 17, wave 16 DEFERRED 004bfac0 -- CTCGraphicAppearance is NGraphicAppearance::CTCGraphicAppearance (Ego_r
# ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z, a class); the four callers modelled it at global scope as a struct.
# The row's local model moves into the namespace (class, public) with a using-declaration so the row text keeps its spelling. Name-only.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w, block_end
CLS, NS = 'CTCGraphicAppearance', 'NGraphicAppearance'
NOTE = '// Naming wave 17: %s is %s::%s (Ego_r ?SetAlpha@%s@%s@@QAEXE@Z; a class) -- the local model moved into the namespace (name-only).\n' % (CLS, NS, CLS, CLS, NS)
for a in ['0080fad0', '008c64f0', '008ccb00', '008ccd70']:
    p, rel, t = _rw(a, 'src')
    if 'namespace %s' % NS in t: print('already', a); continue
    m = re.search(r'(?m)^struct %s \{' % CLS, t)
    if not m: print('skip', a, 'no definition'); continue
    e = block_end(t, m.start())
    body = t[m.start():e]
    body = body.replace('struct %s {' % CLS, 'class %s {\npublic:' % CLS, 1)
    t = t[:m.start()] + 'namespace %s {\n%s\n} // namespace %s\n' % (NS, body, NS) + t[e:]
    t = re.sub(r'(?m)^struct %s;\n' % CLS, '', t)
    t = re.sub(r'\bstruct %s\*' % CLS, CLS + '*', t)
    if a == '008c64f0':   # the same address landed under the stand-in name SetVisibleFlag
        t = t.replace('    void SetVisibleFlag(unsigned char flag);                 // 0x004BFAC0', '    void SetAlpha(unsigned char alpha);                      // 0x004BFAC0 (was the stand-in SetVisibleFlag)')
        t = re.sub(r'\bSetVisibleFlag\(', 'SetAlpha(', t)
    # forward declaration + using before the first code line
    lines = t.split('\n'); i = 0
    while i < len(lines) and (lines[i].startswith('//') or not lines[i].strip()): i += 1
    lines.insert(i, NOTE + 'namespace %s { class %s; }\nusing %s::%s;' % (NS, CLS, NS, CLS))
    _w(p, '\n'.join(lines)); print('ok', a, 'src')
