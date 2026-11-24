# triage.py (NF339): scripted re-read of the 227 T3 "self respell" ENTRIES lines on today's tree, no compiles.
# Per line: the recorded old -> PDB spelling, today's landed Leaf, stay-off flags, how the class is modelled in the row (shared header
# include / local model / alias class), and whether the shared header already declares the member.  -> work/tri.tsv (raw facts)
import os, re, sys, csv, collections, glob
R = '/home/user/FableDecomp'; S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad')
A = S + '/w22/agentNF339'; W = A + '/work'
sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.argv = ['x']
os.chdir(R)
import naming_status as ns
ent = ns.landed_rows()
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
# ---- stay-off sets
off = collections.defaultdict(list)
for ag in ('NF337', 'NF338'):
    for l in open(f'{S}/w22/agent{ag}/ENTRIES.tsv'):
        for x in re.findall(r'\b(?:0x)?([0-9a-fA-F]{8})\b', l.split('\t')[0] + ' ' + (l.split('\t')[1] if '\t' in l else '')):
            off[x.lower()].append('entries_' + ag)
for p in glob.glob(R + '/rebuild/candidates/manual/pending/*codex_pass2[678]*/RESERVED.tsv'):
    for l in open(p).read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) > 3 and f[3].startswith('reserved'): off[f[0].lower()].append('codex_reserved')
newest = {}
for l in open(R + '/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv').read().splitlines():
    f = l.split('\t')
    if len(f) > 4: newest[(f[1], f[2])] = f
for (acct, item), f in newest.items():
    if acct == 'B' and f[4] not in ('done', 'released'):
        for x in re.findall(r'([0-9a-fA-F]{8})', item): off[x.lower()].append('B_claim')
for p in glob.glob(R + '/rebuild/candidates/manual/pending/2026-11-24_acctB_b24/rows/*.cpp'):
    off[os.path.basename(p)[:8].lower()].append('acctB_b24')
def lock(a):
    p = f'{S}/w22/nflocks/{a}/owner'
    return open(p).read().strip() if os.path.exists(p) else 'released'
# other T3 lines naming the address (not ours)
ours = {(e[0], e[1].lower().replace('0x', '')) for e in E}
t3o = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv', encoding='utf-8', errors='replace').read().splitlines()[1:]:
    q = l.split('\t')
    if len(q) < 3: continue
    a = q[1].lower().replace('0x', '')
    if (q[0], a) not in ours: t3o[a].append(q[0] + ':col2')
    for x in re.findall(r'0x([0-9a-fA-F]{8})', q[2]): t3o[x.lower()].append(q[0] + ':unit')
SYM = re.compile(r'(\?\S+)\s+->\s+(\?\S+)')
def cls_meth(sym):
    m = re.match(r'\?(\?[0-9A-Z_]|[^@?]+)@((?:[^@]+@)*?)@', sym)
    if not m: return None, None
    return m.group(1), '::'.join(reversed([x for x in m.group(2).split('@') if x]))
out = []
for e in E:
    tag, a = e[0], e[1].lower().replace('0x', '')
    m = SYM.search(e[3]); old, new = (m.group(1), m.group(2)) if m else ('', '')
    if tag == 'nf334':
        old = re.search(r'binds (\?\S+)', e[3]).group(1) if re.search(r'binds (\?\S+)', e[3]) else ''
    kv = ent.get(a); src = kv['Source'] if kv else ''
    text = open(R + '/rebuild/src/compiled/' + src, encoding='utf-8', errors='replace').read() if src else ''
    leaf = ns.leaf_of(src) if src else ''
    meth, cls = cls_meth(new) if new else (None, None)
    ocls = cls_meth(old)[1] if old else None
    base = (cls or '').split('::')[-1]
    incs = re.findall(r'#\s*include\s*"([^"]+)"', text)
    hdr = f'engine/{base}.h'
    hpath = R + '/rebuild/include/' + hdr
    hdecl = ''
    if base and os.path.exists(hpath):
        ht = open(hpath, encoding='utf-8', errors='replace').read()
        hl = [l.strip() for l in ht.splitlines() if meth and re.search(r'\b' + re.escape(meth) + r'\s*\(', l) and not l.strip().startswith('//')]
        hdecl = ' | '.join(hl[:3])[:300]
    local = bool(base and re.search(r'^\s*(?:class|struct)\s+' + re.escape(base) + r'\b[^;]*\{', text, re.M))
    lalias = ocls.split('::')[-1] if ocls and ocls != cls else ''
    flags = sorted(set(off.get(a, []) + (['lock=' + lock(a)] if lock(a) not in ('released', 'NF339') else []) + t3o.get(a, [])))
    if not new: state = 'binder_line'
    else: state = None
    state = state or ('no_row' if not kv else ('already_pdb' if leaf == new else ('leaf_old' if leaf == old else ('leaf_prefix' if leaf and new.startswith(leaf) else f'leaf_other'))))
    out.append([a, tag, state, old, new, leaf, src, hdr if hdr in incs else ('hdr_exists' if os.path.exists(hpath) else 'no_hdr'), 'local' if local else '', lalias, hdecl, ';'.join(flags), e[3][:200]])
with open(W + '/tri.tsv', 'w') as f:
    f.write('addr\ttag\tstate\told\tnew\tleaf\tsrc\thdr\tlocal\talias_cls\thdr_decl\tflags\treason\n')
    for r in out: f.write('\t'.join(r) + '\n')
print(collections.Counter(r[2] for r in out)); print(collections.Counter(bool(r[11]) for r in out))
# ---- second pass (advisor): self_symbols.tsv (last line per address) and the member's own declaration in the row
ss = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 2: ss[f[0].lower()] = (f[1], f[2])
ACC = {'Q': 'public', 'I': 'protected', 'A': 'private', 'U': 'public virtual', 'M': 'protected virtual', 'E': 'private virtual',
       'S': 'public static', 'K': 'protected static', 'C': 'private static'}
def tailletter(sym):
    m = re.match(r'\?(\?[0-9A-Z_]|[^@?]+)@((?:[^@]+@)*?)@([A-Z])([AB]?)', sym or '')
    return (m.group(3), m.group(4)) if m else ('', '')
rows2 = []
for r in out:
    a, new, old, src = r[0], r[4], r[3], r[6]
    sym, st = ss.get(a, ('', ''))
    ssv = 'ss=new' if sym and sym == new else ('ss=old' if sym and sym == old else ('ss=none' if not sym else 'ss=other'))
    text = open(R + '/rebuild/src/compiled/' + src, encoding='utf-8', errors='replace').read() if src else ''
    meth, cls = cls_meth(new) if new else (None, None)
    decl = ''
    if meth and not meth.startswith('?'):
        code = re.sub(r'//[^\n]*', '', text)
        d = [m.group(0).strip() for m in re.finditer(r'[^;{}\n]*\b' + re.escape(meth) + r'\s*\([^;{)]*\)[^;{]*[;{]', code)]
        decl = ' | '.join(re.sub(r'\s+', ' ', x)[:120] for x in d[:3])
    alias_def = ''
    if r[9]:
        alias_def = 'alias_defined' if re.search(r'\b(?:class|struct)\s+' + re.escape(r[9]) + r'\b', text) else 'alias_gone'
    L, C = tailletter(new)
    rows2.append(r + [ssv, sym if ssv == 'ss=other' else '', st, ACC.get(L, L) + (' const' if C == 'B' else ''), alias_def, decl])
with open(W + '/tri2.tsv', 'w') as f:
    f.write('addr\ttag\tstate\told\tnew\tleaf\tsrc\thdr\tlocal\talias_cls\thdr_decl\tflags\treason\tss\tss_sym\tss_status\tpdb_tail\talias_def\trow_decl\n')
    for r in rows2: f.write('\t'.join(r) + '\n')
print(collections.Counter((r[2], r[13]) for r in rows2))
