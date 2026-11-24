# tcgen.py <idx:old_spelling_regex_free_marker...>: for each ENTRIES idx, every table line at the target whose symbol != PDB:
# prints the TABLE_CHANGES line: replace (no PDB line yet at that target) or delete (a PDB line exists), witness kept.
import sys, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333'
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
T = collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T[p[1].lower().replace('0x', '')].append((p[0], p[2].lower().replace('0x', '')))
skip = set(x for x in sys.argv[1:] if not x.isdigit())   # witnesses NOT moved by this landing (kept lines)
for i in [int(x) for x in sys.argv[1:] if x.isdigit()]:
    e = E[i - 1]; t = e[0]; pdb = e[6]; lines = T.get(t, [])
    has = any(s == pdb for s, w in lines); done_replace = False
    for s, w in lines:
        if s == pdb or w in skip: continue
        if not has and not done_replace:
            print('\t'.join(['replace', '%s -> %s' % (s, pdb), t, w, 'witness now binds the PDB spelling; no line for it yet'])); done_replace = True
        else:
            print('\t'.join(['delete', s, t, w, 'witness now binds the PDB spelling, which already has a line at this target']))
