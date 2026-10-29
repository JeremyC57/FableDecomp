# wave 6 family (generic reference accessors): retail accessors that return a reference, modelled by rows as returning a
# pointer (same bytes, wrong mangling). Per SPEC entry: the one row-local declaration `<Ret>* <old>()[ const];` inside
# class <Cls>, whose comment names the retail address, becomes `<newdecl>`; calls `<old>()->m` become `<new>().m` and any
# other use `<chain><old>()` becomes `&<chain><new>()` (as fam_gmp6.py). Idempotent; skips rows where <old> is declared
# by another class too, or the declaration is not inside <Cls>.
#   0x004C7A30 CThing::GetPlayer  Ego_r.pdb ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ (calls CPlayerManager::GetPlayer 0x004498C0;
#              the const twin 0x004C7A10 is ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ; 2026-10-26 journal, QUEUE_561)
import re, sys, os
SPEC = [
    dict(addr='0x004C7A30', cls='CThing', ret='CPlayer', olds=('GetPlayer', 'GetPlayer_004c7a30'), new='GetPlayer',
         newdecl='CPlayer& GetPlayer();'),
]
def enclosing_class(t, pos):
    d = 0; i = pos
    while i > 0:
        i -= 1; c = t[i]
        if c == '}': d += 1
        elif c == '{':
            if d == 0:
                head = t[max(0, i - 300):i]
                m = re.search(r'\b(?:class|struct)[ \t]+(\w+)[^;{}()]*$', head)
                return m.group(1) if m else None
            d -= 1
    return None
def chain_start(t, j):
    while True:
        k = j
        while k > 0 and t[k - 1] in ' \t': k -= 1
        if t[k - 2:k] == '->': k -= 2
        elif t[k - 1:k] == '.': k -= 1
        elif t[k - 2:k] == '::': k -= 2
        else: return j
        while k > 0 and t[k - 1] in ' \t': k -= 1
        while k > 0 and t[k - 1] in ')]':
            close = t[k - 1]; op = '(' if close == ')' else '['; d = 0; m = k - 1
            while m >= 0:
                if t[m] == close: d += 1
                elif t[m] == op:
                    d -= 1
                    if d == 0: break
                m -= 1
            if m < 0: return None
            k = m
            while k > 0 and t[k - 1] in ' \t': k -= 1
        if k > 0 and t[k - 1] == '>' and t[k - 2:k] != '->':
            # template arguments of a functional-cast / call: Type<Args>(...)
            d = 0; m = k - 1
            while m >= 0:
                if t[m] == '>': d += 1
                elif t[m] == '<':
                    d -= 1
                    if d == 0: break
                elif t[m] in ';{}': return None
                m -= 1
            if m < 0: return None
            k = m
        while k > 0 and (t[k - 1].isalnum() or t[k - 1] == '_'): k -= 1
        j = k
def apply(t, s):
    olds = '|'.join(s['olds'])
    rx = re.compile(r'^([^\n]*?)\b(?:class[ \t]+)?' + s['ret'] + r'[ \t]*\*[ \t]*(' + olds + r')[ \t]*\([ \t]*\)[ \t]*(?:const[ \t]*)?;([^\n]*)$', re.M)
    ds = [m for m in rx.finditer(t) if s['addr'].lower() in m.group(3).lower()]
    if len(ds) != 1: return None, 'decl %d' % len(ds)
    d = ds[0]; old = d.group(2)
    if enclosing_class(t, d.start(2)) != s['cls']: return None, 'decl not in ' + s['cls']
    code = re.sub(r'//[^\n]*', '', t)
    decls = re.findall(r'^[^\n]*\b\w[\w:<>]*[ \t*&]+(?:\w+::)?' + old + r'[ \t]*\([^)\n]*\)[ \t]*(?:const[ \t]*)?[;{]', code, re.M)
    decls = [x for x in decls if not re.search(r'(->|\.)[ \t]*' + old + r'[ \t]*\(', x)]
    if len(decls) != 1: return None, '%s declared %d times' % (old, len(decls))
    t = t[:d.start()] + d.group(1) + s['newdecl'] + d.group(3) + t[d.end():]
    out = []; pos = 0; n = 0
    for m in re.finditer(r'(?<![\w&])' + old + r'[ \t]*\(\)', t):
        ls = t.rfind('\n', 0, m.start()) + 1
        if '//' in t[ls:m.start()]: continue
        le = t.find('\n', m.end()); le = len(t) if le < 0 else le
        if s['newdecl'] in t[ls:le] and t[ls:le].find(s['newdecl'].split('(')[0].split()[-1]) >= 0 and m.start() >= ls + t[ls:le].find(s['newdecl']):
            continue
        rest = t[m.end():]; r2 = rest.lstrip(' \t')
        if r2.startswith('->'):
            out.append(t[pos:m.start()]); out.append(s['new'] + '().'); pos = m.end() + (len(rest) - len(r2)) + 2; n += 1
        else:
            st = chain_start(t, m.start())
            if st is None or st < pos: return None, 'chain'
            out.append(t[pos:st]); out.append('&'); out.append(t[st:m.start()]); out.append(s['new'] + '()'); pos = m.end(); n += 1
    out.append(t[pos:])
    return ''.join(out), 'ok %s %d' % (s['addr'], n)
def fix(t):
    whys = []; changed = False
    for s in SPEC:
        if s['addr'].lower() not in t.lower(): continue
        new, why = apply(t, s); whys.append(why)
        if new is not None: t = new; changed = True
    return (t if changed else None), '; '.join(whys) or 'no spec address'
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
