#!/usr/bin/env python3
"""pk.py <H|N> <addr> [src] : compile a row as vland does (catalog flags, its // Leaf:) at ONE work path (work/g/<addr>), keep the object as
work/obj/<H|N>_<addr>.obj, grade it, and run BOTH pick-any checkers (pk_all.check + pickany_vs_retail.py) -> evidence/pk_<H|N>_<addr>.txt.
H = HEAD source with the repo headers; N = rows/<addr>.cpp (or [src]) with FABLE_HDR_OVERLAY=<agent>/hdr (set by this tool).
Other kinds (P*, B, M, D) take [src] with the repo headers. PK_OVERLAY=<tree> overrides the header overlay for any kind
(B's staged row: its hdr/<addr>); PK_LEN=<n> supplies the oracle length for a row missing from auto-re-candidates.tsv
(bytes are then read from the retail image in memory only). Lock must be NF343's. Launch in the background with `timeout 900` (agent NF343)."""
import os, re, sys, shutil, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF343'
R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.dont_write_bytecode = True
from pathlib import Path
import nf
kind, a = sys.argv[1], sys.argv[2].lower().replace('0x', '')
if os.environ.get('PK_OVERLAY'):
    os.environ['FABLE_HDR_OVERLAY'] = os.environ['PK_OVERLAY']
elif kind == 'N':
    os.environ['FABLE_HDR_OVERLAY'] = A + '/hdr'
else:
    os.environ.pop('FABLE_HDR_OVERLAY', None)
import verify_and_land as V, pk_all
if nf.lock_owner(a) != 'NF343': print('NOLOCK', a); sys.exit(1)
src = Path(sys.argv[3]) if len(sys.argv) > 3 else (nf.src_of(a) if kind == 'H' else Path(A) / 'rows' / (a + '.cpp'))
text = src.read_text(encoding='utf-8')
if a in nf.catalog(): retail = bytes.fromhex(nf.catalog()[a]['bytes'])
else:  # staged row not in the catalog (B's 0x00ED8680): PK_LEN bytes straight from the retail image
    va = int(a, 16); n = int(os.environ['PK_LEN'])
    retail = next(d[va - b:va - b + n] for (nm, b, d, ch) in nf.exe() if b <= va < b + len(d))
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = nf.leaf_of(text, a); fl = nf.flags_of(a, text)
wd = Path(A) / 'work' / 'g' / a; wd.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=tuple(fl))
obj = wd / (a + '.obj'); dst = Path(A) / 'work' / 'obj' / ('%s_%s.obj' % (kind, a)); dst.parent.mkdir(exist_ok=True)
out = open(Path(A) / 'evidence' / ('pk_%s_%s.txt' % (kind, a)), 'w')
print('# %s %s src=%s grade=%s leaf=%s flags=%s overlay=%s' % (kind, a, src, st, leaf, ' '.join(fl), os.environ.get('FABLE_HDR_OVERLAY', '-')), file=out)
if obj.exists():
    shutil.copy(obj, dst)
    res = pk_all.check(str(obj), a, leaf=leaf, row_length=len(retail), use_leaf_relocs=st.startswith(('MATCH', 'RELOCATION')))
    print('# pk_all leaf %s source_a=%s %s' % (res['leaf'], res['leaf_ok'], res['leaf_note']), file=out)
    for l in res['lines']:
        print('PK  ' + '  '.join(str(l[f]) for f in ('verdict', 'status', 'address', 'sources', 'kind', 'size')) + '  leaf=%s reach=%s %s' % (l['leafref'], l['reach'], l['symbol']), file=out)
    o = subprocess.run([sys.executable, '-I', '-B', R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py',
                        os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', str(dst), a], capture_output=True, text=True)
    print('# old checker (pickany_vs_retail.py)', file=out)
    for l in o.stdout.splitlines(): print('OLD ' + l, file=out)
    if o.stderr.strip(): print('# old checker stderr: ' + o.stderr[-300:].replace('\n', ' | '), file=out)
else:
    print('NO_OBJECT', file=out)
out.close()
subprocess.run(['python3', R + '/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, str(wd)], capture_output=True)
print(kind, a, st)
