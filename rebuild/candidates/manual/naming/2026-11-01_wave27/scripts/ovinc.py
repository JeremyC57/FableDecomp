# ovinc.py (wave 27): a header edited in an overlay (<ov>/rebuild/include/<path>) is compiled from there: rewrite(text, ov)
# points each `#include "<path>"` of such a header at the overlay copy (absolute Z: path), as mkpayload_fx does for funclet
# parents. The landed text keeps the plain include; the coordinator copies the header with ov_final.
import os, re
def rewrite(t, ov):
    if not ov or ov == '-': return t
    base = os.path.join(os.path.abspath(ov), 'rebuild/include')
    if not os.path.isdir(base): return t
    def fx(m):
        p = os.path.join(base, m.group(1))
        return '#include "Z:%s"' % p if os.path.exists(p) else m.group(0)
    return re.sub(r'#include "([^"]+\.h)"', fx, t)
