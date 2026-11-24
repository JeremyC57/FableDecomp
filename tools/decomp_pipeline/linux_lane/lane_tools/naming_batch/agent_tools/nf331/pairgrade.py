# pairgrade.py (NF326) <tag> <addr...>: HEAD (landed source) and new (rows/<addr>.cpp) compiled one after the other at the SAME work path
# (work/g/<addr>_P), so anonymous-namespace ?A0x hashes cannot differ between the two objects (GOTCHAS, coordinator 2026-11-23). Objects ->
# work/pobj/H_<addr>.obj and work/pobj/<addr>.obj; grades -> work/pg_<tag>.log. The row lock must be NF326's. Background, timeout 900.
import sys, shutil, subprocess
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
tag = sys.argv[1]; log = open(nf.W / ('pg_%s.log' % tag), 'a'); (nf.W / 'pobj').mkdir(exist_ok=True)
for a in sys.argv[2:]:
    if nf.lock_owner(a) != 'NF331': print('NOLOCK', a, file=log); log.flush(); continue
    for side, p in (('H', nf.src_of(a)), ('N', nf.A / 'rows' / (a + '.cpp'))):
        st, leaf, fl, obj, wd = nf.compile_one(a, p, 'P')
        dst = nf.W / 'pobj' / (('H_' if side == 'H' else '') + a + '.obj')
        if obj.exists(): shutil.copy(obj, dst)
        print('%s %s %s leaf=%s' % (side, a, st, leaf), file=log); log.flush()
        subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', str(nf.A), str(wd)], capture_output=True)
print('PG_DONE', file=log)
