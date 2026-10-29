# wave 6 family: 0x00449970 is the non-const CPlayerManager::GetMainPlayer, returning a reference (Ego_r.pdb
# ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ; the const twin 0x00449960 is ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ;
# QUEUE_561 / chunk 561; 159 landed rows already emit the reference form). Row-local models returned `CPlayer*` (same bytes).
# Transform (idempotent, source files whose only GetMainPlayer declaration is the pointer form commented 0x00449970):
#   `CPlayer* GetMainPlayer();`  ->  `CPlayer& GetMainPlayer();`
#   `...GetMainPlayer()->m`       ->  `...GetMainPlayer().m`
#   any other use `<chain>GetMainPlayer()` (a pointer value)  ->  `&<chain>GetMainPlayer()`
# where <chain> is the postfix expression before it (identifiers, `->`, `.`, `::`, balanced calls / subscripts).
import re, sys
DECL = re.compile(r'^([^\n]*?)\b(?:class[ \t]+)?CPlayer[ \t]*\*[ \t]*GetMainPlayer[ \t]*\([ \t]*\)[ \t]*;([^\n]*)$', re.M)
def chain_start(t, i):
    j = i
    while True:
        k = j
        while k > 0 and t[k - 1] in ' \t': k -= 1
        if t[k - 2:k] == '->': k -= 2
        elif t[k - 1:k] == '.': k -= 1
        elif t[k - 2:k] == '::': k -= 2
        else: return j
        while k > 0 and t[k - 1] in ' \t': k -= 1
        # operand: identifier, optionally followed by balanced (...) / [...] groups
        while k > 0 and t[k - 1] in ')]':
            close = t[k - 1]; open_ = '(' if close == ')' else '['; d = 0; m = k - 1
            while m >= 0:
                if t[m] == close: d += 1
                elif t[m] == open_:
                    d -= 1
                    if d == 0: break
                m -= 1
            if m < 0: return None
            k = m
            while k > 0 and t[k - 1] in ' \t': k -= 1
        e = k
        while k > 0 and (t[k - 1].isalnum() or t[k - 1] == '_'): k -= 1
        if k == e:
            # a parenthesised operand `(expr)->` : already consumed as a group; accept
            pass
        j = k
def fix(t):
    if '0x00449970' not in t.replace('0x00449970', '0x00449970'): return None, 'no 0x00449970'
    ds = list(DECL.finditer(t))
    alld = re.findall(r'\bGetMainPlayer[ \t]*\([^)]*\)[ \t]*(?:const[ \t]*)?[;{]', re.sub(r'//[^\n]*', '', t))
    calls_semicolon = re.findall(r'(?:->|\.)[ \t]*GetMainPlayer[ \t]*\(\)[ \t]*;', t)
    if len(ds) != 1 or '0x00449970' not in ds[0].group(2): return None, 'decl %d' % len(ds)
    if len(alld) - len(calls_semicolon) != 1: return None, 'other GetMainPlayer declarations'
    d = ds[0]
    t = t[:d.start()] + d.group(0).replace('CPlayer* GetMainPlayer()', 'CPlayer& GetMainPlayer()').replace('CPlayer *GetMainPlayer()', 'CPlayer& GetMainPlayer()') + t[d.end():]
    if 'CPlayer& GetMainPlayer()' not in t: return None, 'decl spelling'
    out = []; pos = 0; n = 0
    for m in re.finditer(r'\bGetMainPlayer[ \t]*\(\)', t):
        ls = t.rfind('\n', 0, m.start()) + 1
        line = t[ls:t.find('\n', m.end()) if t.find('\n', m.end()) >= 0 else len(t)]
        if '//' in t[ls:m.start()]: continue
        if 'CPlayer& GetMainPlayer()' in line and m.start() - ls == line.find('GetMainPlayer'): continue
        rest = t[m.end():]
        r2 = rest.lstrip(' \t')
        if r2.startswith('->'):
            out.append(t[pos:m.end()]); pos = m.end() + (len(rest) - len(r2)) + 2; out.append('.'); n += 1
        else:
            s = chain_start(t, m.start())
            if s is None or s < pos: return None, 'chain'
            out.append(t[pos:s]); out.append('&'); pos = s; n += 1
    out.append(t[pos:])
    return ''.join(out), 'ok %d' % n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
