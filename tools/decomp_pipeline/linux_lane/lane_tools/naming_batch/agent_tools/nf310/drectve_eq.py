# drectve_eq.py <addr...>: for objsec NOT_EQ on .drectve only: compare HEAD v new .drectve after mapping the row's anonymous-namespace
# hash (?A0x........, which VC7.1 derives from the compile path; HEAD and new compile in different work dirs) and the method renames
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF310'
sys.path.insert(0, A + '/tools'); import nf
for a in sys.argv[1:]:
    h, n = A + '/work/obj/H_%s.obj' % a, A + '/work/obj/%s.obj' % a
    dh = [x['data'] for x in nf.coff(h)[0] if x['name'] == '.drectve'][0].decode('latin1')
    dn = [x['data'] for x in nf.coff(n)[0] if x['name'] == '.drectve'][0].decode('latin1')
    hh = set(re.findall(r'\?A0x[0-9a-f]{8}', dh)); hn = set(re.findall(r'\?A0x[0-9a-f]{8}', dn))
    if len(hh) != 1 or len(hn) != 1: print(a, 'HASHES', hh, hn); continue
    m = dh.replace(hh.pop(), hn.pop())
    sh, sn = nf.ext_syms(h), nf.ext_syms(n)
    for o in sorted(x[4:] for x in sh - sn):
        if '?A0x' in o: continue
        meth = re.match(r'\?+(\w+)@', o).group(1); c = [x[4:] for x in sn - sh if x[4:].startswith('?' + meth + '@')]
        if len(c) == 1: m = m.replace(o, c[0])
    print(a, 'DRECTVE_EQ (hash + renames mapped)' if m == dn else 'DRECTVE_DIFF')
