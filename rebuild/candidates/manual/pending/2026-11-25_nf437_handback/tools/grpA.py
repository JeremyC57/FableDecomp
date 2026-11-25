import sys, re
sys.path.insert(0, sys.argv[1] + '/tools'); import xf
A = sys.argv[1]
em = {}
for l in open(A + '/work/emitters.tsv'):
    t, s, rows = l.rstrip('\n').split('\t')
    if 'CVectorMap@W4ETC' in s: continue
    for r in rows.split(): em.setdefault(r, []).append(s)
for a in sys.argv[2:]:
    p = A + '/l1/rows/%s.cpp' % a; s = open(A + '/l1/orig/rows/%s.cpp' % a).read()
    names = []
    for M in ('CInterfaceMap', 'CInterfaceMapB'):
        s, at = xf.remove_struct(s, M)
        if at is not None:
            names.append(M); s, n = xf.remove_defs(s, M)
    s = xf.drop_empty_pragmas(s)
    old = ' / '.join(em.get(a, []))
    first = True
    for M in names:
        if first: s = s.replace('@@TD_%s@@\n' % M, xf.typedef_block(M, old, True)); first = False
        else: s = s.replace('@@TD_%s@@\n' % M, 'typedef %s %s;\n' % (xf.TD, M))
    for M in names:
        s = re.sub(r'\b%s::S?Entry\b' % M, '%s::value_type' % M, s)
        s = re.sub(r'(?<!const )\b%s::value_type\s*\*' % M, 'const %s::value_type*' % M, s)
        s = re.sub(r'(?<!const )\b%s\s*\*\s*map\b' % M, 'const %s* map' % M, s)
    s = re.sub(r'->(last|Last)\b', '->end()', s)
    s = re.sub(r'->(type|Type)\b', '->first', s)
    s = re.sub(r'->(component|Component)\b', '->second', s)
    for v in set(re.findall(r'LowerBound\(&(\w+)\)', s)):
        s = re.sub(r'\b(long|fable_i32)\s+%s\s*=\s*([^;]+);' % v, lambda m: 'ETCInterfaceType %s = (ETCInterfaceType)%s;' % (v, m.group(2) if re.match(r'^\w+$', m.group(2)) else '(' + m.group(2) + ')'), s)
    s = re.sub(r'LowerBound\(&(\w+)\)', r'LowerBound(\1)', s)
    open(p, 'w').write(s)
    print('==', a, names)
    for l in s.split('\n'):
        if re.search(r'\b(CInterfaceMapB?)\s+\w+\s*;|_pad\w*\[|pad4c|LowerBound\(|ETCInterfaceType \w+ =', l): print('   ', l.strip()[:140])
