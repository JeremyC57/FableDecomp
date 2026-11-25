import sys, re
sys.path.insert(0, sys.argv[1] + '/tools'); import xf
A = sys.argv[1]; M = sys.argv[2]
em = {}
for l in open(A + '/work/emitters.tsv'):
    t, s, rows = l.rstrip('\n').split('\t')
    if 'CVectorMap@W4ETC' in s: continue
    for r in rows.split(): em.setdefault(r, []).append((s, t))
for a in sys.argv[3:]:
    p = A + '/l1/rows/%s.cpp' % a; s = open(A + '/l1/orig/rows/%s.cpp' % a).read()
    # the old Find's return type
    st = re.search(r'^[ \t]*struct\s+%s\s*\{.*?^\};' % M, s, re.S | re.M).group(0)
    rets = dict(re.findall(r'FABLE_FORCEINLINE\s+([\w:]+\s*\*)\s*Find\s*\(long kind\)\s*(const)?', st)) if False else re.findall(r'FABLE_FORCEINLINE\s+([\w:]+\s*\*)\s*Find\s*\(long kind\)', st)
    ret = rets[0].replace(' ', '')
    s, at = xf.remove_struct(s, M); s, n = xf.remove_defs(s, M); s = xf.drop_empty_pragmas(s)
    const = all(t == '004365b0' for _, t in em[a])
    s = s.replace('@@TD_%s@@\n' % M, xf.typedef_block(M, ' / '.join(x for x, _ in em[a]), const, True) +
        '// (the row-local inline Find(kind) of the stand-in is CVectorMap::Find (engine/CVectorMap.h) on the key temporary, then ->second)\n')
    s, k = re.subn(r'\b(\w+(?:->|\.))m_tcs\.Find\(kind\)|\bm_tcs\.Find\(kind\)', lambda m: '(%s)%sm_tcs.Find((ETCInterfaceType)kind)->second' % (ret, m.group(1) or ''), s)
    open(p, 'w').write(s)
    print('==', a, 'ret', ret, 'const', const, 'defs removed', n, 'Find sites', k)
    for l in s.split('\n'):
        if re.search(r'\b%s\s+\w+\s*;|m_tcs\b' % M, l): print('   ', l.strip()[:150])
