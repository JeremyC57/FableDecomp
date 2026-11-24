# srccensus_op.py (NF331, from NF328's srccensus2.py): text census for the CScriptThing::operator= unit (??4CScriptThing@@QAEAAV0@ABV0@@Z,
# self 0x008AB980). Per landed row (catalog Source -> address), the reasons its object MAY define the member (confirmed by compiling):
#   A_out   out-of-class `CScriptThing::operator=(` definition (body follows)        A_in  in-class `operator=(const CScriptThing&` body
#   MODEL   a local `class CScriptThing` / `struct CScriptThing` definition (implicit operator= possible)
#   IMPLA   FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT (implicit operator= on the shared header)
#   NSCR    FABLE_RETAIL_NSCRIPT defined (the polymorphic shared model, where only a declaration is visible) -- recorded, not a reason alone
#   INC     the row #includes another landed source ("../src/compiled/...") that has one of the reasons above
# -> work/srccensus_op.tsv (addr, reasons, kind of each definition: inline / FORCEINLINE / strong, file); work/rescan_op.tsv lists every
# OTHER landed source whose comment-stripped text names both `CScriptThing` and `operator` + `=` (the unanchored re-scan).
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
cache = {}; rescan = {}
def reasons(path):
    if path in cache: return cache[path]
    try: raw = path.read_text(encoding='utf-8', errors='replace')
    except OSError: cache[path] = ([], []); return cache[path]
    cache[path] = ([], [])
    if 'CScriptThing' not in raw: return cache[path]
    t = strip_comments(raw); r = []; incs = []
    for mm in re.finditer(r'^([^\n;{}]*?)\bCScriptThing\s*::\s*operator\s*=\s*\(', t, re.M):
        if isdef(t, mm.end()) and not re.search(r'(->|\.)\s*$', mm.group(1)): r.append('A_out:' + kind(mm.group(1)))
    for mm in re.finditer(r'^([^\n;{}:]*?)(?<!::)\boperator\s*=\s*\(\s*const\s+CScriptThing\s*&[^)]*\)\s*\{', t, re.M):
        r.append('A_in:' + kind(mm.group(1)))
    for mm in re.finditer(r'\b(class|struct)\s+CScriptThing\b\s*(:[^{;]*)?\{', t):
        r.append('MODEL:' + mm.group(1))
    if re.search(r'#\s*define\s+FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT\b', t): r.append('IMPLA')
    if r and re.search(r'#\s*define\s+FABLE_RETAIL_NSCRIPT\b', t): r.append('NSCR')
    if not r and re.search(r'\boperator\s*=', t) and 'CScriptThing' in t: rescan[path] = True
    for mm in re.finditer(r'#\s*include\s+"((?:\.\./)+src/compiled/[^"]+)"', raw):
        incs.append(mm.group(1))
    cache[path] = (r, incs); return cache[path]
out = open(A / 'work/srccensus_op.tsv', 'w'); out.write('addr\treasons\tfile\n'); n = 0; inset = set()
for a, s in sorted(rows):
    p = R / 'rebuild/src/compiled' / s
    r, incs = reasons(p)
    r = list(r)
    for i in incs:
        q = (p.parent / i).resolve()
        if not q.exists(): q = (R / 'rebuild/include' / i).resolve()
        rr, _ = reasons(q)
        if rr: r.append('INC(%s):%s' % (q.name, '+'.join(sorted(set(rr)))))
    if r and r != ['NSCR']: out.write('%s\t%s\t%s\n' % (a, ','.join(sorted(set(r))), s)); n += 1; inset.add(p)
out.close(); print('rows', n)
ro = open(A / 'work/rescan_op.tsv', 'w')
k = 0
for a, s in sorted(rows):
    p = R / 'rebuild/src/compiled' / s
    if p in rescan and p not in inset: ro.write('%s\t%s\n' % (a, s)); k += 1
ro.close(); print('rescan candidates (CScriptThing + operator= text, no reason)', k)
