"""Shared helpers for the wave-2 agent-A family transforms (fam_*.py).

Every transform works on a masked copy of the text (comments, string and char literals blanked to
spaces, same length), finds declarations by brace scope, and rewrites the original text at the
offsets it found. All transforms are deterministic and idempotent.
"""
import re, sys, os


def mask(text):
    """Same-length copy of text with comments, string and char literals blanked."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            j = text.find('\n', i)
            j = n if j < 0 else j
            for k in range(i, j): out[k] = ' '
            i = j
        elif c == '/' and i + 1 < n and text[i + 1] == '*':
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != '\n': out[k] = ' '
            i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            j = min(j + 1, n)
            for k in range(i + 1, j - 1): out[k] = ' '
            i = j
        else:
            i += 1
    return ''.join(out)


def comment_spans(text, m):
    """(start, end) spans of comments: where text and mask differ outside literals."""
    spans = []
    i, n = 0, len(text)
    while i < n:
        if text[i] == '/' and i + 1 < n and text[i + 1] in '/*' and m[i] == ' ':
            if text[i + 1] == '/':
                j = text.find('\n', i); j = n if j < 0 else j
            else:
                j = text.find('*/', i + 2); j = n if j < 0 else j + 2
            spans.append((i, j)); i = j
        else:
            i += 1
    return spans


def match_brace(m, pos):
    d = 0
    for k in range(pos, len(m)):
        if m[k] == '{': d += 1
        elif m[k] == '}':
            d -= 1
            if d == 0: return k
    return len(m)


HEAD_CLASS = re.compile(r'(?:^|[\s;{}])(?:template\s*<[^{};]*>\s*)?(class|struct|union)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{};]*)?$', re.S)
HEAD_NS = re.compile(r'(?:^|[\s;{}])namespace\s*([A-Za-z_]\w*)?\s*$', re.S)
HEAD_EXTERN = re.compile(r'(?:^|[\s;{}])extern\s*"\s*"\s*$|(?:^|[\s;{}])extern\s*"[^"]*"\s*$', re.S)


class Scope:
    __slots__ = ('lb', 'rb', 'kind', 'name', 'key', 'parent')

    def __init__(self, lb, rb, kind, name, key, parent):
        self.lb, self.rb, self.kind, self.name, self.key, self.parent = lb, rb, kind, name, key, parent

    def __repr__(self):
        return f'Scope({self.kind} {self.name} {self.lb}-{self.rb})'


def scopes(m):
    """All brace scopes of the masked text, with kind ns / class / linkage / block."""
    out = []
    stack = []
    for k, ch in enumerate(m):
        if ch == '{':
            # head: text from the previous ; { } (or start) to the brace
            j = k - 1
            while j >= 0 and m[j] not in ';{}':
                j -= 1
            head = m[j + 1:k]
            # strip preprocessor lines from the head
            head = '\n'.join(l for l in head.split('\n') if not l.lstrip().startswith('#'))
            kind, name, key = 'block', None, None
            mc = HEAD_CLASS.search(head)
            mn = HEAD_NS.search(head)
            if mn:
                kind, name = 'ns', mn.group(1) or ''
            elif HEAD_EXTERN.search(head):
                kind = 'linkage'
            elif mc and not re.search(r'\benum\s+(class|struct)?\s*\w*\s*$', head) and '(' not in head.split(mc.group(1))[-1].split(':')[0]:
                kind, name, key = 'class', mc.group(2), mc.group(1)
            s = Scope(k, None, kind, name, key, stack[-1] if stack else None)
            out.append(s); stack.append(s)
        elif ch == '}':
            if stack:
                stack.pop().rb = k
    for s in out:
        if s.rb is None: s.rb = len(m)
    return out


def innermost(sc, pos):
    best = None
    for s in sc:
        if s.lb < pos < s.rb and (best is None or s.lb > best.lb):
            best = s
    return best


def chain(s):
    out = []
    while s is not None:
        out.append(s); s = s.parent
    return out


def at_global(s):
    """True when scope s (a class) sits at file scope (only extern-"C" blocks around it)."""
    p = s.parent
    while p is not None:
        if p.kind != 'linkage': return False
        p = p.parent
    return True


def is_global_pos(sc, pos):
    s = innermost(sc, pos)
    while s is not None:
        if s.kind != 'linkage': return False
        s = s.parent
    return True


def global_classes(m, sc, name):
    return [s for s in sc if s.kind == 'class' and s.name == name and at_global(s)]


def access_at(m, s, pos):
    """Access in effect at pos (depth-1 labels of class scope s)."""
    acc = 'private' if s.key == 'class' else 'public'
    for mm in re.finditer(r'\b(public|private|protected)\s*:(?!:)', m[s.lb + 1:pos]):
        p = s.lb + 1 + mm.start()
        if innermost_depth(m, s.lb, p) == 1:
            acc = mm.group(1)
    return acc


def innermost_depth(m, lb, pos):
    d = 0
    for k in range(lb, pos):
        if m[k] == '{': d += 1
        elif m[k] == '}': d -= 1
    return d


def apply_edits(text, edits):
    """edits: list of (start, end, new). Non-overlapping; applied back to front."""
    edits = sorted(set(edits), key=lambda e: (e[0], e[1]))
    for a, b in zip(edits, edits[1:]):
        if a[1] > b[0]:
            raise ValueError('overlapping edits %r %r' % (a, b))
    for st, en, new in reversed(edits):
        text = text[:st] + new + text[en:]
    return text


def drop_nested(edits):
    """Drop edits that sit inside a deletion/replacement span of another edit."""
    edits = sorted(set(edits), key=lambda e: (e[0], -e[1]))
    out = []
    for e in edits:
        if any(o[0] <= e[0] and e[1] <= o[1] and (o[1] - o[0]) > (e[1] - e[0])
               and not (e[0] == e[1] and e[0] in (o[0], o[1])) for o in out):
            continue
        out.append(e)
    return out


def del_span(text, m, st, en):
    """Edit deleting [st,en): the whole line when nothing else (but a comment-free blank) is on it,
    else the span plus the blanks before it."""
    a = text.rfind('\n', 0, st) + 1
    b = text.find('\n', en); b = len(text) if b < 0 else b
    if m[a:st].strip() == '' and m[en:b].strip() == '' and '/*' not in text[en:b]:
        return (a, min(b + 1, len(text)), '')
    k = st
    while k > a and text[k - 1] in ' \t': k -= 1
    return (k, en, '')


def tokens(m, name):
    return [mm.start() for mm in re.finditer(r'(?<![\w$])%s(?![\w$])' % re.escape(name), m)]


def prev_sig(m, pos):
    """Previous non-space character index before pos, or -1."""
    j = pos - 1
    while j >= 0 and m[j] in ' \t\r\n':
        j -= 1
    return j


def prev_word(m, pos):
    j = prev_sig(m, pos)
    e = j + 1
    while j >= 0 and (m[j].isalnum() or m[j] == '_'):
        j -= 1
    return m[j + 1:e]


def skipped(path, skip):
    """Reason when the file belongs to a row in the family's skip table, else None."""
    a, _ = row_of(path)
    return skip.get(a)


def run(transform, argv):
    """CLI: fam_x.py [--check] <file>...   rewrites in place; prints `edited <file> <n>`."""
    check = '--check' in argv
    files = [a for a in argv if not a.startswith('--')]
    for f in files:
        raw = open(f, 'rb').read()
        text = raw.decode('utf-8', errors='surrogateescape')
        try:
            new, n, notes = transform(text, f)
        except Exception as e:  # never half-edit a file
            print('error', f, repr(e)); continue
        for note in notes:
            print('note', f, note)
        if new != text:
            if not check:
                open(f, 'wb').write(new.encode('utf-8', errors='surrogateescape'))
            print('edited', f, n)


def row_of(path):
    mm = re.search(r'_([0-9a-f]{8})(_test)?\.cpp$', os.path.basename(path))
    return (mm.group(1), bool(mm.group(2))) if mm else (None, False)


def line_of(text, pos):
    a = text.rfind('\n', 0, pos) + 1
    b = text.find('\n', pos)
    return a, (len(text) if b < 0 else b)


def decl_like(m, p, name):
    """Token at p is a declaration/definition of a function `name` (a type word before it)."""
    j = prev_sig(m, p)
    if j < 0: return False
    if m[j] in '.>:': return False
    if m[j] in '*&': return True
    w = prev_word(m, p)
    if not w or w in ('return', 'new', 'delete', 'else', 'case', 'throw', 'sizeof', 'do'):
        return False
    k = p + len(name)
    while k < len(m) and m[k] in ' \t\r\n': k += 1
    return k < len(m) and m[k] == '(' and (w[0].isalpha() or w[0] == '_')


def free_to_static(text, m, sc, spec):
    """Turn file-scope free stand-ins of a retail static member into calls of that member.

    spec keys: names (free identifiers), ret (regex of the accepted return type), params (callable:
    normalized parameter text -> bool), cls (class name), member (member declaration text),
    qual ('Cls::Fn'), defhead (out-of-line definition head 'ret Cls::Fn'), defparams (callable:
    params -> replacement parameter text or None), header (regex of an include that blocks),
    anchor (regex required on the declaration line, or None), absorbed (names whose member already
    exists in the class model). Returns (edits, notes, done) -- done is False when nothing applies.
    """
    import re as _re
    cc = r'(?:__fastcall|__cdecl|__stdcall|FABLE_FASTCALL)'
    pat = _re.compile(r'(?P<pre>\b(?:extern\s+(?:"\s*"\s+)?|static\s+)?(?:__declspec\s*\(\s*noinline\s*\)\s*)?(?:inline\s+)?)(?P<ret>(?<![\w:])%s)\s*(?:(?P<cc>%s)\s+)?(?P<name>%s)\s*\((?P<params>[^()]*)\)\s*(?P<end>[;{])'
                      % (spec['ret'], cc, '|'.join(map(_re.escape, spec['names']))))
    edits, notes = [], []
    found = {}
    for mm in pat.finditer(m):
        if not is_global_pos(sc, mm.start()): continue
        if spec.get('anchor'):
            a, b = line_of(text, mm.start())
            if not _re.search(spec['anchor'], text[a:b]): continue
        prm = _re.sub(r'\s+', ' ', mm.group('params')).strip()
        if not spec['params'](prm):
            notes.append('PARAMS %s(%s)' % (mm.group('name'), prm)); return [], notes, False
        found.setdefault(mm.group('name'), []).append(mm)
    if not found:
        return [], notes, False
    good = {}
    for name, ms in found.items():
        span = [(x.start(), x.end('name')) for x in ms]
        calls = []
        for p in tokens(m, name):
            if any(a <= p < b for a, b in span): continue
            j = prev_sig(m, p)
            if j >= 0 and m[j] in '.>':
                notes.append('MEMBER_CALL %s' % name); return [], notes, False
            if decl_like(m, p, name):
                notes.append('AMBIG %s line %d' % (name, text.count('\n', 0, p) + 1)); return [], notes, False
            if j >= 1 and m[j] == ':' and m[j - 1] == ':' and prev_word(m, j - 1):
                notes.append('QUALIFIED %s' % name); return [], notes, False
            calls.append(p)
        good[name] = (ms, calls)
    if spec.get('header') and _re.search(spec['header'], text):
        notes.append('HEADER %s' % spec['header']); return [], notes, False
    worlds = global_classes(m, sc, spec['cls'])
    if len(worlds) > 1:
        notes.append('MULTI %s bodies' % spec['cls']); return [], notes, False
    leaf = spec['qual'].split('::')[-1]
    absorbed = spec.get('absorbed', set())
    if worlds and not (set(good) <= absorbed) and any(worlds[0].lb < p < worlds[0].rb for p in tokens(m, leaf)):
        notes.append('EXISTS %s' % spec['qual']); return [], notes, False
    allm = sorted((x for ms, _ in good.values() for x in ms), key=lambda x: x.start())
    decls = [x for x in allm if x.group('end') == ';']
    defs = [x for x in allm if x.group('end') == '{']
    if worlds and any(x.start() < worlds[0].lb for x in defs):
        notes.append('DEF_BEFORE_CLASS'); return [], notes, False
    for x in defs:
        if spec.get('ref') and _re.sub(r'\s+', '', x.group('ret')).endswith('*'):
            notes.append('DEF_KIND'); return [], notes, False
        if spec['defparams'](_re.sub(r'\s+', ' ', x.group('params')).strip()) is None:
            notes.append('DEF_PARAMS'); return [], notes, False
    fwd = _re.search(r'\b(class|struct)\s+%s\s*;' % spec['cls'], m)
    key = fwd.group(1) if fwd else 'class'
    st_of = lambda x: x.start()
    if set(good) <= absorbed:
        for x in decls: edits.append(del_span(text, m, st_of(x), x.end()))
    elif worlds:
        w = worlds[0]
        acc = '' if (w.key == 'struct' and not _re.search(r'\b(private|protected)\s*:', m[w.lb:w.rb])) else 'public:\n'
        ins = ('\n' if m[w.rb - 1] != '\n' else '') + acc + '    ' + spec['member'] + '   ' + spec['comment'] + '\n'
        edits.append((w.rb, w.rb, ins))
        for x in decls: edits.append(del_span(text, m, st_of(x), x.end()))
    else:
        first = allm[0]
        base = ''
        if spec.get('base'):
            bs = [b for b in global_classes(m, sc, spec['base']) if b.rb < first.start()]
            if not bs:
                notes.append('NO_BASE %s' % spec['base']); return [], notes, False
            base = ' : public %s' % spec['base']
        newcls = '%s %s%s { %s%s };' % (key, spec['cls'], base, 'public: ' if key == 'class' else '', spec['member'])
        if first.group('end') == ';':
            edits.append((st_of(first), first.end(), newcls))
        else:
            a0, _ = line_of(text, first.start())
            edits.append((a0, a0, newcls + '   ' + spec['comment'] + '\n'))
        for x in decls:
            if x is not first: edits.append(del_span(text, m, st_of(x), x.end()))
    for x in defs:
        np = spec['defparams'](_re.sub(r'\s+', ' ', x.group('params')).strip())
        edits.append((x.start(), x.end('params') + 1, '%s(%s)' % (spec['defhead'], np)))
    for name, (ms, calls) in good.items():
        old_ptr = any(_re.sub(r'\s+', '', x.group('ret')).endswith('*') for x in ms)
        for p in calls:
            edits.append((p, p + len(name), spec['qual']))
            if spec.get('ref') and old_ptr:
                edits += deref_site(m, p, p + len(name))
    return edits, notes, True


def deref_site(m, st, en):
    """A call `name()` (st..en is the name) whose result turns from T* into T&: `->` becomes `.`,
    any other use takes the address."""
    import re as _re
    r = _re.match(r'\s*\(\s*\)', m[en:])
    if not r: return []
    t = en + r.end()
    while t < len(m) and m[t] in ' \t\r\n': t += 1
    if m.startswith('->', t):
        return [(t, t + 2, '.')]
    b = qual_start(m, st)
    return [(b, b, '&')]


def comment_respell(text, m, names, new, anchor, window=40):
    """Comment edits: a name from `names` (optionally `Qual::`-prefixed) followed within `window`
    characters by the `anchor` regex (the retail address) becomes `new`."""
    import re as _re
    out = []
    alt = '|'.join(_re.escape(n) for n in sorted(names, key=len, reverse=True))
    for a, b in comment_spans(text, m):
        seg = text[a:b]
        for mm in _re.finditer(r'(?<![\w:])(?:\w+\s*::\s*)?(?:%s)(?![\w])(?:\s*\(\))?' % alt, seg):
            if mm.group(0).replace('()', '').strip() == new: continue
            en = a + mm.end()
            if _re.search(anchor, text[en:en + window]):
                g = mm.group(0)
                out.append((a + mm.start(), en, new + ('()' if g.endswith('()') else '')))
    return out


def ns_path(s):
    """Namespace names enclosing scope s, outermost first (linkage blocks skipped)."""
    out = []
    p = s.parent
    while p is not None:
        if p.kind == 'ns': out.append(p.name)
        elif p.kind != 'linkage': return None   # nested in a class or a block
        p = p.parent
    return list(reversed(out))


def qual_start(m, st):
    """Start of the qualified-name chain `a::b::` that ends at st (st: start of the last name)."""
    b = st
    while True:
        j = b - 1
        while j >= 0 and m[j] in ' \t': j -= 1
        if j >= 1 and m[j] == ':' and m[j - 1] == ':':
            c = j - 2
            k = c
            while k >= 0 and m[k] in ' \t': k -= 1
            e2 = k + 1
            while k >= 0 and (m[k].isalnum() or m[k] == '_'): k -= 1
            if k + 1 < e2:
                b = k + 1; continue
            return j - 1          # a leading `::`
        return b


def member_respell(text, m, sc, cfg):
    """Respell a static member getter `static <old ret> [cc] <name>()` of class cfg['cls'] (inside
    namespaces cfg['ns']) to `static <cfg['ret']> __fastcall <cfg['new']>()`.

    cfg: cls, ns (list), names (old member names), new, ret ('T*' or 'T&', T == cls), base_ok (set
    of classes whose ptr/ref an old declaration may return; the class must then derive from it,
    or get `: public <base>` when its head has no base clause and the base is a complete
    file-scope class defined before it).
    Returns (edits, notes, done)."""
    import re as _re
    cls, new, ret = cfg['cls'], cfg['new'], cfg['ret']
    kind = ret.strip()[-1]
    edits, notes = [], []
    decl = _re.compile(r'\bstatic\s+(?P<const>const\s+)?(?P<rc>(?:\w+\s*::\s*)*\w+)\s*(?P<rk>[*&])\s*(?:(?P<cc>__fastcall|__cdecl|__stdcall|FABLE_FASTCALL)\s+)?(?P<name>%s)\s*\(\s*(?:void)?\s*\)'
                       % '|'.join(map(_re.escape, cfg['names'])))
    hits = []
    for s in sc:
        if s.kind != 'class' or s.name != cls or ns_path(s) != cfg['ns']:
            continue
        for mm in decl.finditer(m, s.lb, s.rb):
            if innermost(sc, mm.start()) is not s: continue
            hits.append((s, mm))
    if not hits:
        return [], notes, False
    if len(hits) > 1:
        notes.append('MULTI decls'); return [], notes, False
    s, mm = hits[0]
    rc = _re.sub(r'\s+', '', mm.group('rc')).split('::')[-1]
    rk = mm.group('rk')
    name = mm.group('name')
    if (rc == cls and rk == kind and mm.group('cc') == '__fastcall' and name == new and not mm.group('const')):
        return [], notes, False
    if s.key != 'class' and cfg.get('need_class_key', True):
        notes.append('STRUCT_KEY %s' % cls); return [], notes, False
    head = m[m.rfind(s.key, 0, s.lb):s.lb]
    if rc != cls:
        if rc not in cfg.get('base_ok', ()):
            notes.append('RET %s' % rc); return [], notes, False
        if _re.search(r':\s*(?:public\s+|private\s+|protected\s+)?(?:\w+::)*%s\b' % rc, head):
            pass
        elif ':' in head:
            notes.append('BASES %s' % head.strip()); return [], notes, False
        else:
            bases = [b for b in global_classes(m, sc, rc) if b.rb < s.lb]
            if not bases:
                notes.append('NO_BASE %s' % rc); return [], notes, False
            edits.append((s.lb, s.lb, ': public %s ' % rc if m[s.lb - 1] in ' \t' else ' : public %s ' % rc))
    if name != new and any(s.lb < p < s.rb for p in tokens(m, new)):
        notes.append('EXISTS %s::%s' % (cls, new)); return [], notes, False
    # inline body of the declaration (tests)
    k = mm.end()
    while k < len(m) and m[k] in ' \t\r\n': k += 1
    if k < len(m) and m[k] == '{' and rc != cls:
        notes.append('INLINE_BODY_RET %s' % rc); return [], notes, False
    if k < len(m) and m[k] == '{' and rk != kind:
        e = match_brace(m, k)
        body = m[k:e + 1]
        if kind == '&' and _re.fullmatch(r'\{\s*return\s*&\s*\w+\s*;\s*\}', body):
            amp = m.index('&', k)
            edits.append((amp, amp + 1, ''))
        elif kind == '*' and _re.fullmatch(r'\{\s*return\s+\w+\s*;\s*\}', body):
            r = m.index('return', k) + 6
            edits.append((r, r, ' &'))
        else:
            notes.append('INLINE_BODY'); return [], notes, False
    edits.append((mm.start(), mm.end('name'), 'static %s __fastcall %s' % (ret, new)))
    # out-of-line definitions `R cls::name(` (tests): only when the return kind is unchanged
    ool = _re.compile(r'(?P<ret>(?:const\s+)?(?:\w+\s*::\s*)*\w+\s*[*&])\s*(?:(?:__fastcall|__cdecl)\s+)?(?:\w+\s*::\s*)*%s\s*::\s*(?P<name>%s)\s*\(\s*(?:void)?\s*\)\s*\{'
                      % (cls, _re.escape(name)))
    for od in ool.finditer(m):
        if od.group('ret').strip()[-1] != kind:
            notes.append('OOL_KIND'); return [], notes, False
        orc = _re.sub(r'\s+', '', od.group('ret'))
        if _re.sub(r'^const', '', orc)[:-1].split('::')[-1] != cls:
            # the stub returns a base-class object: it cannot back the derived reference
            notes.append('OOL_RET %s' % orc); return [], notes, False
        if orc.startswith('const'):
            edits.append((od.start('ret'), od.start('ret') + len('const'), ''))
        if name != new:
            edits.append((od.start('name'), od.end('name'), new))
    # calls: `[ns::]cls::name()` anywhere, bare `name()` inside the class body
    q = _re.compile(r'(?<![\w:])(?:::\s*)?(?:\w+\s*::\s*)*%s\s*::\s*(?P<name>%s)\s*\(\s*\)' % (cls, _re.escape(name)))
    sites = [(c.start('name'), c.end('name'), c.end()) for c in q.finditer(m) if not (mm.start() <= c.start() < mm.end())
             and not any(od.start() <= c.start() < od.end() for od in ool.finditer(m))]
    for p in tokens(m, name):
        if s.lb < p < s.rb and not (mm.start() <= p < mm.end()) and prev_sig(m, p) >= 0 and m[prev_sig(m, p)] not in '.>:':
            e = p + len(name)
            r = _re.match(r'\s*\(\s*\)', m[e:])
            if r: sites.append((p, e, e + r.end()))
    for st, en, close in sites:
        if name != new:
            edits.append((st, en, new))
        if rk != kind:
            t = close
            while t < len(m) and m[t] in ' \t\r\n': t += 1
            if kind == '&' and m.startswith('->', t):
                edits.append((t, t + 2, '.'))
            elif kind == '*' and m[t] == '.' and not m.startswith('...', t):
                edits.append((t, t + 1, '->'))
            else:
                # value use: take the address / dereference the whole call expression
                b = qual_start(m, st)
                edits.append((b, b, '&' if kind == '&' else '*'))
    return edits, notes, True
