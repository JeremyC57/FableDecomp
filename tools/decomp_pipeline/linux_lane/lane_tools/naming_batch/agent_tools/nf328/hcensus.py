# hcensus.py (NF328) <tag> <listfile>: HEAD census compiles. For each listed row whose lock is NF328's AND whose HEAD source passed its
# precheck in an earlier call (work/pre.log clang PASS for H_<addr>, or work/pre_zs.log ZS_PASS for H_<addr>), compile + grade the landed
# source at the SAME work path pairgrade.py uses (work/g/<addr>_P) and keep the object as work/pobj/H_<addr>.obj.
# -> work/hc_<tag>.log (grade per row; SKIP lines say why), work/hc_<tag>.done. Background, timeout 900 on the launch line.
import sys, shutil, subprocess, re
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
tag, lst = sys.argv[1], sys.argv[2]
ok = set()
for l in open(nf.W / 'pre.log'):
    m = re.match(r'\S+ H_([0-9a-f]{8}) PASS', l)
    if m: ok.add(m.group(1))
try:
    for l in open(nf.W / 'pre_zs.log'):
        m = re.match(r'\S+ ZS_PASS H_([0-9a-f]{8}) ', l)
        if m: ok.add(m.group(1))
except OSError: pass
log = open(nf.W / ('hc_%s.log' % tag), 'a'); (nf.W / 'pobj').mkdir(exist_ok=True)
for a in open(lst).read().split():
    if nf.lock_owner(a) != 'NF328': print('SKIP NOLOCK', a, file=log); log.flush(); continue
    if a not in ok: print('SKIP NOPRE', a, file=log); log.flush(); continue
    st, leaf, fl, obj, wd = nf.compile_one(a, nf.src_of(a), 'P')
    if obj.exists(): shutil.copy(obj, nf.W / 'pobj' / ('H_' + a + '.obj'))
    print('H %s %s leaf=%s' % (a, st, leaf), file=log); log.flush()
    subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', str(nf.A), str(wd)], capture_output=True)
open(nf.W / ('hc_%s.done' % tag), 'w').write('done\n')
