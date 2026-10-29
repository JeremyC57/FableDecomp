# wave 6 family: CCharString::UnassignString 0x0099E9B0 and CCharString::AllocStringData 0x0099EA60 are protected
# (Ego_r.pdb and FableWin.pdb: ?UnassignString@CCharString@@IAEXXZ,
# ?AllocStringData@CCharString@@IAEPAVCCharStringData@@PBDJ@Z; the second overload
# ?AllocStringData@CCharString@@IAEPAVCCharStringData@@KD@Z is a different function). Row-local CCharString models
# declared them public (QAE) and one took `char*` (PADJ).
# Transform (idempotent): inside a row-local `class|struct CCharString` body, a one-line declaration
#   void UnassignString();   /   CCharStringData* AllocStringData(<const> char* p, long n);
# (and the matching out-of-line definition head in a test) gets the PDB parameter `const char*`, and the declaration is
# wrapped `protected: <decl> <access in force>:` so the members after it keep their access.
import re, sys
HEAD = re.compile(r'\b(class|struct)[ \t]+CCharString\b[ \t]*(?:\n[ \t]*)?\{')
DECL = re.compile(r'^([ \t]*)((?:void[ \t]+UnassignString[ \t]*\([ \t]*\)|CCharStringData[ \t]*\*[ \t]*AllocStringData[ \t]*\([ \t]*(?:const[ \t]+)?char[ \t]*\*[ \t]*\w*[ \t]*,[ \t]*long[ \t]+\w*[ \t]*\))[ \t]*;)')
ACC = re.compile(r'\b(public|private|protected)[ \t]*:(?!:)')
def body_span(t):
    m = HEAD.search(t)
    if not m: return None
    i = m.end(); d = 1
    while i < len(t) and d:
        d += {'{': 1, '}': -1}.get(t[i], 0); i += 1
    return m.group(1), m.end(), i
def fix(t):
    sp = body_span(t)
    if not sp: return None, 'no CCharString class'
    key, a, b = sp
    acc = 'private' if key == 'class' else 'public'
    out = []; n = 0; depth = 0
    for line in t[a:b].split('\n'):
        m = DECL.match(line)
        if m and depth == 0 and not line.lstrip().startswith('protected:'):
            decl = re.sub(r'AllocStringData[ \t]*\([ \t]*char[ \t]*\*', 'AllocStringData(const char*', m.group(2))
            rest = line[m.end():]
            if acc == 'protected': line = m.group(1) + decl + rest
            else: line = '%sprotected: %s %s:%s' % (m.group(1), decl, acc, rest)
            n += 1
        else:
            for x in ACC.finditer(line):
                if depth == 0: acc = x.group(1)
        depth += line.count('{') - line.count('}')
        out.append(line)
    t2 = t[:a] + '\n'.join(out) + t[b:]
    t2 = re.sub(r'(CCharString::AllocStringData[ \t]*\()[ \t]*char[ \t]*\*', r'\1const char*', t2)
    if t2 == t: return None, 'nothing to change'
    return t2, 'ok %d' % n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
