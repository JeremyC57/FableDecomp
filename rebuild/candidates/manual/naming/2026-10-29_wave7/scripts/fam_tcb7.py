# wave 7 family: CTCBase's protected accessors (Ego_r.pdb CTCBase member list in donor order = the retail catalog
# 0x00686A70.. labels; wave 1 / 6 DECISIONS, coordinator RELOC_CONFLICT list 2026-10-29):
#   0x00686A70 ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ            CWorld& GetWorld() const
#   0x00686A80 ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ           const CWorld& PeekWorld() const
#   0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ     const CWorldMap& PeekWorldMap() const
#   0x00686D30 ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ      CWorldMap& GetWorldMap()
#   0x00686D40 ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ          CPlayerManager& GetPlayerManager()
#   0x00686D50 ?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ         const CPlayerManager& PeekPlayerManager() const
# Rows model them as public pointer-returning members of the derived TC class (or of a local CTCBase), so they emit
# ?GetWorld@CTCHeroStats@@QAEPAVCWorld@@XZ and the like. Transform (idempotent; sources only):
#   every declaration `[const] T* <name>()[ const];` whose comment names one of the addresses (T the PDB return class),
#   inside class X, moves to `class CTCBase { protected: <PDB decl>; };` -- the file's own CTCBase model when X derives
#   from it, else an injected empty base placed before X (X gains `: public CTCBase`; X must have no other base and
#   the file no other CTCBase body); calls `<name>()->m` -> `<new>().m`, `*<name>()` -> `<new>()`, other uses ->
#   `&<new>()`; a local `T* v = <name>();` of a const accessor becomes `const T* v = &<new>();`.
# Blocked: two declarations of one name, a declaration of a class other than the PDB return class, X with bases other
# than CTCBase, X not a class body, calls on another object (`obj->GetWorld()` where obj is not this) -- compile decides.
import re, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fam_ref6 import enclosing_class, chain_start
TAB = {
    '686a70': ('GetWorld', 'CWorld', 'CWorld& GetWorld() const;', False),
    '686a80': ('PeekWorld', 'CWorld', 'const CWorld& PeekWorld() const;', True),
    '686d20': ('PeekWorldMap', 'CWorldMap', 'const CWorldMap& PeekWorldMap() const;', True),
    '686d30': ('GetWorldMap', 'CWorldMap', 'CWorldMap& GetWorldMap();', False),
    '686d40': ('GetPlayerManager', 'CPlayerManager', 'CPlayerManager& GetPlayerManager();', False),
    '686d50': ('PeekPlayerManager', 'CPlayerManager', 'const CPlayerManager& PeekPlayerManager() const;', True),
}
CONSTIFY = [('CPlayerManager', 'GetMainPlayerNumber'),    # ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ
            ('CWorldMap', 'GetNavigationLayerAt'),        # ?GetNavigationLayerAt@CWorldMap@@QBEJABVC3DVector@@PBVCThing@@@Z
            ('CWorldMap', 'GetRegionNumberMapIsIn')]      # ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z
DECL = re.compile(r'^([ \t]*)((?:const[ \t]+)?(?:class[ \t]+)?(\w+)[ \t]*\*[ \t]*(\w+)[ \t]*\([ \t]*(?:void)?[ \t]*\)[ \t]*(?:const[ \t]*)?;)([ \t]*//[^\n]*)$', re.M)
def body_span(t, hdr_end):
    d = 0; i = hdr_end - 1
    while i < len(t):
        if t[i] == '{': d += 1
        elif t[i] == '}':
            d -= 1
            if d == 0: return hdr_end, i
        i += 1
    return None
def fix(t):
    found = []
    for m in DECL.finditer(t):
        a = re.search(r'0x0*(686[0-9a-f]{3})\b', m.group(5), re.I)
        if a and a.group(1).lower() in TAB: found.append((m, a.group(1).lower()))
    if not found:
        return (None, 'already') if re.search(r'class CTCBase \{ protected:|\bPeekWorld\(\) const;|\bGetWorld\(\) const;', t) else (None, 'no decl')
    code = re.sub(r'//[^\n]*', '', t)
    classes = set()
    for m, a in found:
        name, ret, newdecl, peek = TAB[a]
        if m.group(3) != ret: return None, 'decl returns %s not %s' % (m.group(3), ret)
        old = m.group(4)
        nd = len(re.findall(r'\b\w[\w:<>]*[ \t*&]+%s[ \t]*\([^)\n]*\)[ \t]*(?:const[ \t]*)?[;{]' % old, code))
        if nd != 1: return None, '%s declared %d times' % (old, nd)
        classes.add(enclosing_class(t, m.start(2)))
    if len(classes) != 1 or None in classes: return None, 'decl classes %s' % classes
    X = classes.pop()
    hx = [h for h in re.finditer(r'\b(class|struct)[ \t]+%s\b([^;{()]*)\{' % X, t)]
    if len(hx) != 1: return None, 'class %s bodies %d' % (X, len(hx))
    hx = hx[0]; bases = hx.group(2).strip()
    own = [h for h in re.finditer(r'\b(class|struct)[ \t]+CTCBase\b([^;{()]*)\{', t)]
    decls = [TAB[a][2] for m, a in found]
    # remove the declarations from X (bottom-up), remember the line text for comments
    for m, a in sorted(found, key=lambda x: -x[0].start()):
        le = m.end() + 1 if m.end() < len(t) and t[m.end()] == '\n' else m.end()
        t = t[:m.start()] + t[le:]
    hx = re.search(r'\b(class|struct)[ \t]+%s\b([^;{()]*)\{' % X, t)
    inj = 'protected:\n' + ''.join('    %s   // 0x%s (Ego_r.pdb CTCBase, protected)\n' % (d, a.zfill(8).upper()) for d, (m, a) in zip(decls, found))
    if X == 'CTCBase':
        sp = body_span(t, hx.end())
        t = t[:sp[1]] + inj + t[sp[1]:]
    elif own:
        if len(own) != 1: return None, 'CTCBase bodies %d' % len(own)
        if not re.fullmatch(r':[ \t]*public[ \t]+CTCBase', bases): return None, '%s does not derive from the file CTCBase (%s)' % (X, bases)
        o = re.search(r'\b(class|struct)[ \t]+CTCBase\b([^;{()]*)\{', t)
        sp = body_span(t, o.end())
        t = t[:sp[1]] + inj + t[sp[1]:]
    else:
        if bases: return None, '%s has bases %r' % (X, bases)
        xs = body_span(t, hx.end())
        if re.search(r'\bvirtual\b', re.sub(r'//[^\n]*', '', t[xs[0]:xs[1]])):
            return None, 'polymorphic %s: an empty CTCBase base would sit after the vfptr (this+4)' % X
        ls = t.rfind('\n', 0, hx.start()) + 1
        if t[ls:hx.start()].strip(): return None, 'class head not at line start'
        t = t[:ls] + 'class CTCBase { ' + inj.replace('\n', '\n', 1) + '};\n' + t[ls:]
        hx = re.search(r'\b(class|struct)[ \t]+%s\b([^;{()]*)\{' % X, t)
        t = t[:hx.end() - 1].rstrip() + ' : public CTCBase {' + t[hx.end():]
    # the return classes must be declared (as classes) before the CTCBase body
    o = re.search(r'\b(class|struct)[ \t]+CTCBase\b[^;{()]*\{', t)
    for T in sorted(set(TAB[a][1] for m, a in found)):
        if re.search(r'\bstruct[ \t]+%s\b' % T, t): return None, 'struct %s (would mangle U)' % T
        if not re.search(r'\bclass[ \t]+%s\b' % T, t[:o.start()]):
            ls = t.rfind('\n', 0, o.start()) + 1
            t = t[:ls] + 'class %s;\n' % T + t[ls:]
            o = re.search(r'\b(class|struct)[ \t]+CTCBase\b[^;{()]*\{', t)
    # call sites
    for m, a in found:
        name, ret, newdecl, peek = TAB[a]; old = m.group(4)
        out = []; pos = 0
        for c in re.finditer(r'(?<![\w~])%s[ \t]*\([ \t]*\)' % old, t):
            ls = t.rfind('\n', 0, c.start()) + 1
            if '//' in t[ls:c.start()]: continue
            le = t.find('\n', c.end()); le = len(t) if le < 0 else le
            if '(Ego_r.pdb CTCBase, protected)' in t[ls:le]: continue
            pre = t[ls:c.start()]
            if re.search(r'[\w>)\]][ \t]*(->|\.)[ \t]*$', pre) and not re.search(r'\bthis[ \t]*->[ \t]*$', pre):
                return None, 'call on another object: %s' % pre.strip()[-40:]
            rest = t[c.end():]; r2 = rest.lstrip(' \t')
            tok = '@@TCB_%s@@' % name
            if r2.startswith('->'):
                out.append(t[pos:c.start()]); out.append(tok + '().'); pos = c.end() + (len(rest) - len(r2)) + 2
            elif re.search(r'\*[ \t]*$', pre) and not re.search(r'\w[ \t]*\*[ \t]*$', pre):
                st = pre.rstrip()[:-1]; out.append(t[pos:ls] + st); out.append(tok + '()'); pos = c.end()
            else:
                lv = re.search(r'(?:^|[;{}\s])((?:const[ \t]+)?%s[ \t]*\*[ \t]*\w+[ \t]*=[ \t]*)$' % ret, pre)
                if lv and peek and not lv.group(1).startswith('const'):
                    out.append(t[pos:ls + lv.start(1)]); out.append('const ' + lv.group(1) + '&' + tok + '()'); pos = c.end()
                else:
                    out.append(t[pos:c.start()]); out.append('&' + tok + '()'); pos = c.end()
        out.append(t[pos:]); t = ''.join(out)
        t = re.sub(r'CTCBase::%s([ \t]*(?:\(\))?[ \t]*@?[ \t]*0x0*%s)' % (old, a), r'CTCBase::%s\1' % name, t, flags=re.I)
    t = re.sub(r'@@TCB_(\w+)@@', r'\1', t)
    # methods reached through a const accessor that the PDB spells const (Ego_r.pdb)
    for cls, meth in CONSTIFY:
        for d in list(re.finditer(r'^([^\n]*\b%s[ \t]*\([^)\n]*\))([ \t]*);' % meth, t, re.M)):
            if d.group(1).lstrip().startswith(('//', 'return')) or '->' in d.group(1) or '.' + meth in d.group(1): continue
            if enclosing_class(t, d.start(1) + d.group(1).find(meth)) != cls: continue
            t = t[:d.end(1)] + ' const' + t[d.end(1):]
    return t, 'ok %s %s' % (X, ','.join(TAB[a][0] for m, a in found))
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
