#!/usr/bin/env python3
"""link377.py <tag> <watch regex> <objs...> -- <objs...>: /FORCE /MAP link (VC7.1 link.exe, /DLL /NOENTRY /NODEFAULTLIB) of a HEAD set and a
new set. Per set: duplicates (LNK2005/LNK4006), unresolved externals matching watch, and for every watched symbol the map's defining
object (resolution). Writes evidence/link_<tag>.txt; the diff HEAD -> new (resolved->unresolved, unresolved->resolved, object changes)."""
import sys, os, re, collections, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf387 as nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
from pathlib import Path
W = Path(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
tag, watch, objs = sys.argv[1], re.compile(sys.argv[2]), sys.argv[3:]
i = objs.index('--'); sets = (('HEAD', objs[:i]), ('new', objs[i + 1:]))
e = V.env(); e.pop('LINK', None)
out = open(W / 'evidence' / ('link_%s.txt' % tag), 'w'); res = {}
for nm, ob in sets:
    base = W / 'work' / ('lk_%s_%s' % (tag, nm))
    rsp = Path(str(base) + '.rsp'); rsp.write_text('\n'.join('"%s"' % V.wpath(o) for o in ob) + '\n')   # NF408: a response file (271 paths overflow the wine command line)
    r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/DLL', '/NOENTRY', '/FORCE', '/NODEFAULTLIB', '/OPT:NOREF', '/OPT:NOICF', '/MAP:' + V.wpath(str(base) + '.map'), '/out:%s.dll' % base, '@' + V.wpath(str(rsp))], e)
    if not ((r.stdout or '') + (r.stderr or '')).strip() and not Path(str(base) + '.map').exists(): print('LINK_FAILED_NO_OUTPUT', file=out)
    t = (r.stdout or '') + (r.stderr or '')
    open(str(base) + '.log', 'w').write(t)   # NF382: raw linker output kept for the symbol-level duplicate diff (tools/dupdiff.py)
    d = [l for l in t.splitlines() if 'LNK4006' in l or 'LNK2005' in l]
    print('## %s set (%d objects): %s' % (nm, len(ob), ' '.join(Path(x).parent.name + ('/' + Path(x).parts[-6] if True else '') for x in ob)), file=out)
    print('duplicates', len(d), file=out)
    for l in d: print('  DUP', l[-260:], file=out)
    u = collections.Counter()
    for l in t.splitlines():
        m = re.search(r'(LNK2001|LNK2019): unresolved external symbol (?:"[^"]*" \((\S+)\)|(\S+))', l)
        if m:
            s = m.group(2) or m.group(3)
            if watch.search(s): u[s] += 1
    defs = {}
    mp = Path(str(base) + '.map')
    if mp.exists():
        for l in mp.read_text(errors='replace').splitlines():
            f = l.split()
            if len(f) >= 4 and re.match(r'[0-9a-f]{4}:[0-9a-f]{8}$', f[0]) and watch.search(f[1]): defs[f[1]] = f[-1]
    for s, c in sorted(u.items()): print('UNRESOLVED', c, s, file=out)
    for s, o in sorted(defs.items()): print('DEFINED', s, 'in', o, file=out)
    res[nm] = (set(u), defs)
    for x in ('.dll', '.lib', '.exp', '.map'):
        p = Path(str(base) + x)
        if p.exists(): p.unlink()
hu, hd = res['HEAD']; nu, nd = res['new']
print('## diff HEAD -> new', file=out)
for s in sorted(set(hd) - set(nd)): print('RESOLVED_TO_GONE', s, file=out)
for s in sorted(nu - hu): print('NEW_UNRESOLVED', s, ('(HEAD resolved in %s)' % hd[s]) if s in hd else '(not at HEAD)', file=out)
for s in sorted(hu - nu): print('NOW_RESOLVED' if s in nd else 'GONE_UNRESOLVED', s, nd.get(s, ''), file=out)
print('LK_DONE', file=out); out.close(); print(open(W / 'evidence' / ('link_%s.txt' % tag)).read())
