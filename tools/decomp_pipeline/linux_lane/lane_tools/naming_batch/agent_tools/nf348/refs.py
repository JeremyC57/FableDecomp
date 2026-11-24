# refs.py <obj> <symbol-substring>...: every relocation in the object (non-debug sections) that names a symbol containing the substring.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
secs, sl = nf.coff(sys.argv[1])
for k, s in enumerate(secs, 1):
    if re.match(r'^\.debug', s['name']): continue
    for off, sym, t in s['rel']:
        if any(x in sym for x in sys.argv[2:]): print('sec %d %s +%x -> %s' % (k, s['name'], off, sym[:120]))
print('done')
