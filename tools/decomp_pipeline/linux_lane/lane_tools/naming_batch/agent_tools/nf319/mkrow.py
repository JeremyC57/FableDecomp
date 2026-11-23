# mkrow.py <spec.tsv>: alias-row drafts. spec lines: idx <TAB> defines (comma list, in order) <TAB> definition head (e.g.
# `EAIStateGroupType CAIStateGroup_X::GetStateGroupType() const`) [<TAB> extra includes, comma list, placed before the row's own].
# From the HEAD row (catalog Source): drop the alias `struct/class X_<Alias> : X { ... };` block, replace the alias definition head
# (`R X_<Alias>::M(...) [const]`, up to the `{`) by the given head, Leaf -> pdb_spelling, `#define`s first (after the leading comment
# lines), the stale stand-in trailer -> one provenance line. Every other HEAD line is kept. Writes rows/<addr>.cpp; saves orig/<addr>.cpp.
import re, sys, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
R = Path('/home/user/FableDecomp/rebuild/src/compiled')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
FD = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas_decl.tsv')}
for l in open(sys.argv[1]):
    if not l.strip() or l.startswith('#'): continue
    p = l.rstrip('\n').split('\t'); i = int(p[0]); defs = [x for x in p[1].split(',') if x]; head = p[2]
    incs = [x for x in p[3].split(',') if x] if len(p) > 3 else []
    e = E[i - 1]; a = e[0]; pdb = e[3]; cur = e[4]; src = R / e[7]
    m = re.match(r'\?([^@]+)@([^@]+)@@', cur); M, AL = m.group(1), m.group(2)
    X = re.match(r'\?[^@]+@([^@]+)@@', pdb).group(1)
    t = src.read_text(encoding='utf-8'); shutil.copy(src, A / 'orig' / (a + '.cpp'))
    t2, n = re.subn(r'\n?[ \t]*(?:struct|class)\s+' + re.escape(AL) + r'\s*:\s*(?:public\s+)?' + re.escape(X) + r'\s*\{[^{}]*\}\s*;[ \t]*', '', t, count=1)
    if n != 1: print(i, a, 'NO_ALIAS_BLOCK'); continue
    hm = re.search(r'^[^\n#/]*\b' + re.escape(AL) + r'::' + re.escape(M) + r'\s*\([^)]*\)\s*(const\s*)?(?=\{)', t2, re.M)
    if not hm: print(i, a, 'NO_DEF_HEAD'); continue
    # keep the HEAD parameter NAMES, take the PDB types from the given head
    hp = re.search(r'\(([^)]*)\)', hm.group(0)).group(1).strip(); np_ = re.search(r'\(([^)]*)\)', head).group(1).strip()
    hps = [x.strip() for x in hp.split(',')] if hp and hp != 'void' else []; nps = [x.strip() for x in np_.split(',')] if np_ else []
    if len(hps) == len(nps) and hps:
        nm = []
        for x, y in zip(hps, nps):
            mm = re.search(r'(\w+)\s*$', x)
            nm.append(y + ' ' + mm.group(1) if mm and not re.fullmatch(r'(const\s+)?(unsigned\s+)?\w+\s*[&*]*', x.replace(mm.group(1), '').strip() or 'x') is None and mm.group(1) not in ('int', 'long', 'char', 'bool', 'float', 'void') and not re.search(r'[&*]\s*$', x) or (mm and re.search(r'[\w&*]\s+\w+\s*$', x)) else y)
        head = head[:head.index('(') + 1] + ', '.join(nm) + head[head.rindex(')'):]
    elif len(hps) != len(nps): print(i, a, 'PARAM_COUNT', hps, nps)
    t2 = t2[:hm.start()] + head + t2[hm.end():]
    rt = re.match(r'\s*(E\w+)\s', head)
    if rt: t2 = re.sub(r'return\s+(\d+|0x[0-9a-fA-F]+)\s*;', r'return (%s)\1;' % rt.group(1), t2)
    L = t2.split('\n')
    L = [('// Leaf: ' + pdb) if x.startswith('// Leaf:') else x for x in L]
    L = [x for x in L if not x.startswith('// Stand-in (owner rule 2026-11-05)')]
    k = 0
    while k < len(L) and L[k].startswith('//'): k += 1
    prov = ['// Header route (NF319, 2026-11-23): defined on the real class of engine/%s.h (was %s); the retail RTTI vftable' % (X, cur),
            '//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in',
            ('//   FABLE_RETAIL_%s_RTTI_DECLS. Bytes unchanged.' % X.upper()) if i not in FD else ('//   declared there under %s (switches landed rows define). Bytes unchanged.' % ' + '.join(FD[i][3].split(',')))]
    if i in FD: prov[1] = prov[1].replace('; declared by the add-only opt-in', ';')
    body = L[k:]
    own_inc = [x for x in body if re.match(r'\s*#\s*include', x)]
    body = [x for x in body if not re.match(r'\s*#\s*include', x)]
    # keep pragmas / code order; includes go after the defines, extra includes first
    inc_lines = [(x if re.match(r'(class|struct|enum) ', x) else '#include "%s"' % x) for x in incs if not any(x in y for y in own_inc)] + own_inc
    pre_body = []
    while body and (body[0].startswith('#pragma') or body[0].startswith('//') or not body[0].strip()): pre_body.append(body.pop(0))
    if any(AL in x for x in body if not x.startswith('//')): print(i, a, 'ALIAS_LEFT')
    out = L[:k] + prov + ['#define ' + d for d in defs] + pre_body + inc_lines + body
    (A / 'rows' / (a + '.cpp')).write_text('\n'.join(out), encoding='utf-8')
    print(i, a, 'OK')
