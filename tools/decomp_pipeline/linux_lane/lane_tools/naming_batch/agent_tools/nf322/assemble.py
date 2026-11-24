# assemble.py: RENAMES.tsv (work/ren1.tsv + ren2.tsv), STATUS.tsv + APPLIED.tsv (ENTRIES + work/st.tsv; unreached lines `open`),
# TABLE_CHANGES.tsv (every table line whose witness is a moved caller: replace when no line for the new spelling exists, else delete;
# plus work/tc_extra.tsv), rows.txt (rows/*.cpp)
import collections, os
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF322'
r1 = [l for l in open(A + '/work/ren1.tsv').read().split('\n') if l.strip()]
r2 = [l for l in open(A + '/work/ren2.tsv').read().split('\n') if l.strip()]
open(A + '/RENAMES.tsv', 'w').write('address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\n' + '\n'.join(r1) +
    '\n# Section 2\ncaller\told_symbol\tnew_symbol\tcallee\n' + '\n'.join(r2) + '\n')
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
st = {}
for l in open(A + '/work/st.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 5: st[p[0]] = p[1:5]
so = ['idx\taddress\tclass\ttier\tstatus\trows_changed\tspellings_before_after\tnote']; ap = ['entry\taddress\tstatus\tbinders_respelled\tnote']
for i, e in enumerate(E, 1):
    s = st.get(str(i), ['open', '', '', 'not started'])
    so.append('%d\t0x%s\t%s\t%s\t%s\t%s\t%s\t%s' % (i, e[0].upper(), e[4], e[2], s[0], s[1], s[2], s[3]))
    ap.append('%d\t0x%s\t%s\t%s\t%s' % (i, e[0].upper(), s[0], s[1], s[3]))
open(A + '/STATUS.tsv', 'w').write('\n'.join(so) + '\n'); open(A + '/APPLIED.tsv', 'w').write('\n'.join(ap) + '\n')
T = collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    r = l.rstrip('\n').split('\t')
    if len(r) == 3: T[(r[0], r[1].lower())].append(r[2].lower())
out = ['action\tsymbol\ttarget\twitness\tnote']; seen = set()
# work/tc_skip.tsv: (old symbol, target) pairs whose generated line is replaced by a hand line in tc_extra.tsv (e.g. a rewitness)
if os.path.exists(A + '/work/tc_skip.tsv'):
    for l in open(A + '/work/tc_skip.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 2: seen.add((p[0], p[1].lower()))
for x in r2:
    caller, old, new, callee = x.split('\t'); c = callee[2:].lower(); w = caller[2:].lower()
    if (old, c) in T and w in T[(old, c)] and (old, c) not in seen:
        seen.add((old, c))
        if (new, c) in T: out.append('delete\t%s\t%s\t%s\twitness moved to %s; the (%s) line stays (witness %s)' % (old, c, w, new, new, T[(new, c)][0]))
        else: out.append('replace\t%s -> %s\t%s\t%s\twitness now binds the PDB spelling; no line for it yet' % (old, new, c, w))
if os.path.exists(A + '/work/tc_extra.tsv'): out += [l for l in open(A + '/work/tc_extra.tsv').read().split('\n') if l.strip()]
open(A + '/TABLE_CHANGES.tsv', 'w').write('\n'.join(out) + '\n')
rows = sorted(f[:-4] for f in os.listdir(A + '/rows') if f.endswith('.cpp') and not f.endswith('_test.cpp'))
open(A + '/rows.txt', 'w').write('\n'.join(rows) + '\n')
print(len(r1), 'self renames', len(r2), 'caller lines', len(out) - 1, 'table lines', len(rows), 'rows', collections.Counter(v[0] for v in st.values()))
