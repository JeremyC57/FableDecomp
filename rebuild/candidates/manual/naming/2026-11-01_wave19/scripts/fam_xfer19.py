# fam_xfer19.py <ov> <rows.txt> [tag]: CPersistContext::Transfer<T> -- the row declares the retail Transfer instantiations as plain overloads
# (void Transfer(long&) 0x00431102 / (unsigned long&) 0x00431020 / (float&) 0x00431061 / (bool&) 0x0043314A); the PDB spells them
# ??$Transfer@J@CPersistContext@@QAEXAAJ@Z etc. (member template instantiations): the overload declarations become one
# `template <class T> void Transfer(T& value);` declaration (the calls are unchanged; the compiler instantiates ??$Transfer@<T>@).
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'xfer'
OLD = {'?Transfer@CPersistContext@@QAEXAAJ@Z', '?Transfer@CPersistContext@@QAEXAAK@Z', '?Transfer@CPersistContext@@QAEXAAM@Z', '?Transfer@CPersistContext@@QAEXAA_N@Z'}
NEW = {'??$Transfer@J@CPersistContext@@QAEXAAJ@Z', '??$Transfer@K@CPersistContext@@QAEXAAK@Z', '??$Transfer@M@CPersistContext@@QAEXAAM@Z', '??$Transfer@_N@CPersistContext@@QAEXAA_N@Z'}
DECL = re.compile(r'(?m)^([ \t]*)void[ \t]+Transfer[ \t]*\([ \t]*(unsigned long|long|float|bool)[ \t]*&[ \t]*\w*[ \t]*\)[ \t]*;[^\n]*\n')
def tr(t, a):
    c = F.code_only(t)
    d = F.class_defs(c, 'CPersistContext')
    if len(d) != 1: return None, '%d definitions of CPersistContext' % len(d)
    kw, s, b, e = d[0]
    body = t[b + 1:e]
    ms = list(DECL.finditer(body))
    if not ms: return None, 'no scalar Transfer overload declaration'
    if re.search(r'template\s*<\s*class\s+\w+\s*>\s*void\s+Transfer\s*\(', body):
        # the row already declares the member template (for its enum / container instantiations): the scalar overloads are
        # just removed, the calls instantiate the template
        return t[:b + 1] + DECL.sub('', body) + t[e:], 'template Transfer<T> already declared: %d scalar overload declaration(s) removed' % len(ms)
    first = ms[0]
    rep = first.group(1) + 'template <class T> void Transfer(T& value);              // ??$Transfer@J@CPersistContext@@QAEXAAJ@Z 0x00431102, @K@ 0x00431020, @M@ 0x00431061, @_N@ 0x0043314A (Ego_r.pdb member template instantiations)\n'
    nb = body[:first.start()] + rep + DECL.sub('', body[first.end():])
    return t[:b + 1] + nb + t[e:], 'Transfer<T> replaces %d overload declaration(s)' % len(ms)
def ck(D0, U0, D1, U1, a):
    if U1 & OLD: return 'old overload still named'
    rem = [s for s in U0 - U1 if s not in OLD and '?A0x' not in s]
    add = [s for s in U1 - U0 if s not in NEW and '?A0x' not in s]
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    if not (U1 & NEW): return 'no template instantiation named'
    return None
F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
