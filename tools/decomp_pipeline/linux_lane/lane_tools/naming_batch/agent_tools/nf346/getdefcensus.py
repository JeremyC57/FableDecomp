# getdefcensus.py <obj>...: every by-global-index GetDef<T> COMDAT (??$GetDef@...@CDefinitionManager@@QBE_NJPAV...) an object defines,
# compared with its retail owner (reloc_bindings.tsv target of that exact symbol) by pickany.py: bytes with relocation fields zeroed.
# One line per (object, symbol): EQ / NEQ / NOOWNER, size.
import sys, os, re, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
T = collections.defaultdict(set)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    r = l.rstrip('\n').split('\t')
    if len(r) == 3: T[r[0]].add(r[1])
pat = re.compile(r'^\?\?\$GetDef@.*@CDefinitionManager@@QBE_NJPAV')
for o in sys.argv[1:]:
    secs, sl = nf.coff(o)
    for i, n, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and pat.match(n):
            own = sorted(T.get(n, ()))
            if not own: print('NOOWNER\t%s\t%s' % (os.path.basename(o), n)); continue
            for a in own:
                r = subprocess.run(['python3', os.path.join(os.path.dirname(os.path.abspath(__file__)), 'pickany.py'), o, n, a], capture_output=True, text=True)
                last = (r.stdout.strip().splitlines() or ['?'])[-1]
                m = re.search(r'size (\d+) bytes_equal (\w+)', last)
                bad = [l for l in r.stdout.splitlines() if 'NOT_IN_TABLE' in l]
                v_ = 'EQ' if m and m.group(2) == 'True' and not bad else 'NEQ'
                print('%s\t%s\t%s\t%s\tsize=%s%s' % (v_, os.path.basename(o), a, n, m.group(1) if m else '?', ' relocs-not-in-table' if bad else ''))
