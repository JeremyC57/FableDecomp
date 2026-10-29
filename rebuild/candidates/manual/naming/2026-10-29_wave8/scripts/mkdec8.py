# DECISIONS_W8.tsv: chunk 564/565 NAME_FIXES + ChatGPT pass 9 NAME_FIXES (+ its conflicts file) + wave-8 settlements,
# merged one row per retail address and checked against the Ego_r / FableWin name lists (exact; a `_W` proposal is checked
# in its `_W` form and lands in the `G` form, owner 2026-10-29). Columns as DECISIONS_W7.tsv plus `identity`
# (same = every bound spelling already names the same function; change = some bound spelling names another function)
# and `w8` (what wave 8 did with it).
import csv, collections, os, json, sys
os.chdir(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, 'tools'); import qn
from targets import FINAL, CONF, REJECT, MANUAL_PROVEN, APPLIED
ROOT = '/home/user/FableDecomp'
def names(p):
    s = set()
    for l in open(p, encoding='utf-8', errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: s.add(q[1])
    return s
EG = names(ROOT + '/ghidra_out/egor_pdb_names.tsv'); FW = names(ROOT + '/ghidra_out/fablewin_pdb_names.tsv')
U = json.load(open('und.json'))
bound = collections.defaultdict(set)
for r in csv.reader(open(ROOT + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    if len(r) >= 2 and r[0] != 'symbol': bound[r[1]].add(r[0])
props = collections.defaultdict(list)
for l in open('QUEUE_564_565.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 3: props[q[1].lower().replace('0x', '').zfill(8)].append(('chunk' + q[0], q[2], q[3] if len(q) > 3 else '', q[4] if len(q) > 4 else ''))
for l in list(open('GPT_PASS9_NAME_FIXES.tsv'))[1:]:
    q = l.rstrip('\n').split('\t')
    props[q[0].lower().zfill(8)].append(('pass9', q[1], q[2], q[3] if len(q) > 3 else ''))
for l in list(open('GPT_PASS9_NAME_FIX_CONFLICTS.tsv'))[1:]:
    q = l.rstrip('\n').split('\t')
    props[q[0].lower().zfill(8)].append(('pass9-conflict', q[1], q[2], q[3] if len(q) > 3 else ''))
for a in FINAL:
    if a not in props: props[a].append(('w8', FINAL[a], CONF.get(a, ''), ''))
out = ['\t'.join(['address', 'final_symbol', 'confidence', 'sources', 'chunks', 'identity', 'w8', 'wrong_spellings', 'bound_wrong_symbols', 'evidence'])]
cnt = collections.Counter()
for a in sorted(props):
    ps = props[a]
    if a in FINAL: s = FINAL[a]
    else:
        c = collections.Counter(p[1].replace('_W', 'G') for p in ps if p[0] != 'pass9-conflict')
        s = c.most_common(1)[0][0] if c else ps[0][1]
    orig = next((p[1] for p in ps if p[1].replace('_W', 'G') == s), s)
    src = '+'.join(x for x, S in (('Ego_r.pdb', EG), ('FableWin.pdb', FW)) if orig in S) or '-'
    wrong = sorted(bound.get(a, set()) - {s})
    fb = qn.base(qn.qname(U.get(s, s)))[0]
    idc = 'change' if any(qn.base(qn.qname(U.get(w, w)))[0] != fb for w in wrong) else 'same'
    if a in REJECT and a not in FINAL: conf = 'rejected'
    elif a in MANUAL_PROVEN: conf = 'proven'
    elif src == 'Ego_r.pdb+FableWin.pdb' and idc == 'same': conf = 'proven'
    elif src != '-' and idc == 'same': conf = 'proven-1pdb'
    elif src != '-': conf = 'pdb-spelling, identity unverified'
    else: conf = 'probable'
    if '_W' in orig and orig != s: src += ' (_W form; lands as G)'
    ev = [p[2] for p in ps if p[2]]
    if a in CONF: ev.insert(0, 'W8: ' + CONF[a])
    if a in REJECT: ev.insert(0, 'W8 REJECTED: ' + REJECT[a])
    cnt[conf] += 1
    out.append('\t'.join([a, s, conf, src, ','.join(sorted(set(p[0] for p in ps))), idc, APPLIED.get(a) or ('rejected' if conf == 'rejected' else ('no change needed: every bound spelling is already the final' if not wrong else 'open: %d bound spelling(s) differ; not reached in wave 8' % len(wrong))),
                          ' ; '.join(sorted(set(p[3] for p in ps if p[3])))[:2000].replace('\t', ' '), '|'.join(wrong),
                          ' | '.join(ev)[:3000].replace('\t', ' ')]))
open('DECISIONS_W8.tsv', 'w').write('\n'.join(out) + '\n')
print(cnt)
