# Ordered assert-pin repair of fin/<rel>: align the merged header with HEAD (pins normalised) and give every HEAD-origin
# assert line HEAD's effective number; then align with each folder version (x[3] / new file, the folder's own pinned text) and
# give the remaining assert lines the folder's effective number. Reports disagreements and unaligned HEAD asserts.
# argv[1] = check | fix
import json, os, re, sys, difflib
L = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/lc2/'
R = '/home/user/FableDecomp/rebuild/include/'
items = json.load(open(L + 'items.json')); mode = sys.argv[1]
A = re.compile(r'FABLE_STATIC_ASSERT(?:_AT\(\s*(\d+)\s*,\s*|\()')

def norm(l):
    return re.sub(r'\s+', ' ', A.sub('FABLE_STATIC_ASSERT(', l.strip()))

def eff(lines):
    out = {}
    for i, l in enumerate(lines):
        if l.lstrip().startswith('#') or l.lstrip().startswith('//'):
            continue
        m = A.search(l)
        if m:
            out[i] = int(m.group(1)) if m.group(1) else i + 1
    return out

def rd(p):
    return open(p, 'rb').read().decode('latin-1')

changed = 0; probs = []
for rel in open(L + 'changed.txt').read().split():
    raw = rd(L + 'fin/' + rel); crlf = '\r\n' in raw; M = raw.replace('\r\n', '\n').split('\n')
    if 'FABLE_STATIC_ASSERT' not in raw:
        continue
    want = {}   # merged line index -> (number, source)
    srcs = []
    if os.path.exists(R + rel):
        srcs.append(('HEAD', rd(R + rel).replace('\r\n', '\n').split('\n')))
    for x in items.get(rel, []):
        p = x[2] if x[0] == 'new' else x[3]
        srcs.append((x[1].split('acctC_')[-1], rd(p).replace('\r\n', '\n').split('\n')))
    nm = [norm(l) for l in M]
    for name, V in srcs:
        e = eff(V); nv = [norm(l) for l in V]
        sm = difflib.SequenceMatcher(None, nv, nm, autojunk=False)
        aligned = {}
        for i1, j1, n in sm.get_matching_blocks():
            for k in range(n):
                aligned[i1 + k] = j1 + k
        for i, num in e.items():
            j = aligned.get(i)
            if j is None:
                if name == 'HEAD':
                    probs.append((rel, 'HEAD assert unaligned', i + 1, V[i].strip()[:80]))
                continue
            if j in want:
                if want[j][0] != num:
                    probs.append((rel, 'head_vs_folder' if want[j][1] == 'HEAD' else 'disagree', j + 1, want[j], (num, name), M[j].strip()[:60]))
                continue
            want[j] = (num, name)
    new = list(M)
    for j, (num, src) in sorted(want.items()):
        l = M[j]; m = A.search(l)
        if not m:
            continue
        cur = int(m.group(1)) if m.group(1) else j + 1
        if cur == num:
            continue
        sp = 'FABLE_STATIC_ASSERT(' if num == j + 1 else 'FABLE_STATIC_ASSERT_AT(%d, ' % num
        new[j] = l[:m.start()] + sp + l[m.end():]
        changed += 1; print('fix', rel, 'line', j + 1, cur, '->', num, src)
    for j in eff(M):
        if j not in want:
            probs.append((rel, 'unattributed', j + 1, M[j].strip()[:80]))
    if mode == 'fix' and new != M:
        t = '\n'.join(new)
        if crlf:
            t = t.replace('\n', '\r\n')
        open(L + 'fin/' + rel, 'wb').write(t.encode('latin-1'))
print('changed', changed)
for p in probs:
    print('PROB', p)
