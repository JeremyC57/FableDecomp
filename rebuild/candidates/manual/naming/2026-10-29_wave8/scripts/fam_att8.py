# fam_att8.py <file>...: CTCDParticleEmitter::AttachToThing takes CThing& (Ego_r.pdb + FableWin.pdb
#   ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z, 0x006E0BE0). Declarations `AttachToThing(CThing* x,`
#   become `CThing& x`; definitions with a body bind `CThing* x = &x_ref;` first; calls `->AttachToThing(e, ...)` / `.AttachToThing(`
#   pass `*e` (or drop a leading `&`). Only the float-last overload (JM) is respelled. Idempotent.
import sys, re
DECL = re.compile(r'AttachToThing\((\s*)(class\s+)?CThing\s*\*\s*(\w*)(\s*,[^;{)]*\bfloat\b\s*\w*\s*\))(\s*)(\{|;|const)')
DECL0 = re.compile(r'AttachToThing\((\s*)(class\s+)?CThing\s*\*\s*(\w*)(\s*,[^;{)]*\bfloat\s*\))')
CALL = re.compile(r'(->|\.)AttachToThing\(')
def first_arg(t, i):
    d = 0; j = i
    while j < len(t):
        c = t[j]
        if c in '([': d += 1
        elif c in ')]':
            if d == 0: return None
            d -= 1
        elif c == ',' and d == 0: return j
        j += 1
    return None
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    def dsub(m):
        if m.group(6) == '{' and m.group(3):
            return 'AttachToThing(%s%sCThing& %s_ref%s%s{ CThing* %s = &%s_ref;' % (m.group(1), m.group(2) or '', m.group(3), m.group(4), m.group(5), m.group(3), m.group(3))
        return 'AttachToThing(%s%sCThing&%s%s%s%s%s' % (m.group(1), m.group(2) or '', ' ' if m.group(3) else '', m.group(3), m.group(4), m.group(5), m.group(6))
    t = DECL.sub(dsub, t)
    out = []; pos = 0
    for m in CALL.finditer(t):
        s = m.end(); e = first_arg(t, s)
        if e is None: continue
        arg = t[s:e]; a = arg.strip()
        if a.startswith('*'): continue
        lead = arg[:len(arg) - len(arg.lstrip())]
        new = a[1:] if a.startswith('&') else ('*' + a if re.fullmatch(r'[\w.\->:()\[\]]+', a) and not re.search(r'\w\s*\(\s*$', a) else '*(' + a + ')')
        out.append(t[pos:s] + lead + new); pos = e
    t = ''.join(out) + t[pos:]
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
