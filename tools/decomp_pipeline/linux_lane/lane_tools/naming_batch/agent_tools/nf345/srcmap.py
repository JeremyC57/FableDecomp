# srcmap.py <addrfile> [src|test]: print `addr<TAB>path` of each landed row's catalog Source (or TestSource), bounded to its own block (nf.field).
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
k = sys.argv[2] if len(sys.argv) > 2 else 'src'
for l in open(sys.argv[1]):
    if not l.strip(): continue
    a = l.split()[0].lower(); p = nf.src_of(a) if k == 'src' else nf.test_of(a)
    print('%s\t%s' % (a, p if p else 'NONE'))
