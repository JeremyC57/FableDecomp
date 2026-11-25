#!/usr/bin/env python3
"""link426.py <tag> <watch regex> <args file: 'HEAD objs -- new objs'>: /FORCE DLL link of each set through a response file (Wine paths),
printing return code, duplicate lines (LNK2005/LNK4006) and unresolved symbols matching <watch>, to evidence/link_<tag>.txt."""
import sys, os, re, subprocess, collections
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426'
tag, watch, af = sys.argv[1:4]; w = re.compile(watch)
sets = open(af).read().split(' -- ')
wp = lambda p: 'Z:' + os.path.abspath(p).replace('/', '\\')
e = V.env(); e.pop('LINK', None)
out = open(A + '/evidence/link_%s.txt' % tag, 'w')
for nm, objs in (('HEAD', sets[0].split()), ('new', sets[1].split())):
    rsp = A + '/work/link_%s_%s.rsp' % (tag, nm); dll = A + '/work/lk_%s_%s.dll' % (tag, nm)
    open(rsp, 'w').write('\r\n'.join(['/nologo', '/DLL', '/NOENTRY', '/FORCE', '/NODEFAULTLIB', '/out:"%s"' % wp(dll)] + ['"%s"' % wp(o) for o in objs]) + '\r\n')
    cmd = ([V.WINE] if V.WINE else []) + [str(V.VC / 'bin' / 'link.exe'), '@' + wp(rsp)]
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=e, timeout=600)
    t = (r.stdout or '') + (r.stderr or '')
    d = [l for l in t.splitlines() if 'LNK2005' in l or 'LNK4006' in l]
    fatal = [l for l in t.splitlines() if 'fatal' in l.lower()]
    u = collections.Counter()
    for l in t.splitlines():
        m = re.search(r'(LNK2001|LNK2019): unresolved external symbol (?:"[^"]*" \((\S+)\)|(\S+))', l)
        if m:
            s = m.group(2) or m.group(3)
            if w.search(s): u[s] += 1
    print('## %s set: %d objects, rc=%d, dll=%s, fatal=%s' % (nm, len(objs), r.returncode, os.path.exists(dll), fatal), file=out)
    print('duplicates %d' % len(d), file=out)
    dc = collections.Counter(re.sub(r'^.*?\((\?\S+)\) already defined.*$', r'\1', l) for l in d)
    for s, c in sorted(dc.items()): print('  DUP x%d %s' % (c, s[:160]), file=out)
    print('unresolved matching watch: %d' % len(u), file=out)
    for s, c in sorted(u.items()): print('  UNRESOLVED x%d %s' % (c, s), file=out)
    print('all unresolved lines: %d' % sum(1 for l in t.splitlines() if 'unresolved external' in l), file=out)
out.close(); print(open(A + '/evidence/link_%s.txt' % tag).read())
