"""Minimal MSVC type-name demangler for RTTI TypeDescriptor names (.?AV / .?AU), templates included.
Output follows the Ego_r donor spelling (struct_layouts_egor.tsv): `A::B<C,unsigned_short,D_*>` (spaces as '_')."""
PRIM = {'C': 'signed char', 'D': 'char', 'E': 'unsigned char', 'F': 'short', 'G': 'unsigned short', 'H': 'int',
        'I': 'unsigned int', 'J': 'long', 'K': 'unsigned long', 'M': 'float', 'N': 'double', 'O': 'long double',
        'X': 'void', '_N': 'bool', '_J': '__int64', '_K': 'unsigned __int64', '_W': 'wchar_t'}

class _P:
    def __init__(self, s): self.s = s; self.i = 0
    def peek(self, n=1): return self.s[self.i:self.i + n]
    def take(self, n=1): r = self.s[self.i:self.i + n]; self.i += n; return r

def _num(p):
    c = p.take()
    neg = False
    if c == '?': neg = True; c = p.take()
    if c.isdigit(): v = int(c) + 1
    else:
        v = 0
        while c != '@':
            v = v * 16 + (ord(c) - ord('A')); c = p.take()
    return -v if neg else v

def _frag(p, names):
    if p.peek().isdigit():
        return names[int(p.take())]
    if p.peek(2) == '?$':
        p.take(2)
        nm = _ident(p)
        tnames = [nm]
        args = _args(p, tnames)
        r = f"{nm}<{','.join(args)}>"
        names.append(r); return r
    r = _ident(p); names.append(r); return r

def _ident(p):
    j = p.s.index('@', p.i); r = p.s[p.i:j]; p.i = j + 1; return r

def _qname(p, names):
    parts = []
    while p.peek() != '@':
        if not p.peek(): raise ValueError('eof')
        parts.append(_frag(p, names))
    p.take()
    return '::'.join(reversed(parts))

def _type(p, names):
    c = p.peek()
    if c.isdigit():
        raise ValueError('type backref')
    if p.peek(2) in PRIM: return PRIM[p.take(2)]
    if c in PRIM and c != '_': p.take(); return PRIM[c]
    if c in 'VUT': p.take(); return _qname(p, names)
    if p.peek(2) == 'W4': p.take(2); return _qname(p, names)
    if c in 'PQ':
        p.take(); cv = p.take()
        t = _type(p, names)
        return t + (' const' if cv == 'B' else '') + ' *'
    if c == 'A':
        p.take(); cv = p.take(); t = _type(p, names); return t + (' const' if cv == 'B' else '') + ' &'
    if p.peek(4) == '$$CB': p.take(4); return _type(p, names) + ' const '
    if p.peek(4) == '$$CA': p.take(4); return _type(p, names)
    raise ValueError('type ' + p.s[p.i:p.i + 8])

def _args(p, names):
    out = []
    while p.peek() != '@':
        if not p.peek(): raise ValueError('eof')
        if p.peek(2) == '$0': p.take(2); out.append(str(_num(p)))
        else: out.append(_type(p, names))
    p.take()
    return out

def demangle(td: str) -> str:
    """'.?AV?$CArray@G@@' -> 'CArray<unsigned_short>' ; returns td unchanged when it cannot parse."""
    if not td.startswith(('.?AV', '.?AU')): return td
    p = _P(td[4:])
    try:
        r = _qname(p, [])
    except (ValueError, IndexError):
        return td
    return r.replace(' *', '_*').replace(' &', '_&').replace(' ', '_')

def last(n: str) -> str:
    """last top-level '::' component (template arguments may contain '::')"""
    d = 0; cut = 0; i = 0
    while i < len(n):
        c = n[i]
        if c == '<': d += 1
        elif c == '>': d -= 1
        elif c == ':' and d == 0 and n[i:i + 2] == '::': cut = i + 2; i += 1
        i += 1
    return n[cut:]

def norm(n: str) -> str:
    return n.replace(' ', '').replace('_', '').replace('struct', '').replace('class', '')

if __name__ == '__main__':
    import sys
    for a in sys.argv[1:]: print(a, '->', demangle(a))
