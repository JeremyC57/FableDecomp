# gradefile.py (NF326) <tag> <addr>=<source> ...: compile + grade arbitrary source files against an address's catalog oracle (nf.compile_one),
# copying each object to work/obj/<tag>_<addr>.obj; log work/gf_<tag>.log. The row lock must be NF326's. Launch in the background, timeout 900.
import sys, shutil, subprocess
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
tag = sys.argv[1]; log = open(nf.W / ('gf_%s.log' % tag), 'a')
for x in sys.argv[2:]:
    a, p = x.split('=', 1)
    if nf.lock_owner(a) != 'NF326': print('NOLOCK', a, file=log); log.flush(); continue
    st, leaf, fl, obj, wd = nf.compile_one(a, p, tag)
    dst = nf.W / 'obj' / ('%s_%s.obj' % (tag, a)); dst.parent.mkdir(exist_ok=True)
    if obj.exists(): shutil.copy(obj, dst)
    print('%s %s %s leaf=%s src=%s' % (tag, a, st, leaf, p), file=log); log.flush()
    subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', str(nf.A), str(wd)], capture_output=True)
print('GF_DONE', file=log)
