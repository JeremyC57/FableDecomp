# sacensus.py <idx..>: NF299 Q4-style census of standin_alias entries. Per wrong table line: the stand-in scope (class / namespace of the
# wrong spelling), the witness row's own self (catalog name + Leaf), and whether the witness's self is DEFINED on that same stand-in scope
# (-> a binder self re-scope onto the real class: T3 port, NF282 D2 / NF300 Q2) or the stand-in is a free function / helper class.
import sys, re, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333/tools'); import nf
E = nf.entries(); T = collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T[p[1].lower().replace('0x', '')].append((p[0], p[2].lower().replace('0x', '')))
off = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(nf.W / 'offlimits_why.tsv')}
oth = set(open(nf.W / 'other_agents_witnesses.txt').read().split())
def scope(sym):
    m = re.match(r'\?\?[0-9A-Z_]([A-Za-z_]\w*)@', sym) or re.match(r'\?[A-Za-z_]\w*@(\w+)@', sym)
    if sym.startswith('??0') or sym.startswith('??1') or sym.startswith('??4') or sym.startswith('??R'):
        m2 = re.match(r'\?\?.([A-Za-z_]\w*)@', sym); return m2.group(1) if m2 else None
    m = re.match(r'\?\w+@(\w+)@', sym); return m.group(1) if m else None
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; t = e[0]; pdb = e[6]
    for s, w in T.get(t, []):
        if s == pdb: continue
        sc = scope(s) or '-'
        src = nf.src_of(w); txt = src.read_text(encoding='utf-8', errors='replace') if src and src.exists() else ''
        lf = re.search(r'^// Leaf:\s*(\S+)', txt, re.M); cat = nf.catalog().get(w, {}).get('name', '?')
        selfdef = bool(re.search(r'\b%s::\w+\s*\(' % re.escape(sc), txt)) if sc != '-' else False
        flag = 'OFF:' + off[w] if w in off else ('NF334W' if w in oth else '')
        verdict = 'PORT(self on stand-in)' if selfdef else ('free-standin' if not s.startswith('?') or re.match(r'\?\w+@@Y', s) else 'helper-class')
        print('%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (i, t, w, flag, verdict, sc, cat, s[:110]))
