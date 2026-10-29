# wave 6 family: CBaseIntelligentPointer::GetPItem 0x00A01B50 returns CBaseObject* (Ego_r.pdb / FableWin.pdb
# ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ; 1,596 landed rows already emit it). Rows whose local
# CBaseIntelligentPointer declared `CThing* GetPItem() const;` emitted ?GetPItem@CBaseIntelligentPointer@@QBEPAVCThing@@XZ.
# Transform (idempotent, source files with exactly one GetPItem declaration, inside CBaseIntelligentPointer):
#   `CThing* GetPItem() const;` -> `CBaseObject* GetPItem() const;` (with `class CBaseObject;` ahead of the class when the
#   row has no CBaseObject yet), and every call `<chain>GetPItem()` -> `((CThing*)<chain>GetPItem())`: the item cast that
#   retail's inline CIntelligentPointer<CThing>::GetPItem() performs.
import re, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fam_ref6 import enclosing_class, chain_start
DECL = re.compile(r'^([ \t]*)(?:class[ \t]+)?CThing[ \t]*\*[ \t]*GetPItem[ \t]*\([ \t]*\)[ \t]*const[ \t]*;', re.M)
def fix(t):
    if 'CBaseObject* GetPItem() const;' in t and '((CThing*)' in t: return None, 'already'
    code = re.sub(r'//[^\n]*', '', t)
    decls = [m for m in re.finditer(r'^[^\n]*\b\w[\w:<>]*[ \t*&]+(?:\w+::)?GetPItem[ \t]*\(', code, re.M)
             if not re.search(r'(->|\.)[ \t]*GetPItem', m.group(0))]
    if len(decls) != 1: return None, 'GetPItem declared %d times' % len(decls)
    ds = list(DECL.finditer(t))
    if len(ds) != 1: return None, 'decl shape'
    d = ds[0]
    if enclosing_class(t, d.start()) != 'CBaseIntelligentPointer': return None, 'decl not in CBaseIntelligentPointer'
    t = t[:d.start()] + d.group(1) + 'CBaseObject* GetPItem() const;' + t[d.end():]
    # calls
    out = []; pos = 0; n = 0
    for m in re.finditer(r'(?<![\w])GetPItem[ \t]*\(\)', t):
        ls = t.rfind('\n', 0, m.start()) + 1
        if '//' in t[ls:m.start()]: continue
        if t[ls:m.start()].rstrip().endswith('CBaseObject*'): continue
        st = chain_start(t, m.start())
        if st is None or st < pos: return None, 'chain'
        if re.search(r'\([ \t]*(?:const[ \t]+)?\w+(?:::\w+)*[ \t]*\*[ \t]*\)[ \t]*$', t[:st]) and \
                not re.match(r'[ \t]*(->|\.|\[)', t[m.end():]):
            continue   # already under an explicit pointer cast: that cast does the item conversion
        out.append(t[pos:st]); out.append('((CThing*)'); out.append(t[st:m.end()]); out.append(')'); pos = m.end(); n += 1
    out.append(t[pos:]); t = ''.join(out)
    # CBaseObject declared before the class?
    cm = re.search(r'\b(?:class|struct)[ \t]+CBaseIntelligentPointer\b[^;{]*\{', t)
    if not re.search(r'\b(?:class|struct)[ \t]+CBaseObject\b', t[:cm.start()]):
        if re.search(r'\bstruct[ \t]+CBaseObject\b', t): return None, 'struct CBaseObject'
        ls = t.rfind('\n', 0, cm.start()) + 1
        t = t[:ls] + 'class CBaseObject;\n' + t[ls:]
    return t, 'ok %d' % n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
