# ovinc.py (wave 27; wave 29: transitive): a header edited in an overlay (<ov>/rebuild/include/<path>) is compiled from there:
# rewrite(text, ov) points each `#include "<path>"` of such a header at the overlay copy (absolute Z: path). Wave 29: a repo
# header that (transitively) includes an edited header is copied, with its own includes rewritten the same way, to a shadow
# tree <ov>.shadow/ (never part of the overlay the coordinator copies) and the include points there. The landed text keeps the
# plain include; the coordinator copies the edited header with ov_final.
import os, re
REPO_INC = '/home/user/FableDecomp/rebuild/include'
_RX = re.compile(r'#include "([^"]+\.h)"')
_memo = {}
def _deps(rel):
    p = os.path.join(REPO_INC, rel)
    if not os.path.exists(p): return []
    return _RX.findall(open(p, encoding='utf-8', errors='surrogateescape').read())
def _touches(rel, base, seen=None):
    # True when rel is edited in the overlay or includes (transitively) an edited header
    key = (base, rel)
    if key in _memo: return _memo[key]
    if os.path.exists(os.path.join(base, rel)): _memo[key] = True; return True
    seen = seen or set()
    if rel in seen: return False
    seen.add(rel)
    r = any(_touches(d, base, seen) for d in _deps(rel))
    _memo[key] = r; return r
def _shadow(rel, ov, base):
    sh = os.path.abspath(ov).rstrip('/') + '.shadow'
    dst = os.path.join(sh, rel)
    src = os.path.join(REPO_INC, rel)
    t = open(src, encoding='utf-8', errors='surrogateescape').read()
    t2 = rewrite(t, ov)
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    if not os.path.exists(dst) or open(dst, encoding='utf-8', errors='surrogateescape').read() != t2:
        tmp = dst + '.%d.%d' % (os.getpid(), id(t2))
        open(tmp, 'w', encoding='utf-8', errors='surrogateescape').write(t2); os.replace(tmp, dst)   # atomic: threads share the tree
    return dst
_stack = set()
def rewrite(t, ov):
    if not ov or ov == '-': return t
    base = os.path.join(os.path.abspath(ov), 'rebuild/include')
    if not os.path.isdir(base): return t
    def fx(m):
        rel = m.group(1); p = os.path.join(base, rel)
        if os.path.exists(p):
            # wave 31: an edited header that includes another edited header gets a shadow copy with its own includes rewritten
            ht = open(p, encoding='utf-8', errors='surrogateescape').read(); ht2 = ht
            if rel not in _stack:
                _stack.add(rel)
                try: ht2 = rewrite(ht, ov)
                finally: _stack.discard(rel)
            if ht2 == ht: return '#include "Z:%s"' % p
            dst = os.path.join(os.path.abspath(ov).rstrip('/') + '.shadow', 'ovhdr', rel); os.makedirs(os.path.dirname(dst), exist_ok=True)
            if not os.path.exists(dst) or open(dst, encoding='utf-8', errors='surrogateescape').read() != ht2:
                tmp = dst + '.%d.%d' % (os.getpid(), id(ht2))
                open(tmp, 'w', encoding='utf-8', errors='surrogateescape').write(ht2); os.replace(tmp, dst)
            return '#include "Z:%s"' % dst
        if _touches(rel, base): return '#include "Z:%s"' % _shadow(rel, ov, base)
        return m.group(0)
    return _RX.sub(fx, t)
