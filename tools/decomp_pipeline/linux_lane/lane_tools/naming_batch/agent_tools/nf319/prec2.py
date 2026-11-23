# prec2.py <Class> <switch>[,<switch>..] [n]: landed rows defining every switch (no *_PASS<nn> define), own-member rows (X::M( defined)
# first, then by size. Prints path, size, whether it defines a member of Class, and its #define count.
import re, sys, subprocess
from pathlib import Path
R = Path('/home/user/FableDecomp/rebuild/src/compiled')
X = sys.argv[1]; sws = sys.argv[2].split(','); n = int(sys.argv[3]) if len(sys.argv) > 3 else 5
r = subprocess.run(['grep', '-rlE', r'^\s*#\s*define\s+' + sws[0] + r'\b', str(R), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
c = []
for f in r:
    t = open(f, encoding='utf-8', errors='replace').read()
    if not all(re.search(r'^\s*#\s*define\s+' + s + r'\b', t, re.M) for s in sws): continue
    if re.search(r'_PASS\d', ''.join(re.findall(r'#\s*define\s+(\w+)', t))): continue
    own = bool(re.search(r'(^|[^\w:])' + X + r'::~?\w+\s*\(', t))
    c.append((0 if own else 1, len(t), f))
c.sort()
for o, s, f in c[:n]:
    t = open(f, encoding='utf-8', errors='replace').read()
    print('%s own=%d bytes=%d defines=%d' % (f.replace(str(R) + '/', ''), 1 - o, s, len(re.findall(r'^\s*#\s*define\s+FABLE', t, re.M))))
print('total', len(c))
