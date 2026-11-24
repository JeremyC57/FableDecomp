# screen.py <cand.txt>: per candidate row: Codex RESERVED hits (any pending/*/RESERVED.tsv, with status), Account B claim (newest line in
# COORDINATION_CLAIMS.tsv naming the address), T3_PORT_QUEUE column-2 / column-3 hits, current lock owner, and whether the HEAD source
# already defines the opt-in / FABLE_RETAIL_STLP_MAP. Agent NF353. Read-only.
import sys, os, re, glob
R = '/home/user/FableDecomp'; M = R + '/rebuild/candidates/manual'
S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
C = [l.strip() for l in open(sys.argv[1]) if l.strip()]
res = {}
for f in glob.glob(M + '/pending/*/RESERVED.tsv'):
    for l in open(f, encoding='utf-8', errors='replace'):
        for a in C:
            if a in l.lower(): res.setdefault(a, []).append(f.split('/')[-2] + ':' + l.strip().split('\t')[-1][:60])
cl = {}
for l in open(M + '/COORDINATION_CLAIMS.tsv', encoding='utf-8', errors='replace'):
    f = l.rstrip('\n').split('\t')
    if len(f) < 5: continue
    for a in C:
        if a in f[2].lower() or ('0x' + a) in f[2].lower(): cl[a] = '%s/%s/%s' % (f[0], f[1], f[4])
t2 = {}; t3 = {}
for l in open(M + '/naming/T3_PORT_QUEUE.tsv', encoding='utf-8', errors='replace'):
    f = l.rstrip('\n').split('\t')
    if len(f) < 3: continue
    for a in C:
        if f[1].lower().replace('0x', '') == a: t2.setdefault(a, []).append(f[0])
        if a in f[2].lower(): t3.setdefault(a, []).append(f[0] + ':' + f[1])
print('row\treserved\tclaim\tT3col2\tT3col3\tlock\tdecl_only\tstlp_map\tsrc')
for a in C:
    p = nf.src_of(a); t = open(p, encoding='utf-8').read() if p and os.path.exists(p) else ''
    print('\t'.join([a, ';'.join(res.get(a, [])) or '-', cl.get(a, '-'), ','.join(t2.get(a, [])) or '-', ','.join(t3.get(a, [])) or '-',
                     nf.lock_owner(a), 'y' if 'FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY' in t else 'n',
                     'y' if re.search(r'#define FABLE_RETAIL_STLP_MAP\b', t) else 'n', str(p).replace(R + '/', '')]))
