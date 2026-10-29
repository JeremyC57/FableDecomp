# wave 6 family: 0x004FB150 (`mov eax,[ecx+9Ch] ; ret`) is CWorldMap::GetLoadedRegion() const (Ego_r.pdb
# ?GetLoadedRegion@CWorldMap@@QBEJXZ; Ego_r CWorldMap::LoadedRegion +0xBC = retail +0x9C after the eight STLport-4.6
# containers before it shrink by 4 each; QUEUE_561 / chunk 561). Row-local spellings GetCurrentRegionNumber /
# GetCurrentRegionIndex / GetCurrentRegion / GetCurrentMap are in neither PDB at this address.
# Transform (idempotent): inside a row-local `class|struct CWorldMap`, the one declaration `long <Old>()[ const];` whose
# comment names 0x004FB150 becomes `long GetLoadedRegion() const;` and the calls `.<Old>(` / `-><Old>(` in the file are
# respelled. Skips when <Old> is declared anywhere else in the file (another class could own the name).
import re, sys
OLDS = ('GetCurrentRegionNumber', 'GetCurrentRegionIndex', 'GetCurrentRegion', 'GetCurrentMap')
def cls_span(t):
    m = re.search(r'\b(?:class|struct)[ \t]+CWorldMap\b[^;{(]*\{', t)
    if not m: return None
    i = m.end(); d = 1
    while i < len(t) and d:
        d += {'{': 1, '}': -1}.get(t[i], 0); i += 1
    return m.end(), i
def fix(t):
    sp = cls_span(t)
    if not sp: return None, 'no CWorldMap class'
    a, b = sp; body = t[a:b]
    rx = re.compile(r'^([ \t]*)long[ \t]+(' + '|'.join(OLDS) + r')[ \t]*\([ \t]*\)[ \t]*(const[ \t]*)?;([^\n]*0x004FB150[^\n]*)$', re.M | re.I)
    ms = list(rx.finditer(body))
    if len(ms) != 1: return None, 'decl count %d' % len(ms)
    m = ms[0]; old = m.group(2)
    decls = re.findall(r'^[ \t]*(?:virtual[ \t]+|static[ \t]+)?(?:const[ \t]+)?\w[\w:<>]*[ \t*&]+(?:\w+::)?' + old + r'[ \t]*\(', t, re.M)
    if len(decls) != 1: return None, '%s declared %d times' % (old, len(decls))
    if re.search(r'\bGetLoadedRegion\b', t): return None, 'already'
    body = body[:m.start()] + '%slong GetLoadedRegion() const;%s' % (m.group(1), m.group(4)) + body[m.end():]
    t = t[:a] + body + t[b:]
    t = re.sub(r'(\.|->)' + old + r'[ \t]*\(', r'\1GetLoadedRegion(', t)
    if re.search(r'\b' + old + r'\b', t.split('\n', 1)[1] if False else re.sub(r'//[^\n]*', '', t)):
        return None, 'other uses of ' + old
    return t, 'ok ' + old
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
