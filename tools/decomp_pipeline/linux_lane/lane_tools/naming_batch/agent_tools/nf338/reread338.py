# reread338.py (NF338): scripted re-read of every ENTRIES target against T3_PORT_QUEUE.tsv; no compiles.
# Output work/reread.tsv: idx target fix klass t3_where(col2|col3unit|col3prose) t3_sources unit_kind stale_hint reason(first 200 chars)
# stale_hint marks a reason whose blocker a later ruling or landing may have removed (needs a human / later agent to confirm):
#   ptr2ref (NF313 Q2 / NF324 Q3), enum (NF300 Q1 / NF307 D3), rescope (NF300 Q2), notcompiled (census / witness not compiled), hdr (header now declares?)
import re, os, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
Q = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv', encoding='utf-8')][1:]
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv', encoding='utf-8')][1:]
H8 = r'(?<![0-9A-Fa-f])(?:0x)?([0-9A-Fa-f]{8})(?![0-9A-Fa-f])'
by2 = collections.defaultdict(list); by3 = collections.defaultdict(list)
for q in Q:
    if len(q) < 2: continue
    m = re.match(r'0x([0-9A-Fa-f]{8})', q[1])
    if m: by2[m.group(1).lower()].append(q)
    if len(q) > 2:
        for x in set(re.findall(H8, q[2])): by3[x.lower()].append(q)
def kind(q):
    u = (q[2] if len(q) > 2 else '').lower()
    if 'family unit' in u: return 'family-unit'
    if u.startswith('deferred'): return 'deferred-unit'
    if 'identity' in u or 'identity' in (q[3] if len(q) > 3 else '').lower(): return 'identity'
    if u.strip() in ('port', 'standin_alias port', 'signature port', 'other port') or u.endswith('port'): return 'port'
    return 'prose:' + u[:30]
HINT = [('ptr2ref', r'pointer|reference|PAV|AAV|by pointer'), ('enum', r'\benum|literal'), ('rescope', r'nested|namespace|re-scope|rescope|@1@'),
        ('notcompiled', r'not compiled|not verified|not opened|cited'), ('hdr', r'header')]
o = open(A + '/work/reread.tsv', 'w')
o.write('idx\ttarget\tfix\tklass\tt3_where\tt3_sources\tunit_kind\tstale_hint\treason\n')
cnt = collections.Counter()
for i, e in enumerate(E, 1):
    t = e[0].lower(); qs = by2.get(t, []); where = 'col2'
    if not qs:
        qs = by3.get(t, []); where = 'col3unit' if any(('unit' in q[2].lower() or q[1].lower().startswith('deferred')) for q in qs) else 'col3prose'
    txt = ' | '.join((q[2] + ' :: ' + (q[3] if len(q) > 3 else '')) for q in qs)
    hints = [h for h, rx in HINT if re.search(rx, txt, re.I)]
    k = ','.join(sorted(set(kind(q) for q in qs)))
    cnt[where] += 1; cnt['kind:' + k.split(',')[0].split(':')[0]] += 1
    o.write('\t'.join([str(i), t, e[3], e[4], where, ','.join(sorted(set(q[0] for q in qs))), k, ','.join(hints) or '-', txt[:200].replace('\t', ' ')]) + '\n')
print(dict(cnt))
