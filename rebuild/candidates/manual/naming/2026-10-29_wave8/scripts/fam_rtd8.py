# fam_rtd8.py <file>...: CDisplayManager::GetRenderTargetDimensions is non-const and returns `const C2DExtentsI` (Ego_r.pdb +
#   FableWin.pdb ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ, 0x009BEDC0). Respells declarations /
#   inline definitions inside a `class CDisplayManager` body (and out-of-line CDisplayManager:: definitions). Idempotent.
import sys, re
DECL = re.compile(r'(?<!const )\bC2DExtentsI(\s+)(CDisplayManager::)?GetRenderTargetDimensions\(\s*\)(\s*const)?')
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    def sub(m):
        if not m.group(2):
            head = list(re.finditer(r'\b(class|struct)\s+(\w+)[^;{()]*\{', t[:m.start()]))
            if not head or head[-1].group(2) != 'CDisplayManager': return m.group(0)
        return 'const C2DExtentsI%s%sGetRenderTargetDimensions()' % (m.group(1), m.group(2) or '')
    t = DECL.sub(sub, t)
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
