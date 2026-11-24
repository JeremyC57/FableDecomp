# pkrun349.py <H|N|R|W> <objdir> <outdir> <addr...> (NF352: R / W read <addr>_R.obj / <addr>_W.obj and write R_ / W_ files): run pk_all.py (--tsv, --row-length = catalog length, --leaf = the source's // Leaf)
# and pickany_vs_retail.py on <objdir>/(H_)<addr>.obj, writing <outdir>/<H|N>_<addr>.pkall.tsv / .pvr.txt. Asserts source_a=yes
# in the pk_all header (prints SRC_A_MISSING otherwise). Leaf from the HEAD landed source (the define does not change it).
import sys, os, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'
PK = R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py'
PVR = R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'
kind, od, out = sys.argv[1], os.path.abspath(sys.argv[2]), os.path.abspath(sys.argv[3]); os.makedirs(out, exist_ok=True)
env = dict(os.environ, FABLE_ROOT=R)
for a in sys.argv[4:]:
    obj = '%s/%s%s%s.obj' % (od, 'H_' if kind == 'H' else '', a, '_' + kind.lower() if kind in ('R', 'W', 'X') else '')
    if not os.path.exists(obj): print('MISSING', a); continue
    src = nf.src_of(a).read_text(encoding='utf-8')
    leaf = nf.leaf_of(src, a); ln = nf.catalog()[a]['length']
    base = '%s/%s_%s' % (out, kind, a)
    r = subprocess.run(['python3', '-I', '-B', PK, obj, a, '--leaf=' + leaf, '--tsv', '--row-length=' + str(int(ln))], capture_output=True, text=True, cwd=R, env=env)
    open(base + '.pkall.tsv', 'w').write(r.stdout + r.stderr)
    if 'source_a=yes' not in (r.stdout.splitlines() or [''])[0]: print('SRC_A_MISSING', kind, a)
    r = subprocess.run(['python3', '-I', '-B', PVR, os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', obj, a, '--leaf=' + leaf],
                       capture_output=True, text=True, cwd=R, env=env)
    open(base + '.pvr.txt', 'w').write(r.stdout + r.stderr)
print('PKRUN_DONE', kind, len(sys.argv) - 4)
