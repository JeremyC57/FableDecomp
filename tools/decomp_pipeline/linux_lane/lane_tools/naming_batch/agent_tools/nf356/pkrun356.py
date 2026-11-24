# pkrun356.py <outdir> <addr> <label>=<obj>[=<leaf>] ...: pkrun352 with a leaf PER SIDE (NF356: 0x0042AF3C's HEAD leaf is the stand-in
# ?GetDef_0042af3c@..., its new leaf the PDB symbol). Default leaf = the HEAD source's // Leaf (catalog name when absent). Runs pk_all.py
# (--tsv, --row-length = catalog length, --leaf) and pickany_vs_retail.py (--leaf) on each object -> <outdir>/<label>_<addr>.{pkall.tsv,pvr.txt}.
import sys, os, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'
PK = R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py'
PVR = R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'
assert os.environ.get('FABLE_EXE') and os.environ.get('FABLE_ROOT') == R, 'set FABLE_EXE and FABLE_ROOT first'
out, a = os.path.abspath(sys.argv[1]), sys.argv[2]; os.makedirs(out, exist_ok=True)
src = nf.src_of(a).read_text(encoding='utf-8'); dleaf = nf.leaf_of(src, a); ln = len(bytes.fromhex(nf.catalog()[a]['bytes']))
for spec in sys.argv[3:]:
    p = spec.split('=', 2); lab, obj = p[0], p[1]; leaf = p[2] if len(p) > 2 else dleaf; base = '%s/%s_%s' % (out, lab, a)
    r = subprocess.run(['python3', '-I', '-B', PK, obj, a, '--leaf=' + leaf, '--tsv', '--row-length=' + str(ln)], capture_output=True, text=True, cwd=R)
    open(base + '.pkall.tsv', 'w').write(r.stdout + r.stderr)
    if 'source_a=yes' not in (r.stdout.splitlines() or [''])[0]: print('SRC_A_MISSING', lab, a)
    r = subprocess.run(['python3', '-I', '-B', PVR, os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', obj, a, '--leaf=' + leaf], capture_output=True, text=True, cwd=R)
    open(base + '.pvr.txt', 'w').write(r.stdout + r.stderr)
print('PKRUN_DONE', a, len(sys.argv) - 3)
