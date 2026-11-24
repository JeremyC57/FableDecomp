# mkpay.py <H|N> <prefix> <addr...>: vland --recheck-landed payload + oracle (nf.py pay form). H = landed source + landed test (HEAD text,
# asserted equal to the repo file); N = rows/<a>.cpp + the landed test (tests unchanged). The oracle holds retail bytes: work/ only. Agent NF353.
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
kind, prefix, addrs = sys.argv[1], sys.argv[2], sys.argv[3:]
R = str(nf.R); cat = nf.catalog(); out = []; orc = []
hdr = open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8').readline().rstrip('\n')
lines = {l.split('\t')[0]: l.rstrip('\n') for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8')}
for a in addrs:
    s = (nf.src_of(a) if kind == 'H' else nf.A / 'rows' / (a + '.cpp')).read_text(encoding='utf-8')
    t = nf.test_of(a).read_text(encoding='utf-8')
    nm = cat[a]['name']; d = 0; cut = 0
    for k, ch in enumerate(nm):
        if ch == '<': d += 1
        elif ch == '>': d -= 1
        elif ch == ':' and d == 0 and nm[k:k + 2] == '::': cut = k + 2
    lf = re.search(r'// Leaf:\s*(\S+)', s)
    out.append(dict(address=a, module=nf.field(a, 'Module'), name=nm[cut:], source_cpp=s, test_cpp=t, pass_pattern=nf.field(a, 'PassPattern'),
                    flags=nf.flags_of(a, s), leaf=lf.group(1) if lf else nm[cut:]))
    orc.append(lines[a])
json.dump({'result': {'authored': out}}, open(prefix + '_payload.json', 'w'), indent=1)
open(prefix + '_oracle.tsv', 'w').write(hdr + '\n' + '\n'.join(orc) + '\n')
print(prefix, kind, len(out))
