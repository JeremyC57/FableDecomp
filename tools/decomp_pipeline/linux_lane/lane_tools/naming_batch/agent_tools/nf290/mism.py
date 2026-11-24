# mism.py <scen file> <idx...>: per entry, the rows whose declaration (own model or shared header) differs from the PDB spelling's
# access / const / virtual letters, with the declaration line text; `ok` rows listed compactly. Reads work/scen_all.txt (nf.py scen).
import re, sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF290/tools'); import nf
ACC = {'Q': ('public', 0, 0), 'I': ('protected', 0, 0), 'A': ('private', 0, 0), 'U': ('public', 1, 0), 'M': ('protected', 1, 0), 'E': ('private', 1, 0),
       'S': ('public', 0, 1), 'K': ('protected', 0, 1), 'C': ('private', 0, 1)}
T = open(sys.argv[1]).read().split('\n#'); want_idx = set(int(x) for x in sys.argv[2:])
for blk in T:
    blk = blk.lstrip('#'); lines = blk.split('\n'); h = lines[0].split()
    if not h or not h[0].isdigit() or int(h[0]) not in want_idx: continue
    meth = h[3]; w = h[4][5:]; acc, virt, st = ACC[w[0]]; const = w[1] == 'B'
    print('#%s %s %s %s want=%s' % (h[0], h[1], h[2], meth, w))
    for l in lines[1:]:
        if not l.strip(): continue
        a = l.split()[0]; m = re.search(r'DECL=(\S+)', l); hd = re.search(r'HDR=(\S+)', l); df = re.search(r'DEF=(\S+)', l)
        decs = [] if not m or m.group(1) == '-' else m.group(1).split(',')
        if not decs and hd: decs = ['H:' + x for x in hd.group(1).split(',')]
        bad = []
        for d in decs:
            mm = re.search(r'\[(\w+)[-@][^\]]*\](C?)(V?)', d)
            if not mm: bad.append(d + '?'); continue
            da, dc, dv = mm.group(1), mm.group(2) == 'C', mm.group(3) == 'V'
            if da != acc or dc != const or dv != bool(virt): bad.append(d)
        lf = (re.search(r"leaf=(\S+)", l) or re.search(r"(NOSRC)", l)).group(1)
        tag = 'OK ' if decs and not bad else ('NODECL' if not decs else 'BAD')
        print('  %-6s %s def=%s leaf=%s %s' % (tag, a, df.group(1) if df else '?', lf[:70], ' '.join(bad)))
        if bad:
            p = nf.src_of(a.replace('(self)', ''))
            if p and p.exists():
                L = p.read_text(encoding='utf-8', errors='replace').split('\n')
                for d in bad:
                    if d.startswith('H:'): continue
                    n = int(re.match(r'L(\d+)', d).group(1)); print('        L%d: %s' % (n, L[n - 1].strip()[:200]))
