#!/usr/bin/env python3
"""cf.py <src.cpp> [tag]: compile-only fitness (no grade): cl with the verifier's BASE_FLAGS + the row's // Flags:, the
row's hdr/ overlay (FABLE_HDR_OVERLAY), into tools/srch/obj/<tag>.obj; prints one line: tag size frame exact rf segs [list];
appends it to tools/srch/SEARCH.tsv. ADDR / LEAF / A env override the row (default: this dir's row)."""
import sys, os, re, subprocess, time
from pathlib import Path
here = Path(__file__).resolve().parent
A = Path(os.environ.get('A', here.parent.parent))
ADDR = os.environ.get('ADDR', '006096e0')
LEAF = os.environ.get('LEAF', '?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z')
os.environ.setdefault('FABLE_HDR_OVERLAY', str(A / 'hdr'))
os.environ.setdefault('FABLE_OBJDUMP', 'objdump')
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src = Path(sys.argv[1]); tag = sys.argv[2] if len(sys.argv) > 2 else src.stem
text = src.read_text()
m = re.search(r'^// Flags: (.+)$', text, re.M); flags = m.group(1).split() if m else []
od = here / 'obj'; od.mkdir(exist_ok=True)
work = od / tag; work.mkdir(exist_ok=True)
sp = work / f'{ADDR}.cpp'; sp.write_text(V.vc71(text), encoding='utf-8')
obj = od / f'{tag}.obj'; obj.unlink(missing_ok=True)
t0 = time.time()
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + flags + [f"/Fo{obj}", str(sp)], V.env())
sp.unlink(); work.rmdir()
if cp.returncode or not obj.exists():
    print(tag, 'SRC_FAIL'); print('\n'.join(l for l in cp.stdout.splitlines() if 'error' in l)[:1500]); sys.exit(1)
cx = here.parent / 'cmpx.py'
env = dict(os.environ)
r1 = subprocess.run([sys.executable, str(cx), str(obj), ADDR, LEAF], capture_output=True, text=True, env=env).stdout.strip()
r2 = subprocess.run([sys.executable, str(cx), str(obj), ADDR, LEAF, '--segs', '--mask', 'frame'], capture_output=True, text=True, env=env).stdout
segs = [l.split()[1] for l in r2.splitlines() if l.startswith('seg ')]
mm = re.search(r'frame esp, (0x[0-9a-f]+) v.*exact ([0-9.]+%).*regs\+frame ([0-9.]+%)', r1)
sz = re.search(r'(\d+) v (\d+)', r1)
line = f"{tag}\t{sz.group(0) if sz else '?'}\tfr {mm.group(1) if mm else '?'}\tex {mm.group(2) if mm else '?'}\trf {mm.group(3) if mm else '?'}\tsegs {len(segs)} [{','.join(segs)}]\t{time.time()-t0:.0f}s"
print(line)
with open(here / 'SEARCH.tsv', 'a') as f: f.write(time.strftime('%H:%M ') + line + '\n')
if not os.environ.get('KEEPOBJ'): pass
