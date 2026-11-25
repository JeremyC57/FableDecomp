#!/usr/bin/env python3
# hobj.py <outdir> <srcdir|HEAD> <addr...>: compile rows (HEAD repo source, or <srcdir>/<a>.cpp) with the catalog flags, grade vs catalog, copy obj.
import sys, os, shutil, subprocess
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
from pathlib import Path
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True); sd = sys.argv[2]; addrs = sys.argv[3:]
nf.blk("00000000"); nf.catalog()
def one(a):
    p = nf.src_of(a) if sd == 'HEAD' else Path(sd) / (a + '.cpp')
    try:
        st, leaf, fl, obj, wd = nf.compile_one(a, p, out.name)
    except Exception as e:
        return '%s ERR %s' % (a, e)
    if obj.exists(): shutil.copy(obj, out / (a + '.obj'))
    subprocess.run(['python3', str(nf.R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(nf.A), str(wd)], capture_output=True)
    return '%s %s leaf=%s flags=%s' % (a, st, leaf, ' '.join(fl))
with ThreadPoolExecutor(int(os.environ.get('J', '6'))) as ex:
    for r in ex.map(one, addrs): print(r, flush=True)
print('HOBJ_DONE')
