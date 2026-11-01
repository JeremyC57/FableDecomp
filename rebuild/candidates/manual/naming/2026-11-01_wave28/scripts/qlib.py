# demangled-signature helpers (from fam_q.py)
import re
import und
from memlib import split_args
DEM = re.compile(r'^(public|protected|private): (static |virtual )?(.*?) ?(__thiscall|__fastcall|__cdecl|__stdcall) (.+?)\((.*)\)( const)?\s*$')
FREE = re.compile(r'^(.*?) ?(__thiscall|__fastcall|__cdecl|__stdcall) ([\w:]+)\((.*)\)\s*$')
def splitq(q):
    out = []; d = 0; cur = ''; i = 0
    while i < len(q):
        if q[i] == '<': d += 1
        elif q[i] == '>': d -= 1
        if d == 0 and q.startswith('::', i): out.append(cur); cur = ''; i += 2; continue
        cur += q[i]; i += 1
    out.append(cur); return out
def typ(s):
    s = re.sub(r'\b(class|struct|enum|union) ', '', s.strip())
    s = re.sub(r'\s*([*&])', r'\1', s); s = re.sub(r'([*&])(?=\w)', r'\1 ', s)
    return s.replace('(void)', '()').strip()
def parse(sym):
    d = und.get([sym])[sym]
    m = DEM.match(d)
    if m:
        q = splitq(m.group(5)); ps = [] if m.group(6) in ('void', '') else split_args(m.group(6))
        return dict(kind='mem', acc=m.group(1), static=(m.group(2) or '').strip() == 'static', virtual=(m.group(2) or '').strip() == 'virtual',
                    ret=m.group(3), cc=m.group(4), scope=q[:-1], name=q[-1], params=ps, const=bool(m.group(7)), dem=d)
    m = FREE.match(d)
    if m:
        q = splitq(m.group(3)); ps = [] if m.group(4) in ('void', '') else split_args(m.group(4))
        return dict(kind='free', ret=m.group(1), cc=m.group(2), scope=q[:-1], name=q[-1], params=ps, dem=d)
    mm = re.match(r'^@(\w+)@(\d+)$', sym)
    if mm: return dict(kind='free', ret='?', cc='__fastcall', scope=[], name=mm.group(1), params=['?'] * (int(mm.group(2)) // 4), dem=sym)
    return None
