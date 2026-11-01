# vrun.py <payload> <oracle> <pairs.jsonl>: runs lane_tools/vland.py --recheck-landed unchanged, and records every graded row's
# relocation (symbol, retail target) pairs (reloc_identity.check results) to pairs.jsonl. Run from the repo root.
import sys, os, json, runpy
sys.path.insert(0, 'tools/decomp_pipeline')
import verify_and_land as V
pp = sys.argv[3]; F = open(pp, 'w')
_chk = V.reloc_identity.check
def check(root, objdump, objpath, leaf, addr, retail, roff=0, built=None):
    h, w, p = _chk(root, objdump, objpath, leaf, addr, retail, roff, built)
    F.write(json.dumps({'a': addr, 'pairs': sorted({(x[2], '%08x' % x[3]) for x in p})}) + '\n'); F.flush()
    return h, w, p
V.reloc_identity.check = check
sys.argv = ['vland.py', sys.argv[1], sys.argv[2], '--recheck-landed']
runpy.run_path('tools/decomp_pipeline/linux_lane/lane_tools/vland.py', run_name='__main__')
