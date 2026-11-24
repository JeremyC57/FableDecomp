#!/usr/bin/env python3
"""pk.py <H|N> <addr...> : compile each row as vland does (catalog flags, its // Leaf:) at ONE work path (work/g/<addr>, H and N sequential),
keep the object as work/obj/<H|N>_<addr>.obj, record the grade (work/grades_<H|N>.log), then run BOTH pick-any checkers from the repo:
pk_all.py (--tsv --leaf=<full Leaf> --row-length=<catalog length>) and pickany_vs_retail.py (--leaf) -> evidence/pk/<H|N>_<addr>.{pkall.tsv,pvr.txt}.
Asserts that pk_all's `# leaf` line names the row's own Leaf (LEAF_MISMATCH otherwise). H = landed source, N = rows/<addr>.cpp.
No header overlay (FABLE_HDR_OVERLAY unset). Lock must be NF353's. Run in the background; one `timeout 900` per row (agent NF353)."""
import os, re, sys, shutil, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353'
R = '/home/user/FableDecomp'
os.environ['FABLE_ROOT'] = R; os.environ.pop('FABLE_HDR_OVERLAY', None)
sys.path.insert(0, A + '/tools'); sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.dont_write_bytecode = True
from pathlib import Path
import nf
import verify_and_land as V
PK = R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py'
PVR = R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'
kind, a = sys.argv[1], sys.argv[2].lower().replace('0x', '')
log = open(A + '/work/grades_%s.log' % kind, 'a')
if nf.lock_owner(a) != 'NF353': print('NOLOCK', a, file=log); sys.exit(1)
src = nf.src_of(a) if kind == 'H' else Path(A) / 'rows' / (a + '.cpp')
text = src.read_text(encoding='utf-8')
retail = bytes.fromhex(nf.catalog()[a]['bytes'])
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = nf.leaf_of(text, a); fl = nf.flags_of(a, text)
wd = Path(A) / 'work' / 'g' / a; wd.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=tuple(fl))
obj = wd / (a + '.obj'); dst = Path(A) / 'work' / 'obj' / ('%s_%s.obj' % (kind, a))
if obj.exists(): shutil.copy(obj, dst)
print('%s %s %s leaf=%s flags=%s src=%s' % (kind, a, st, leaf, ' '.join(fl) or '-', src), file=log); log.flush()
subprocess.run(['python3', R + '/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, str(wd)], capture_output=True)
if obj.exists() or dst.exists():
    ln = int(nf.catalog()[a]['length']); base = '%s/evidence/pk/%s_%s' % (A, kind, a); env = dict(os.environ)
    r = subprocess.run(['python3', '-I', '-B', PK, str(dst), a, '--leaf=' + leaf, '--tsv', '--row-length=%d' % ln], capture_output=True, text=True, cwd=R, env=env)
    open(base + '.pkall.tsv', 'w').write(r.stdout + r.stderr)
    first = (r.stdout.splitlines() or [''])[0]
    m = re.match(r'# leaf (\S+)', first)
    print('%s %s PKLEAF %s %s' % (kind, a, 'OK' if m and m.group(1) == leaf else 'LEAF_MISMATCH', first[:200]), file=log)
    r = subprocess.run(['python3', '-I', '-B', PVR, os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', str(dst), a, '--leaf=' + leaf],
                       capture_output=True, text=True, cwd=R, env=env)
    open(base + '.pvr.txt', 'w').write(r.stdout + r.stderr)
else:
    print('%s %s NO_OBJECT' % (kind, a), file=log)
log.close()
