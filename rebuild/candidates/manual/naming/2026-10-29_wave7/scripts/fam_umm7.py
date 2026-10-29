# wave 7 family: 0x004378C0 is ?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z (Ego_r.pdb + FableWin.pdb; chunk 563: the body
# forwards to the CDrawMiniMap at +0x160 whose 0x00643E9A forwards to CMiniMapDisplay::UpdateMarkers(bool)). Every landed
# caller calls it on the global 0x013B8790 with 0, so the stand-in names (Refresh(long), Notify, Register,
# OnHeroCriminalStatusChanged ...) are one function under several names (no ICF in retail: the byte-identical twins
# 0x0070B990 / 0x0070B9E0 and 0x00527860 / 0x005278A0 / 0x00527910 / 0x00527950 stay separate).
# Transform (idempotent; sources whose single declaration commented 0x004378C0 is `void <name>(long|int|bool <arg>);`):
#   the declaration -> `void UpdateMiniMapMarkers(bool <arg>);`, every call `<name>(0)` -> `UpdateMiniMapMarkers(false)`;
#   a stand-in class whose only member is that method -> `class CPlayerGui { public: ... }` and its 0x013B8790 global ->
#   `extern CPlayerGui* g_pPlayerGui;` (the spelling 129 landed rows emit); comment mentions of the old name follow.
import re, sys
ADDR = re.compile(r'0x0*4378c0', re.I)
DECL = re.compile(r'^([ \t]*)void[ \t]+(\w+)[ \t]*\([ \t]*(?:long|int|bool)[ \t]+(\w+)[ \t]*\)[ \t]*;([^\n]*)$', re.M)
def fix(t):
    if 'UpdateMiniMapMarkers(bool' in t and not re.search(r'void[ \t]+(?!UpdateMiniMapMarkers)\w+\([^)]*\);[^\n]*0x0*4378C0', t, re.I):
        return None, 'already'
    ds = [d for d in DECL.finditer(t) if ADDR.search(d.group(4))]
    if len(ds) != 1: return None, 'decl %d' % len(ds)
    d = ds[0]; name = d.group(2)
    code = re.sub(r'//[^\n]*', '', t)
    uses = [m for m in re.finditer(r'\b%s\b' % name, code)]
    calls = list(re.finditer(r'(->|\.)[ \t]*%s[ \t]*\([ \t]*0[ \t]*\)' % name, code))
    if len(uses) != len(calls) + 1: return None, 'other uses of %s (%d uses, %d literal-0 calls)' % (name, len(uses), len(calls))
    # enclosing class of the declaration
    hdr = None
    for m in re.finditer(r'\b(class|struct)[ \t]+(\w+)[^;{(]*\{', t[:d.start()]):
        hdr = m
    if not hdr: return None, 'no class'
    cls = hdr.group(2)
    body_end = t.find('};', d.end())
    body = re.sub(r'//[^\n]*', '', t[hdr.end():body_end])
    body = re.sub(r'\bpublic[ \t]*:', '', body).strip()
    newdecl = d.group(1) + 'void UpdateMiniMapMarkers(bool %s);' % d.group(3)
    col = len(d.group(0)) - len(d.group(4).lstrip())
    if d.group(4).strip(): newdecl += ' ' * max(1, col - len(newdecl)) + d.group(4).lstrip()
    t = t[:d.start()] + newdecl + t[d.end():]
    t = re.sub(r'(->|\.)([ \t]*)%s([ \t]*)\([ \t]*0[ \t]*\)' % name, r'\1\2UpdateMiniMapMarkers\3(false)', t)
    if cls != 'CPlayerGui':
        if not re.fullmatch(r'void[ \t]+%s[ \t]*\([^)]*\)[ \t]*;' % name, body): return None, 'stand-in %s has other members' % cls
        if re.search(r'\bCPlayerGui\b', re.sub(r'//[^\n]*', '', t)): return None, 'CPlayerGui already modelled'
        g = re.search(r'^extern[ \t]+%s[ \t]*\*[ \t]*(\w+)[ \t]*;([^\n]*)$' % cls, t, re.M)
        if not g or not re.search(r'0x0*13b8790', g.group(2), re.I): return None, 'global of %s' % cls
        gname = g.group(1)
        c2 = re.sub(r'//[^\n]*', '', t)
        if len(re.findall(r'\b%s\b' % cls, c2)) != 2: return None, 'class %s used elsewhere' % cls
        if gname != 'g_pPlayerGui' and re.search(r'\bg_pPlayerGui\b', c2): return None, 'g_pPlayerGui taken'
        col = len(g.group(0)) - len(g.group(2).lstrip())
        t = t[:g.start()] + 'extern CPlayerGui* g_pPlayerGui;' + ' ' * max(1, col - 32) + g.group(2).lstrip() + t[g.end():]
        t = re.sub(r'\b(class|struct)([ \t]+)%s\b' % cls, r'class\2CPlayerGui', t, count=1)
        if hdr.group(1) == 'struct' and not re.search(r'class[ \t]+CPlayerGui[ \t]*\{[ \t\n]*public', t):
            t = re.sub(r'(class[ \t]+CPlayerGui[ \t]*\{)', r'\1 public:', t, count=1)
        t = re.sub(r'\b%s\b' % gname, 'g_pPlayerGui', t) if gname != 'g_pPlayerGui' else t
        t = re.sub(r'\b%s\b' % cls, 'CPlayerGui', t)
    t = re.sub(r'CPlayerGui::%s\(long\)' % name, 'CPlayerGui::UpdateMiniMapMarkers(bool)', t)
    t = re.sub(r'CPlayerGui::%s\b' % name, 'CPlayerGui::UpdateMiniMapMarkers', t)
    return t, 'ok %s %s' % (cls, name)
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
