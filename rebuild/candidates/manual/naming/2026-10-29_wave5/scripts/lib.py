"""Shared helpers for the w2b family transforms (masking, brace matching, class bodies, access)."""
import re, os

def mask(text):
    """Same-length copy with comments and string/char literals blanked (newlines kept)."""
    out = list(text); i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            j = text.find('\n', i); j = n if j < 0 else j
            for k in range(i, j): out[k] = ' '
            i = j
        elif c == '/' and i + 1 < n and text[i + 1] == '*':
            j = text.find('*/', i + 2); j = n if j < 0 else j + 2
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
        elif c == '#':
            # preprocessor line: keep (needed for #define detection) but blank `<...>` include names
            i += 1
        else:
            i += 1
    return ''.join(out)

def match(m, pos, o, c):
    d = 0
    for k in range(pos, len(m)):
        if m[k] == o: d += 1
        elif m[k] == c:
            d -= 1
            if d == 0: return k
    return len(m)

def class_bodies(m, name):
    """[(key, lb, rb, head_start)] for every `class|struct name [: bases] {` definition."""
    out = []
    for h in re.finditer(r'\b(class|struct)\s+%s\b\s*(?::[^;{()]*)?\{' % re.escape(name), m):
        lb = h.end() - 1
        out.append((h.group(1), lb, match(m, lb, '{', '}'), h.start()))
    return out

LAB = re.compile(r'\b(public|private|protected)\s*:(?!:)')

def access_at(m, key, lb, pos):
    acc = 'public' if key == 'struct' else 'private'
    d = 0; k = lb + 1
    while k < pos:
        ch = m[k]
        if ch == '{': d += 1
        elif ch == '}': d -= 1
        elif d == 0 and ch == 'p' and not (m[k - 1].isalnum() or m[k - 1] == '_'):
            mm = LAB.match(m, k)
            if mm: acc = mm.group(1); k = mm.end(); continue
        k += 1
    return acc

def depth1(m, lb, pos):
    seg = m[lb + 1:pos]
    return seg.count('{') == seg.count('}')

def ns_ranges(m):
    return [(n.end() - 1, match(m, n.end() - 1, '{', '}')) for n in re.finditer(r'\bnamespace\s*\w*\s*\{', m)]

def in_ns(m, pos, nsr=None):
    return any(a < pos < b for a, b in (nsr if nsr is not None else ns_ranges(m)))

def apply(text, edits):
    """edits: {offset: (length, new)}; applied right to left."""
    for off, (ln, new) in sorted(edits.items(), reverse=True):
        text = text[:off] + new + text[off + ln:]
    return text

def rw(path, fn):
    raw = open(path, 'rb').read()
    text = raw.decode('utf-8', errors='surrogateescape')
    new, info = fn(text)
    if new != text:
        open(path, 'wb').write(new.encode('utf-8', errors='surrogateescape'))
    return new != text, info
