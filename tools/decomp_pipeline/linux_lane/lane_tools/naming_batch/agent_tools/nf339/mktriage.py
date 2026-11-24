# mktriage.py: TRIAGE.tsv + STATUS.tsv skeleton from work/tri2.tsv (and work/census_def.tsv once the HEAD census ran)
import os, re, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF339'
rows = [l.rstrip('\n').split('\t') for l in open(A + '/work/tri2.tsv')][1:]
cen = {}
if os.path.exists(A + '/work/census_def.tsv'):
    for l in open(A + '/work/census_def.tsv').read().splitlines()[1:]:
        f = l.split('\t'); cen[f[0]] = f      # addr verdict grade defs
over = {}
if os.path.exists(A + '/work/triage_override.tsv'):
    for l in open(A + '/work/triage_override.tsv').read().splitlines():
        f = l.split('\t')
        if len(f) >= 4: over[(f[0], f[1])] = f[2:]
out = []
for r in rows:
    a, tag, state = r[0], r[1], r[2]
    flags = r[11]; other = ('; other T3 lines: ' + flags) if flags else ''
    if (a, tag) in over:
        sh, plan, why = over[(a, tag)][0], over[(a, tag)][1], over[(a, tag)][2] if len(over[(a, tag)]) > 2 else ''
        out.append([a, tag, why + other, sh, plan]); continue
    if state == 'already_pdb':
        c = cen.get(a)
        if c and c[1] == 'DEF_PDB':
            out.append([a, tag, f'today\'s row defines the PDB symbol (HEAD census: {c[2]}; Leaf = PDB; self_symbols.tsv of 03:05 is stale){other}', 'n', 'done-already: no source change; close the T3 line (decision D1)'])
        elif c:
            out.append([a, tag, f'Leaf = PDB but the HEAD object does not define it ({c[1]} {c[2]} {c[3][:120]}){other}', 'y', 'work'])
        else:
            out.append([a, tag, f'Leaf = PDB, row declaration {r[18][:100]}; self_symbols.tsv (stale?) says {r[13]}{other}', 'unknown', 'HEAD census (DEF symbols) first'])
    elif state == 'binder_line':
        out.append([a, tag, f'binder line: {r[12][:160]}{other}', 'unknown', 're-read binder'])
    else:
        out.append([a, tag, f'self still {r[3][:90]} (Leaf {r[5][:60] or "none"}; {r[13]}); PDB {r[16]}{other}', 'y', 'work'])
with open(A + '/TRIAGE.tsv', 'w') as f:
    f.write('address\ttag\treason\tstill_holds\tplan\n' + ''.join('\t'.join(x) + '\n' for x in out))
st = {}
if os.path.exists(A + '/STATUS.tsv'):
    for l in open(A + '/STATUS.tsv').read().splitlines()[1:]:
        f = l.split('\t'); st[(f[0], f[1])] = l
with open(A + '/STATUS.tsv', 'w') as f:
    f.write('address\ttag\tstatus\treason\n')
    for x in out:
        f.write(st.get((x[0], x[1]), f'{x[0]}\t{x[1]}\tpending\t{x[4]}') + '\n')
print(collections.Counter((x[3], x[4][:20]) for x in out))
