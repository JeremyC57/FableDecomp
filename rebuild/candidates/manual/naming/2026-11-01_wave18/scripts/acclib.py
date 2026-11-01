# acclib: access / const respelling of one member declaration inside its class body
import re
def class_span(t, pos):
    # innermost class/struct body containing pos: (kw, name, body_start(after '{'), body_end(index of matching '}'))
    best = None
    for m in re.finditer(r'\b(class|struct)\s+(\w+)\b[^;{()]*\{', t):
        if m.end() > pos: break
        d = 0
        for j in range(m.end() - 1, len(t)):
            c = t[j]
            if c == '{': d += 1
            elif c == '}':
                d -= 1
                if d == 0: break
        if m.end() <= pos < j: best = (m.group(1), m.group(2), m.end(), j)
    return best
def access_at(t, kw, start, pos):
    acc = 'private' if kw == 'class' else 'public'
    # only top-level specifiers of this body (skip nested braces)
    d = 0; i = start
    for m in re.finditer(r'[{}]|\b(public|protected|private)\s*:(?!:)', t[start:pos]):
        s = m.group(0)
        if s == '{': d += 1
        elif s == '}': d -= 1
        elif d == 0: acc = m.group(1)
    return acc
def respell_decl(t, decl_rx, new_decl, want_acc=None):
    """decl_rx: regex matching exactly one member declaration (whole, incl ';'); new_decl replaces it; want_acc: access the
    member must have ('protected'/'private'/'public'); returns new text or (None, reason)"""
    ms = list(re.finditer(decl_rx, t))
    if len(ms) != 1: return None, '%d declarations' % len(ms)
    m = ms[0]; cs = class_span(t, m.start())
    if not cs: return None, 'not in a class'
    kw, name, bs, be = cs
    cur = access_at(t, kw, bs, m.start())
    ls = t.rfind('\n', 0, m.start()) + 1; le = t.find('\n', m.end()); le = len(t) if le < 0 else le
    line = t[ls:le]; ind = re.match(r'[ \t]*', line).group(0)
    pre = line[:m.start() - ls]; post = line[m.end() - ls:]
    if pre.strip(): return None, 'declaration shares its line: %r' % pre.strip()[:40]
    if re.search(r'\b(public|protected|private)\s*:(?!:)', post.split('//')[0]): return None, 'access label after the declaration'
    newline = ind + new_decl + post
    if want_acc and cur != want_acc:
        newline = ind[:-4 if len(ind) >= 4 else 0] + want_acc + ':\n' + newline + '\n' + ind[:-4 if len(ind) >= 4 else 0] + cur + ':'
    return t[:ls] + newline + t[le:], name
