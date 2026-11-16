# seq.py <rel> : sequential 3-way merge of every distinct folder version (assert pins undone) into HEAD (pins undone), base = B0.
# Conflicts are resolved from res.json {rel: {"<md5>": "t,o,..." | "file:<path>"}}; an unresolved conflict stops the run and the
# diff3 text is written to seqout/<rel>.<md5>.conf. Clean result -> seqout/<rel> (LF, pins re-applied by repin()).
import json, os, re, subprocess, sys, collections, hashlib
L = os.path.dirname(os.path.abspath(__file__))
B0 = L + '/b0/rebuild/include/'; R = '/home/user/FableDecomp/rebuild/include/'
items = json.load(open(L + '/items.json'))
res = json.load(open(L + '/res.json')) if os.path.exists(L + '/res.json') else {}
PIN = re.compile(r'FABLE_STATIC_ASSERT_AT\(\s*\d+\s*,\s*')
AR = re.compile(r'FABLE_STATIC_ASSERT(?:_AT\(\s*(\d+)\s*,|\()(.*)\);')
rd = lambda p: open(p, encoding='latin-1', newline='').read().replace('\r\n', '\n')
unpin = lambda t: PIN.sub('FABLE_STATIC_ASSERT(', t)
def effmap(text):
    m = collections.defaultdict(list)
    for i, l in enumerate(text.split('\n')):
        x = AR.search(l)
        if x: m[re.sub(r'\s+', '', x.group(2))].append(int(x.group(1)) if x.group(1) else i + 1)
    return m
def repin(text, sources):
    # every assert keeps the effective line number it had in the first source that has it (HEAD first, then folder originals)
    used = collections.defaultdict(int); out = []
    for i, l in enumerate(text.split('\n')):
        x = AR.search(l)
        if x:
            k = re.sub(r'\s+', '', x.group(2)); n = None
            for s in sources:
                if k in s:
                    lst = s[k]; n = lst[min(used[k], len(lst) - 1)]; break
            used[k] += 1
            body = x.group(2) if x.group(1) is None else x.group(2).lstrip()
            if n is not None and n != i + 1:
                if not (x.group(1) is not None and int(x.group(1)) == n):
                    l = l[:x.start()] + 'FABLE_STATIC_ASSERT_AT(%d, %s);' % (n, body) + l[x.end():]
            elif n == i + 1 and x.group(1) is not None:
                l = l[:x.start()] + 'FABLE_STATIC_ASSERT(%s);' % body + l[x.end():]
        out.append(l)
    return '\n'.join(out)
def dnorm(ls):
    c = collections.Counter()
    for l in ls:
        s = re.sub(r'\s+', ' ', l.strip())
        s = re.sub(r'^#\s*ifdef\s+(\w+)$', r'#if defined(\1)', s)
        s = re.sub(r'^#\s*elif\b', '#if', s)
        if s: c[s] += 1
    return c
def auto(txt):
    # conflict hunks where one side's lines are all on the other side (multiset, whitespace-normalised) take the larger side
    norm = lambda ls: collections.Counter(re.sub(r'\s+', ' ', l.strip()) for l in ls if l.strip())
    lines = txt.split('\n'); out = []; i = 0; left = 0
    while i < len(lines):
        if not lines[i].startswith('<<<<<<<'):
            out.append(lines[i]); i += 1; continue
        st = i; o = []; b = []; t = []; i += 1
        while not lines[i].startswith('|||||||'): o.append(lines[i]); i += 1
        i += 1
        while not lines[i].startswith('======='): b.append(lines[i]); i += 1
        i += 1
        while not lines[i].startswith('>>>>>>>'): t.append(lines[i]); i += 1
        i += 1
        no, nt = norm(o), norm(t)
        if not (nt - no): out += o
        elif not (no - nt): out += t
        elif not (dnorm(t) - dnorm(o)):
            out += o; print('    auto(dir-spelling) ours kept over theirs:', [l for l in t if l.startswith('#')][:3])
        else: out += lines[st:i]; left += 1
    return '\n'.join(out), left
def custom(txt, R):
    lines = txt.split('\n'); out = []; i = 0; k = 0
    while i < len(lines):
        if not lines[i].startswith('<<<<<<<'):
            out.append(lines[i]); i += 1; continue
        o = []; b = []; t = []; i += 1
        while not lines[i].startswith('|||||||'): o.append(lines[i]); i += 1
        i += 1
        while not lines[i].startswith('======='): b.append(lines[i]); i += 1
        i += 1
        while not lines[i].startswith('>>>>>>>'): t.append(lines[i]); i += 1
        i += 1
        out += R(k, o, b, t); k += 1
    return '\n'.join(out)
def run(rel):
    os.makedirs(os.path.dirname(L + '/seqout/' + rel), exist_ok=True)
    import glob
    for f in glob.glob(L + '/seqout/' + rel + '.*.conf'): os.remove(f)
    tmp = L + '/seqtmp'; os.makedirs(tmp, exist_ok=True)
    head = rd(R + rel); crlf = '\r\n' in open(R + rel, encoding='latin-1', newline='').read()
    ours = unpin(head); base = unpin(rd(B0 + rel))
    open(tmp + '/base', 'w').write(base)
    seen = set(); srcs = [effmap(head)]
    for x in items[rel]:
        md5 = os.path.basename(x[3])
        theirs = unpin(rd(x[3] + '.u2'))
        srcs.append(effmap(rd(x[3])))
        if theirs in seen: continue
        seen.add(theirs)
        open(tmp + '/ours', 'w').write(ours); open(tmp + '/theirs', 'w').write(theirs)
        r = subprocess.run(['git', 'merge-file', '-p', '--diff3', '-L', 'ours', '-L', 'base', '-L', x[1], tmp + '/ours', tmp + '/base', tmp + '/theirs'], capture_output=True)
        txt = r.stdout.decode('latin-1')
        if r.returncode:
            txt, left = auto(txt)
            if not left:
                ours = txt; print('  auto-resolved', x[1]); continue
            r = None
        if r is None:
            how = res.get(rel, {}).get(md5)
            if how is None:
                open(L + '/seqout/' + rel + '.' + md5 + '.conf', 'w').write(txt)
                print('CONFLICT', rel, x[1], md5, txt.count('<<<<<<<')); return False
            if how.startswith('py:'):
                ns = {}; exec(open(how[3:]).read(), ns); txt = custom(txt, ns['R'])
            elif how.startswith('file:'):
                txt = open(how[5:]).read()
            else:
                open(tmp + '/m', 'w').write(txt)
                subprocess.run([sys.executable, L + '/resolve_pick.py', tmp + '/m', tmp + '/r', how], check=True)
                txt = open(tmp + '/r').read()
        ours = txt
    out = repin(ours, srcs)
    if crlf: out = out.replace('\n', '\r\n')
    open(L + '/seqout/' + rel, 'w', encoding='latin-1', newline='').write(out)
    print('OK', rel); return True
for rel in sys.argv[1:]:
    run(rel)
