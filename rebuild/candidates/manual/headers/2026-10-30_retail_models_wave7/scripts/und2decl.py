# convert undname output to a C++ member declaration
import re
def ctype(t):
    t = t.strip()
    t = re.sub(r'\b(class|struct|union|enum) ', '', t)
    # X const & -> const X&, X const * -> const X*
    t = re.sub(r'^(.*?) const (\*|&)$', r'const \1\2', t)
    t = re.sub(r'^(.*?) const (\*|&) const$', r'const \1\2 const', t)
    t = t.replace(' &', '&').replace(' *', '*')
    t = re.sub(r'>>', '> >', t)
    return t
def split_args(s):
    out, d, cur = [], 0, ''
    for c in s:
        if c == '<' or c == '(': d += 1
        elif c == '>' or c == ')': d -= 1
        if c == ',' and d == 0: out.append(cur); cur = ''
        else: cur += c
    if cur.strip(): out.append(cur)
    return out
def decl(u, cls, pure=False):
    """u: 'public: virtual class CCharString const & __thiscall CGameScriptInterface::GetName(void)const '"""
    u = u.strip()
    m = re.match(r'^(public|protected|private): (virtual )?(.*?) __thiscall ' + re.escape(cls) + r'::(\w+)\((.*)\)( ?const)?$', u)
    if not m: raise ValueError(u)
    acc, virt, ret, name, args, cst = m.groups()
    a = [] if args.strip() == 'void' else [ctype(x) for x in split_args(args)]
    s = f"virtual {ctype(ret)} {name}({', '.join(a)}){' const' if cst else ''}{' = 0' if pure else ''};"
    return acc, s
