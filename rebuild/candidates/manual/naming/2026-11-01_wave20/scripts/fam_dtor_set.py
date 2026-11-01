# set_stmt / del_stmt / line_span (copied from fam_dtor.py)
import re
import famlib2 as F, acclib
def line_span(t, s, e):
    ls = t.rfind('\n', 0, s) + 1; le = t.find('\n', e); le = len(t) if le < 0 else le
    return ls, le
def set_stmt(t, c, kw, b, s, e, new, want):
    """replace statement t[s:e] with `new` under access `want` (wrapping labels when the current access differs)."""
    cur = acclib.access_at(c, kw, b + 1, s)
    ls, le = line_span(t, s, e)
    pre = t[ls:s]; post = t[e:le]
    alone = not pre.strip() and not F.code_only(post).strip()
    if cur == want: return t[:s] + new + t[e:]
    if alone:
        ind = re.match(r'[ \t]*', pre).group(0)
        return t[:ls] + want + ':\n' + ind + new + post + '\n' + cur + ':' + t[le:]
    return t[:s] + want + ': ' + new + ' ' + cur + ':' + t[e:]
def del_stmt(t, s, e):
    ls, le = line_span(t, s, e)
    if not t[ls:s].strip() and not F.code_only(t[e:le]).strip(): return t[:ls] + t[le + 1:]
    return t[:s] + t[e:]
