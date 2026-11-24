#!/usr/bin/env python3
"""one.py <src.cpp> <row-address> <workdir> [extra cl flags...] (env ROWLEN = retail length for an unlanded row) : compile one source as vland would (V.vc71, BASE + flags, its
`// Leaf:`), print the grade, pk_all.check and the official pickany_vs_retail.py output. Keeps the object in <workdir>.
Validation helper (agent NF340). python3 -I -B, lane env sourced."""
import os, re, subprocess, sys
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp'); HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'decomp_pipeline')); sys.path.insert(0, HERE); sys.dont_write_bytecode = True
from pathlib import Path
import verify_and_land as V, pk_all
src, a, work = sys.argv[1:4]; extra = sys.argv[4:]; a = a.lower().replace('0x', '')
text = open(src, encoding='utf-8').read()
m = re.search(r'^//\s*Leaf:\s*(\S+)', text, re.M); leaf = m.group(1) if m else None
n = int(os.environ['ROWLEN']) if os.environ.get('ROWLEN') else pk_all.tables()[2][a]; retail = pk_all.rd(int(a, 16), n)
wd = Path(work); wd.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=extra)
print('grade', st, 'flags', ' '.join(V.BASE_FLAGS + extra))
obj = wd / f'{a}.obj'
res = pk_all.check(str(obj), a, leaf=leaf, row_length=n, use_leaf_relocs=st.startswith(('MATCH', 'RELOCATION')))
print(f'# pk_all leaf {res["leaf"]} source_a={res["leaf_ok"]} {res["leaf_note"]}')
for l in res['lines']:
    print('  ' + '  '.join(str(l[f]) for f in ('verdict', 'status', 'address', 'sources', 'kind', 'size')) + f"  leaf={l['leafref']} reach={l['reach']} {l['symbol'][:120]}")
o = subprocess.run([sys.executable, '-I', '-B', os.path.join(ROOT, 'rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'),
                    os.environ['FABLE_EXE'], os.path.join(ROOT, 'rebuild/oracles/reloc_bindings.tsv'), str(obj), a], capture_output=True, text=True)
print('# old checker (pickany_vs_retail.py)'); print(o.stdout.rstrip() or '  (no lines)'); print(o.stderr[-300:].rstrip())
