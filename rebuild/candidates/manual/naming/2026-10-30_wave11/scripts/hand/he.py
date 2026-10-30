# hand-edit helper: edit(row, 'src'|'tst', old, new) exact, once; edit_all(...) every occurrence; sub(row, k, regex, repl, n)
# on overlay copies ($HE_OV, default ov_hand; a file is copied from the CURRENT repo on first touch). Idempotent.
import sys, os, shutil, re
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import rows
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
R = rows.rows()
def path(a, k):
    rel = ('rebuild/src/compiled/' + R[a]['Source']) if k == 'src' else ('rebuild/tests/' + R[a]['TestSource'])
    dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), rel)
    if not os.path.exists(dst):
        os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copyfile(os.path.join(rows.ROOT, rel), dst)
    return dst, rel
def _rw(a, k):
    p, rel = path(a, k)
    return p, rel, open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
def _w(p, t): open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t)
def edit(a, k, old, new):
    p, rel, t = _rw(a, k)
    if old in new and new in t: print('already', a, k); return
    n = t.count(old)
    if n != 1:
        if (new and new in t) or (not new and n == 0): print('already', a, k); return   # a deletion already made
        raise SystemExit('COUNT %d in %s for %r' % (n, rel, old[:100]))
    _w(p, t.replace(old, new)); print('ok', a, k)
def edit_all(a, k, old, new, n=None):
    p, rel, t = _rw(a, k)
    c = t.count(old)
    if c == 0 and new in t: print('already', a, k); return
    if c == 0 or (n is not None and c != n): raise SystemExit('COUNT %d in %s for %r' % (c, rel, old[:100]))
    _w(p, t.replace(old, new)); print('ok', a, k)
def touch(a, k='src'): path(a, k)
_NS = re.compile(r'\bnamespace\s+(\w+)\s*\{')
def ns_chain(t, pos):
    # enclosing named namespaces at pos (light brace tracker; comments/strings not masked: used on row headers only)
    st = []; i = 0
    tok = re.compile(r'\bnamespace\s+(\w+)\s*\{|\{|\}')
    for m in tok.finditer(t, 0, pos):
        if m.group(0) == '{': st.append(None)
        elif m.group(0) == '}':
            if st: st.pop()
        else: st.append(m.group(1))
    return st
def block_end(t, i):
    d = 0
    for j in range(i, len(t)):
        if t[j] == '{': d += 1
        elif t[j] == '}':
            d -= 1
            if d == 0:
                k = t.index(';', j); return k + 1
    raise SystemExit('unbalanced')
def hoist(a, k, start_pat, extra_before='', fix=None, qual=()):
    # move the class block matching start_pat (inside named namespaces only) to the enclosing global scope by closing and
    # reopening the namespaces around it; struct -> class { public:; fix(text) post-processes the hoisted text
    p, rel, t = _rw(a, k)
    m = re.search(start_pat, t)
    if not m:
        if 'hoisted to global scope: naming wave 10' in t: print('already', a, k); return   # re-run on an already hoisted file
        raise SystemExit('no %r in %s' % (start_pat, rel))
    ch = ns_chain(t, m.start())
    if not ch: print('already', a, k); return
    if None in ch: raise SystemExit('class nested in a non-namespace scope in %s' % rel)
    s = m.start()
    if extra_before:
        assert t[s - len(extra_before):s] == extra_before, (rel, t[s - 60:s]); s -= len(extra_before)
    e = block_end(t, m.start()); body = t[s:e]
    body = re.sub(r'\bstruct(\s+CCharString\s*)\{', r'class\1{ public:', body, count=1)
    if fix: body = fix(body)
    for q in qual: body = re.sub(r'(?<!::)(?<!\w)' + q + r'\b', '::'.join(ch) + '::' + q, body)
    close = '}' * len(ch); reopen = ' '.join('namespace %s {' % n for n in ch)
    t = t[:s] + close + ' // (hoisted to global scope: naming wave 10)\n' + body + '\n' + reopen + t[e:]
    _w(p, t); print('ok', a, k)
def opeq_const(body):
    return re.sub(r'(?<!const )(?<!const)\bCCharString(\s*)&(\s*)operator(\s*)=(\s*)\(', r'const CCharString\1&\2operator\3=\4(', body)
def resub(a, k, pat, repl, n=None):
    # regex substitution on the overlay copy; n = expected count (None: at least one); idempotent when nothing matches and repl text present
    p, rel, t = _rw(a, k)
    t2, c = re.subn(pat, repl, t)
    if c == 0: print('already', a, k); return
    if n is not None and c != n: raise SystemExit('COUNT %d in %s for %r' % (c, rel, pat[:100]))
    _w(p, t2); print('ok', a, k)
def nscript(a, k, name):
    # move a global `struct NAME [: base] { ... };` into namespace NScript as a class (PDB class key V), with a using-declaration
    # so the rest of the row keeps the unqualified name
    p, rel, t = _rw(a, k)
    if 'using NScript::%s;' % name in t: print('already', a, k); return
    m = re.search(r'\nstruct ' + name + r'(\s*:\s*(?:public\s+)?\w+)?\s*\{', t)
    if not m: raise SystemExit('no struct %s in %s' % (name, rel))
    e = t.index('\n};', m.end()) + 3
    base = (' : public ' + re.sub(r'^\s*:\s*(?:public\s+)?', '', m.group(1))) if m.group(1) else ''
    body = t[m.end():e]
    t = t[:m.start()] + '\nnamespace NScript {\nclass %s%s { public:' % (name, base) + body + '\n}\nusing NScript::%s;' % name + t[e:]
    _w(p, t); print('ok', a, k)
def recode(a, k, start_marker, code, line1=None, leaf=None, extra_comment=None):
    # replace the text from the first occurrence of start_marker to EOF with code; optionally set comment line 1 / Leaf
    p, rel, t = _rw(a, k)
    if extra_comment and extra_comment.split('\n')[0] in t and code.strip() in t: print('already', a, k); return
    i = t.find(start_marker)
    if i < 0:
        if code.strip() in t: print('already', a, k); return
        raise SystemExit('no marker %r in %s' % (start_marker[:60], rel))
    t = t[:i] + code
    L = t.split('\n')
    if line1 is not None:
        if L[0].startswith('//'): L[0] = line1
        else: L.insert(0, line1)
    if leaf is not None:
        lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
        if lf: L[lf[0]] = '// Leaf: ' + leaf
        else: L.insert(1, '// Leaf: ' + leaf)
    if extra_comment:
        lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
        L.insert((lf[0] + 1) if lf else 1, extra_comment)
    _w(p, '\n'.join(L)); print('ok', a, k)
