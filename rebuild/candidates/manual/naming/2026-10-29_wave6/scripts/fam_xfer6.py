# wave 6 family: CPersistContext::Transfer is a member template in both PDBs (Ego_r.pdb / FableWin.pdb carry only
# ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z and the two-argument ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>@Z;
# no plain ?Transfer@CPersistContext@@ overload exists). Chunk 561 proved the instances by RTTI
# (.?AV?$CPersistContext_TransferableComponent@<T>@@ wrapper vtables): Transfer<bool> 0x004045C0, <unsigned char> 0x00405A60,
# <float> 0x00410620, <unsigned long> 0x004106F0, <unsigned __int64> 0x0044BB00.
# Transform (idempotent): inside a row-local `class|struct CPersistContext { ... }`, the concrete overload declarations
#   void Transfer(const char* n, X& v, const X& d);   ->  one `template <class T> void Transfer(const char* name, T& value, const T& defaultValue);`
#   void Transfer(const char* n, X& v);                ->  one `template <class T> void Transfer(const char* name, T& value);`
# keeping their address comments on a following comment line. Call sites are untouched (the argument types deduce T).
# Skips (prints `skip <reason>`): pointer-parameter stand-ins, a virtual Transfer overload with the same arity, files
# that already declare the template.
import re, sys
DECL3 = re.compile(r'^([ \t]*)void[ \t]+Transfer[ \t]*\([ \t]*const[ \t]+char[ \t]*\*[ \t]*\w*[ \t]*,[ \t]*([^,()&*]+?)[ \t]*&[ \t]*\w*[ \t]*,'
                   r'[ \t]*const[ \t]+([^,()&*]+?)[ \t]*&[ \t]*\w*[ \t]*\)[ \t]*;[ \t]*(//[^\n]*)?\n', re.M)
DECL2 = re.compile(r'^([ \t]*)void[ \t]+Transfer[ \t]*\([ \t]*const[ \t]+char[ \t]*\*[ \t]*\w*[ \t]*,[ \t]*([^,()&*]+?)[ \t]*&[ \t]*\w*[ \t]*\)[ \t]*;'
                   r'[ \t]*(//[^\n]*)?\n', re.M)
PTR = re.compile(r'void[ \t]+Transfer[ \t]*\([ \t]*const[ \t]+char[ \t]*\*[ \t]*\w*[ \t]*,[ \t]*[^,()]*\*[ \t]*\w*[ \t]*[,)]')
def cls_span(t):
    m = re.search(r'\b(?:class|struct)[ \t]+CPersistContext\b[^;{]*\{', t)
    if not m: return None
    i = m.end(); d = 1
    while i < len(t) and d:
        d += {'{': 1, '}': -1}.get(t[i], 0); i += 1
    return m.end(), i
def fix(t):
    sp = cls_span(t)
    if not sp: return None, 'no CPersistContext class'
    a, b = sp; body = t[a:b]
    if 'template <class T> void Transfer(const char* name, T& value' in body: return None, 'already'
    if re.search(r'template[ \t]*<[ \t]*class[ \t]+T[ \t]*>[ \t]*void[ \t]+Transfer[ \t]*\([ \t]*const[ \t]+char', body):
        # an inline Transfer<T> beside a concrete overload: the explicit-specialization model (fix_inline) ICEs VC7.1
        # RTM at /O2 (C1001 P2 main.c line 148) -- deferred (00676be0).
        return None, 'inline template + concrete overload (deferred: specialization ICE)'
    if PTR.search(body): return None, 'pointer-parameter Transfer stand-in'
    out = body; n = 0
    for rx, tmpl in ((DECL3, 'template <class T> void Transfer(const char* name, T& value, const T& defaultValue);'),
                     (DECL2, 'template <class T> void Transfer(const char* name, T& value);')):
        ms = list(rx.finditer(out))
        if rx is DECL3: ms = [m for m in ms if m.group(2).strip() == m.group(3).strip()]
        if not ms: continue
        ind = ms[0].group(1)
        notes = []
        for m in ms:
            c = m.group(m.lastindex) if m.group(m.lastindex) and m.group(m.lastindex).startswith('//') else ''
            notes.append('%s%s' % (m.group(2).strip(), (' ' + c[2:].strip()) if c else ''))
        rep = '%s%s   // CPersistContext::Transfer<T> (member template, PDB)\n%s//   instances here: %s\n' % (ind, tmpl, ind, '; '.join(notes))
        for m in reversed(ms[1:]): out = out[:m.start()] + out[m.end():]
        m0 = ms[0]; out = out[:m0.start()] + rep + out[m0.end():]
        n += len(ms)
    if not n: return None, 'no concrete Transfer overload'
    if re.search(r'virtual[^;\n]*\bTransfer[ \t]*\([ \t]*const[ \t]+char', out): return None, 'virtual Transfer overload'
    return t[:a] + out + t[b:], 'ok %d' % n
def fix_inline(t, a, b):
    # the row already defines Transfer<T> inline in the class; a concrete overload beside it is an out-of-line instance:
    # declare it as an explicit specialization after the class (mangles ??$Transfer@<T>@CPersistContext@@...).
    body = t[a:b]; ms = [m for m in DECL3.finditer(body) if m.group(2).strip() == m.group(3).strip()]
    if not ms: return None, 'already'
    specs = []
    for m in reversed(ms):
        X = m.group(2).strip(); c = m.group(4) or ''
        specs.insert(0, 'template <> void CPersistContext::Transfer<%s>(const char* name, %s& value, const %s& defaultValue);%s\n'
                     % (X, X, X, ('   ' + c) if c else ''))
        body = body[:m.start()] + body[m.end():]
    end = t.index(';', b) + 1
    nl = t.index('\n', end) + 1
    return t[:a] + body + t[b:nl] + ''.join(specs) + t[nl:], 'ok-spec %d' % len(ms)
EXPLICIT = __import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)), 'xfer_explicit.tsv')
def explicit(p, t):
    # phase 2: calls whose default argument is a literal (0, "") deduce T twice and are ambiguous (C2782); the trial-compiled
    # resolution (xfer_resolve.py) names the instance explicitly: Transfer<X>(...). Exact line replacements keyed by path.
    n = 0
    for l in open(EXPLICIT, encoding='utf-8'):
        rel, old, new = l.rstrip('\n').split('\t')
        if not p.replace('\\', '/').endswith(rel): continue
        if new in t and old not in t: continue
        assert t.count(old) >= 1, (p, old)
        n += t.count(old); t = t.replace(old, new)
    return t, n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None and why != 'already': print('skip', p, why); continue
        new, k = explicit(p, new if new is not None else t)
        if new == t: print('skip', p, why); continue
        if k: why += ' +explicit %d' % k
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
