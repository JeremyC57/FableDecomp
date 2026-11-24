# mvrow.py <idx> [--defs D1,D2,...] : NF322 class-model move of one ENTRIES row (2026-11-23).
# Starts from the HEAD source (copies it to orig/<a>.cpp first), keeps every HEAD line except:
#   - the alias struct `struct X_Alias : X { ... };` when it only declares the entry's member (else: report KEEP_ALIAS, no draft);
#   - the old `// Leaf:` line (replaced by the full PDB spelling) and the stale "Stand-in (owner rule 2026-11-05)" trailer;
# rewrites the definition head `R X_Alias::M(P a, ...) [const]` as the PDB signature (llvm-undname of pdb_spelling) on the real
# class X, keeping HEAD's parameter names by position; and puts `#define FABLE_RETAIL_<X>_VIRTUAL_MODEL` (+ --defs prerequisites) on
# the lines just above the first `#include`. Writes rows/<a>.cpp; prints one report line (OK / NO_ALIAS / KEEP_ALIAS / NO_DEFN /
# ARGCOUNT / OTHER_ALIAS_USE). Also reports TEST=<path> and whether the landed test names the alias (TEST_ALIAS).
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF327')
R = Path('/home/user/FableDecomp')
sys.path.insert(0, str(A / 'tools'))
KW = {'long', 'int', 'short', 'char', 'bool', 'float', 'double', 'void', '__int64', 'unsigned', 'signed', 'const', 'volatile', 'wchar_t'}
def split_top(s, ch=','):
    out, d, cur = [], 0, ''
    for c in s:
        if c in '<([': d += 1
        elif c in '>)]': d -= 1
        if c == ch and d == 0: out.append(cur); cur = ''
        else: cur += c
    if cur.strip(): out.append(cur)
    return [x.strip() for x in out]
def pname(p):
    p = re.sub(r'=.*$', '', p).strip()
    if p in ('', 'void'): return None, None
    m = re.match(r'^(.*?[\s\*&>])([A-Za-z_]\w*)\s*(\[[^\]]*\])?$', p)
    if m and m.group(2) not in KW:
        ty = m.group(1).strip(); lastw = [w for w in re.split(r'[\s\*&]+', ty) if w]
        if ty and lastw and lastw[-1] not in ('const', 'struct', 'class', 'enum', 'unsigned', 'signed'): return ty, m.group(2)
    return p, None
def ty(t):
    t = t.strip(); t = re.sub(r'\b(class|struct|enum|union)\s+', '', t); t = re.sub(r'>>', '> >', t)
    m = re.match(r'(.*?)\s+const\s*([&*])$', t)
    if m: return 'const %s%s' % (m.group(1), m.group(2))
    m = re.match(r'(.*?)\s*([&*])$', t)
    if m: return '%s%s' % (m.group(1), m.group(2))
    return t
def pdb_sig(pdb):
    dm = subprocess.run(['llvm-undname'], input=pdb + '\n', capture_output=True, text=True).stdout.strip().split('\n')[-1]
    m = re.match(r'(public|protected|private): (virtual )?(.*?)__thiscall ([\w:]+)::(\w+)\((.*)\)( const)?$', dm)
    acc, virt, ret, X, M, params, cst = m.groups()
    ps = [] if params.strip() == 'void' else [ty(p) for p in split_top(params)]
    return ty(ret), X, M, ps, bool(cst)
def catalog_block(a):
    PS = (R / 'rebuild/build_candidates.ps1').read_text(encoding='utf-8')
    for b in PS.split('[pscustomobject]@{'):
        if re.search(r"Address\s*=\s*'?(0x)?%s'?" % a, b, re.I): return b
    return ''
def main():
    args = sys.argv[1:]; idx = int(args[0]); opt = {}
    k = 1
    while k < len(args): opt[args[k][2:]] = args[k + 1]; k += 2
    E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
    e = E[idx - 1]; a = e[0]; pdb = e[3]
    ret, X, M, ptypes, const = pdb_sig(pdb)
    blk = catalog_block(a)
    sm = re.search(r"Source\s*=\s*'([^']+)'", blk); tm = re.search(r"TestSource\s*=\s*'([^']+)'", blk)
    srcp = R / 'rebuild/src/compiled' / sm.group(1)
    src = srcp.read_text(encoding='utf-8')
    (A / 'orig' / (a + '.cpp')).write_text(src, encoding='utf-8')
    tnote = ''
    if tm:
        tp = R / 'rebuild/tests' / tm.group(1); tt = tp.read_text(encoding='utf-8', errors='replace')
        (A / 'orig' / (a + '_test.cpp')).write_text(tt, encoding='utf-8')
        tnote = 'TEST=%s%s' % (tm.group(1), ' TEST_ALIAS' if re.search(r'\b' + re.escape(X) + r'_\w+', tt) or '_Methods' in tt else '')
    else: tnote = 'TEST=none'
    # alias class: the class the HEAD definition is scoped to
    dm = re.search(r'\b(\w+)::' + re.escape(M) + r'\s*\(', re.sub(r'//[^\n]*', '', src))
    if not dm: print(idx, a, 'NO_DEFN', tnote); return
    XA = dm.group(1)
    note = []
    text = src
    if XA != X:
        am = re.compile(r'^\s*struct\s+' + re.escape(XA) + r'\s*:\s*(public\s+)?' + re.escape(X) + r'\s*\{', re.M).search(text)
        if not am: print(idx, a, 'NO_ALIAS', XA, tnote); return
        i = am.end(); d = 1
        while d: d += {'{': 1, '}': -1}.get(text[i], 0); i += 1
        j = text.find(';', i - 1)
        inner = text[am.end():i - 1]
        members = [s.strip() for s in re.split(r';', re.sub(r'//[^\n]*', '', inner)) if s.strip()]
        if '{' in inner or len(members) != 1 or not re.search(r'\b' + re.escape(M) + r'\s*\(', members[0]):
            print(idx, a, 'KEEP_ALIAS', XA, len(members), tnote); return
        end = j + 1 + (1 if text[j + 1:j + 2] == '\n' else 0)
        text = text[:am.start()] + text[end:]
        if re.search(r'\b' + re.escape(XA) + r'\b', re.sub(r'//[^\n]*', '', text).replace(XA + '::' + M, '')):
            note.append('OTHER_ALIAS_USE')
    # definition head
    masked = re.sub(r'//[^\n]*', lambda mm: ' ' * len(mm.group()), text)   # NF325: never match a head inside a comment line
    hm = re.search(r'(^|\n)([^\n;{}]*?)\b' + re.escape(XA) + r'::' + re.escape(M) + r'\s*\(', masked)
    st = len(masked[:hm.start(2)]) + (len(masked[hm.start(2):hm.end(2)]) - len(masked[hm.start(2):hm.end(2)].lstrip())); j = masked.find('(', hm.end(2)); d = 0
    for i in range(j, len(masked)):
        if masked[i] == '(': d += 1
        elif masked[i] == ')':
            d -= 1
            if d == 0: break
    hp = [pname(p) for p in split_top(text[j + 1:i])]; hp = [h for h in hp if h[0]]
    tail = re.match(r'\s*(const)?', text[i + 1:]); after = i + 1 + tail.end()
    if len(hp) != len(ptypes): note.append('ARGCOUNT head=%d pdb=%d' % (len(hp), len(ptypes)))
    ps = []
    for n, t in enumerate(ptypes):
        nm = hp[n][1] if n < len(hp) else None
        ps.append(t + (' ' + nm if nm else ''))
    head = '%s %s::%s(%s)%s' % (ret, X, M, ', '.join(ps), ' const' if const else '')
    old_head = ' '.join(text[st:after].split())
    text = text[:st] + head + (' ' if text[after:after + 1] == '{' else '') + text[after:]
    out = []
    for l in text.split('\n'):
        if re.match(r'\s*//\s*Leaf:', l): continue
        if re.match(r'^// Stand-in \(owner rule 2026-11-05\): the real method on a stand-in class', l): continue
        out.append(l)
    defs = ['FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()] + [x for x in opt.get('defs', '').split(',') if x]
    have = set(re.findall(r'#\s*define\s+(FABLE_\w+)', text))
    k = 0
    while k < len(out) and (out[k].startswith('//') or not out[k].strip()): k += 1
    pro = ['// Leaf: ' + pdb,
           '// Class-model move (NF325, 2026-11-24): defined on the real class of engine/%s.h under %s (the opt-in polymorphic model;' % (X, defs[0]),
           '//   was %s); the retail RTTI vftable placement names this PDB member here.' % (XA + '::' + M)]
    fi = next((n for n, l in enumerate(out) if re.match(r'\s*#\s*include\s', l)), k)
    ins = ['#define ' + x for x in defs if x not in have]
    if fi < k: fi = k
    res = out[:k] + pro + out[k:fi] + ins + out[fi:]
    (A / 'rows' / (a + '.cpp')).write_text('\n'.join(res).rstrip('\n') + '\n', encoding='utf-8')
    print(idx, a, 'OK', head, '|', ' '.join(note), '| old:', old_head[:90], '|', tnote)
main()
