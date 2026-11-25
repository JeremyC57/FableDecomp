"""und439.py: llvm-undname wrapper + parser of a demangled function prototype into parts."""
import subprocess, re, functools
@functools.lru_cache(None)
def und(m):
    r = subprocess.run(['llvm-undname'], input=m + '\n', capture_output=True, text=True).stdout.strip().split('\n')
    return r[-1].strip() if r else ''
def split_top(s, sep=','):
    out = []; d = 0; cur = ''
    for ch in s:
        if ch in '<([': d += 1
        elif ch in '>)]': d -= 1
        if ch == sep and d == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
def fix(t):   # VC7.1 needs '> >'
    while '>>' in t: t = t.replace('>>', '> >')
    return t
def parse(m):
    d = und(m)
    x = re.match(r'^(?:(public|protected|private): )?(static |virtual )?(.*?)\s*\b(__thiscall|__fastcall|__cdecl|__stdcall)\s+(.*)$', d)
    if not x: return None
    acc, sv, ret, cc, rest = x.groups()
    const = rest.endswith(' const')
    if const: rest = rest[:-6]
    if not rest.endswith(')'): return None
    dep = 0
    for i in range(len(rest) - 1, -1, -1):
        if rest[i] == ')': dep += 1
        elif rest[i] == '(':
            dep -= 1
            if dep == 0: break
    qn, ps = rest[:i], rest[i + 1:-1]
    params = [] if ps.strip() in ('', 'void') else [fix(p) for p in split_top(ps)]
    parts = split_top(qn.replace('::', '\x00'), '\x00') if '<' in qn else qn.split('::')
    return dict(dem=d, access=acc, sv=(sv or '').strip(), ret=fix(ret.strip()), cc=cc, qn=qn, name=parts[-1], scope=parts[:-1], params=params, const=const)
