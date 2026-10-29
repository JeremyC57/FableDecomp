# wave 7 family tcd7: fam_tcb7's transform for the rest of the CTCBase protected accessor run (Ego_r.pdb CTCBase = retail +
# 0x2B33F0; DECISIONS_W7):
#   0x006869D0 ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ  const CGameDefinitionManager& GetDefinitionManager() const
#   0x00686AE0 ?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ                CThingManager& GetThingManager()
#   0x00686D60 ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ            const CMainGameComponent& PeekComponent() const
#   0x00686D80 ?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ        CThingSearchTools& GetThingSearchTools()
#   0x00686D90 ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ       const CThingSearchTools& PeekThingSearchTools() const
# Rows declare GetDefinitionManager as returning CDefinitionManager* (GetDefGlobalIndexFromName / GetPDefFromGlobalIndex live on
# that base, Ego_r.pdb, both const); the transform adds `class CGameDefinitionManager : public CDefinitionManager { };` after the
# row's CDefinitionManager. Every edited row is compiled (lane cl.exe /Od, timeout 120 s) and left alone on any error.
import re, sys, os
W = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, W); sys.path.insert(0, os.path.join(W, 'tools'))
import fam_tcb7 as B
import cc7
B.TAB = {
    '6869d0': ('GetDefinitionManager', 'CDefinitionManager', 'const CGameDefinitionManager& GetDefinitionManager() const;', True),
    '686ae0': ('GetThingManager', 'CThingManager', 'CThingManager& GetThingManager();', False),
    '686d60': ('PeekComponent', 'CMainGameComponent', 'const CMainGameComponent& PeekComponent() const;', True),
    '686d80': ('GetThingSearchTools', 'CThingSearchTools', 'CThingSearchTools& GetThingSearchTools();', False),
    '686d90': ('PeekThingSearchTools', 'CThingSearchTools', 'const CThingSearchTools& PeekThingSearchTools() const;', True),
}
B.CONSTIFY = [('CDefinitionManager', 'GetDefGlobalIndexFromName')]   # GetPDefFromGlobalIndex: the rows' return types are stand-ins
def _open_ns(t, pos):
    st = []
    for m in re.finditer(r'\bnamespace[ \t]+\w+[ \t\n]*\{|\{|\}', t[:pos]):
        g = m.group(0)
        if g == '}':
            if st: st.pop()
        else: st.append(g.startswith('namespace'))
    return any(st)

B.DECL = re.compile(r'^([ \t]*)((?:const[ \t]+)?(?:class[ \t]+)?(\w+)[ \t]*\*[ \t]*(\w+)[ \t]*\([ \t]*(?:void)?[ \t]*\)[ \t]*(?:const[ \t]*)?;)([ \t]*//[^\n]*)$', re.M)
_re_already = re.compile(r'GetDefinitionManager\(\) const;   // 0x006869D0|GetThingManager\(\);   // 0x00686AE0|PeekComponent\(\) const;   // 0x00686D60|ThingSearchTools\(\)(?: const)?;   // 0x00686D[89]0')
def fix(t, tag):
    if _re_already.search(t): return None, 'already'
    new, why = B.fix(t)
    if new is None: return None, why
    t = new
    o = re.search(r'\b(class|struct)[ \t]+CTCBase\b[^;{()]*\{', t)
    if o and _open_ns(t, o.start()): return None, 'CTCBase would sit inside a namespace'
    if 'CGameDefinitionManager& GetDefinitionManager()' in t and not re.search(r'\bclass[ \t]+CGameDefinitionManager\b[^;]*\{', t):
        m = re.search(r'\b(class|struct)[ \t]+CDefinitionManager\b[^;{()]*\{', t)
        if not m: return None, 'no CDefinitionManager body'
        if m.group(1) == 'struct': return None, 'struct CDefinitionManager'
        sp = B.body_span(t, m.end()); e = t.find(';', sp[1]) + 1
        le = t.find('\n', e); le = len(t) if le < 0 else le
        t = t[:le + 1] + 'class CGameDefinitionManager : public CDefinitionManager { };\n' + t[le + 1:]
        o = re.search(r'\b(class|struct)[ \t]+CTCBase\b[^;{()]*\{', t)
        if o.start() < le:   # CTCBase body ahead of CDefinitionManager: forward-declare the derived class
            ls = t.rfind('\n', 0, o.start()) + 1
            t = t[:ls] + 'class CGameDefinitionManager;\n' + t[ls:]
    e = cc7.errors(t, tag)
    if e: return None, 'compile: line %d %s' % (e[0][0], e[0][1][:100])
    return t, why
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t, os.path.basename(p)[:-4])
        if new is None: print('skip', p, why); sys.stdout.flush(); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why); sys.stdout.flush()
