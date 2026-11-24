# srccensus2.py (NF328): text census for the ~CScriptThing (??1CScriptThing@@UAE@XZ) / copy-constructor (??0CScriptThing@@QAE@ABV0@@Z)
# unit. Per landed row (catalog Source -> address), the reasons its object MAY define either member (to be confirmed by compiling):
#   D_out   out-of-class `CScriptThing::~CScriptThing(` definition (body follows)   D_in  in-class `~CScriptThing() {` body
#   C_out   out-of-class `CScriptThing::CScriptThing(const CScriptThing&` definition   C_in  in-class copy-ctor body
#   MODEL   a local `class CScriptThing` / `struct CScriptThing` definition (implicit members possible)
#   IMPL    FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (implicit copy constructor on the shared header)
#   INC     the row #includes another landed source ("../src/compiled/...") that has one of the reasons above
# -> work/srccensus2.tsv (addr, reasons, kind of each definition: inline / FORCEINLINE / strong, file)
import re
from pathlib import Path
R = Path('/home/user/FableDecomp'); A = Path(__file__).resolve().parent.parent
PS = (R / 'rebuild/build_candidates.ps1').read_text(encoding='utf-8')
rows = []
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", PS, re.S):
    b = m.group(1)
    k = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", b); s = re.search(r"\bSource = '([^']*)'", b)
    if k and s: rows.append((k.group(1).lower(), s.group(1).replace('\\', '/')))
def strip_comments(t):
    t = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), t, flags=re.S)
    return '\n'.join(l.split('//')[0] for l in t.split('\n'))
def kind(head):
    return 'FORCEINLINE' if ('FABLE_FORCEINLINE' in head or '__forceinline' in head) else ('inline' if re.search(r'\binline\b', head) else 'strong')
def isdef(t, end):
    body = t[end:end + 600]; i = body.find('{'); j = body.find(';')
    return i >= 0 and (j < 0 or i < j)
cache = {}
def reasons(path):
    if path in cache: return cache[path]
    try: raw = path.read_text(encoding='utf-8', errors='replace')
    except OSError: cache[path] = ([], []); return cache[path]
    cache[path] = ([], [])
    if 'CScriptThing' not in raw: return cache[path]
    t = strip_comments(raw); r = []; incs = []
    for mm in re.finditer(r'^([^\n;{}]*?)\bCScriptThing\s*::\s*~\s*CScriptThing\s*\(', t, re.M):
        if isdef(t, mm.end()) and not re.search(r'(->|\.)\s*$', mm.group(1)): r.append('D_out:' + kind(mm.group(1)))
    for mm in re.finditer(r'^([^\n;{}:]*?)(?<!::)~\s*CScriptThing\s*\(\s*\)\s*(const\s*)?\{', t, re.M):
        r.append('D_in:' + kind(mm.group(1)))
    for mm in re.finditer(r'^([^\n;{}]*?)\bCScriptThing\s*::\s*CScriptThing\s*\(\s*const\s+CScriptThing\s*&', t, re.M):
        if isdef(t, mm.end()): r.append('C_out:' + kind(mm.group(1)))
    for mm in re.finditer(r'^([^\n;{}:~]*?)(?<![:~\w])CScriptThing\s*\(\s*const\s+CScriptThing\s*&[^)]*\)\s*(:[^{;]*)?\{', t, re.M):
        r.append('C_in:' + kind(mm.group(1)))
    for mm in re.finditer(r'\b(class|struct)\s+CScriptThing\b\s*(:[^{;]*)?\{', t):
        r.append('MODEL:' + mm.group(1))
    if re.search(r'#\s*define\s+FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY', t): r.append('IMPL')
    for mm in re.finditer(r'#\s*include\s+"((?:\.\./)+src/compiled/[^"]+)"', raw):
        incs.append(mm.group(1))
    cache[path] = (r, incs); return cache[path]
out = open(A / 'work/srccensus2.tsv', 'w'); out.write('addr\treasons\tfile\n'); n = 0
for a, s in sorted(rows):
    p = R / 'rebuild/src/compiled' / s
    r, incs = reasons(p)
    r = list(r)
    for i in incs:
        q = (p.parent / i).resolve()
        if not q.exists(): q = (R / 'rebuild/include' / i).resolve()
        rr, _ = reasons(q)
        if rr: r.append('INC(%s):%s' % (q.name, '+'.join(sorted(set(rr)))))
    if r: out.write('%s\t%s\t%s\n' % (a, ','.join(sorted(set(r))), s)); n += 1
out.close(); print('rows', n)
