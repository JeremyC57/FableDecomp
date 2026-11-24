# mkdraft.py <idx> [--defs D1,D2,...] [--prec <landed src rel path>] [--decl <header line no>] [--keep-alias]
# NF322 alias-sig draft: start from orig/<a>.cpp (HEAD), keep every HEAD line except
#   - the alias struct `struct X_Methods : X { ...M... };` when it only declares M (else kept, report KEEP_ALIAS),
#   - an old `// Leaf:` line (replaced by the full PDB Leaf),
#   - the HEAD `#define FABLE_RETAIL_*` / `#include` lines (re-emitted after the precedent's, no duplicates),
# and rewrite the definition head `R X_Methods::M(P a, ...) [const]` to the header declaration's signature
# (return type, parameter types, const) with HEAD's parameter names (positional). Writes rows/<a>.cpp; prints a one-line report.
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF325')
R = Path('/home/user/FableDecomp')
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

def ptype_name(p):
    p = re.sub(r'=.*$', '', p).strip()
    if p in ('', 'void'): return None
    m = re.match(r'^(.*?[\s\*&>])([A-Za-z_]\w*)$', p)
    if m:
        ty, nm = m.group(1).strip(), m.group(2)
        last = re.split(r'[\s\*&]+', ty.strip())
        lastw = [w for w in re.split(r'\s+', ty) if w][-1] if ty else ''
        if nm not in KW and not (lastw in ('const', 'struct', 'class', 'enum', 'unsigned', 'signed') ) and ty not in ('const', 'struct', 'class', 'enum'):
            return ty, nm
    return p, None

def parse_decl(line, M):
    c = line.split('//')[0]
    k = c.find(M + '(') if M + '(' in c else re.search(re.escape(M) + r'\s*\(', c).start()
    ret = c[:k].strip(); ret = re.sub(r'^(virtual|static|inline|FABLE_FORCEINLINE)\s+', '', ret).strip()
    j = c.find('(', k); d = 0
    for i in range(j, len(c)):
        if c[i] == '(': d += 1
        elif c[i] == ')':
            d -= 1
            if d == 0: break
    params = split_top(c[j + 1:i]); tail = c[i + 1:]
    const = bool(re.match(r'\s*const\b', tail))
    return ret, [ptype_name(p) for p in params if ptype_name(p)], const

def main():
    args = sys.argv[1:]; idx = int(args[0]); opt = {}
    k = 1
    while k < len(args):
        if args[k] == '--keep-alias': opt['keep'] = True; k += 1
        else: opt[args[k][2:]] = args[k + 1]; k += 2
    E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
    e = E[idx - 1]; a = e[0]; pdb = e[3]; cur = e[4]
    m = re.match(r"\?([^@]+)@([^@]+)@@", pdb); M, X = m.group(1), m.group(2)
    m2 = re.match(r"\?([^@]+)@([^@]+)@@", cur); XA = m2.group(2)
    hdr = R / 'rebuild/include/engine' / (X + '.h'); HL = hdr.read_text(encoding='utf-8', errors='replace').split('\n')
    if 'decl' in opt: dl = HL[int(opt['decl']) - 1]
    else:
        T = [l.rstrip('\n').split('\t') for l in open(A / 'work/triage.tsv')]
        T = [t for t in T if t[0] == str(idx) and len(t) > 5 and t[5] == 'DECL']
        if len(T) != 1: print(idx, a, 'DECL_AMBIGUOUS', [t[4] for t in T]); return
        dl = HL[int(T[0][4][1:]) - 1]
        while dl.count('(') > dl.count(')'):
            dl += ' ' + HL[int(T[0][4][1:])].strip()  # one continuation line
    ret, ptypes, const = parse_decl(dl, M)
    defs = opt.get('defs', '').split(',') if opt.get('defs') else []
    src = (A / 'orig' / (a + '.cpp')).read_text(encoding='utf-8')
    L = src.split('\n')
    # precedent preamble
    pdef, pinc = [], []
    if 'prec' in opt:
        pt = (R / 'rebuild/src/compiled' / opt['prec']).read_text(encoding='utf-8', errors='replace').split('\n')
        pdef = [l.split('//')[0].rstrip() for l in pt if re.match(r'\s*#\s*define\s+FABLE_\w+\s*($|//)', l)]
        pinc = [l.split('//')[0].rstrip() for l in pt if re.match(r'\s*#\s*(include|pragma)\s', l)]
    pdef = pdef + ['#define ' + d for d in defs if not any(re.match(r'\s*#\s*define\s+' + d + r'\b', x) for x in pdef)]
    # remove the alias struct when it only declares M
    text = src
    alias_re = re.compile(r'^\s*struct\s+' + re.escape(XA) + r'\s*:\s*(public\s+)?' + re.escape(X) + r'\s*\{', re.M)
    am = alias_re.search(text); note = []
    defn_inclass = None
    if am:
        i = am.end(); d = 1
        while d: d += {'{': 1, '}': -1}.get(text[i], 0); i += 1
        j = text.find(';', i - 1)
        block = text[am.start():j + 1]
        inner = text[am.end():i - 1]
        members = [s.strip() for s in re.split(r';', re.sub(r'//[^\n]*', '', inner)) if s.strip()]
        if '{' in inner:
            # in-class definition: lift it out
            note.append('INCLASS_DEF')
        if len(members) == 1 and re.search(r'\b' + re.escape(M) + r'\s*\(', members[0]) and '{' not in inner and not opt.get('keep'):
            text = text[:am.start()] + text[j + 1:].lstrip('\n') if text[j + 1:j + 2] == '\n' else text[:am.start()] + text[j + 1:]
        else:
            note.append('KEEP_ALIAS(%d members)' % len(members))
    # definition head
    dm = re.search(r'(^|\n)([^\n;{}]*?)\b' + re.escape(XA) + r'::' + re.escape(M) + r'\s*\(', text)
    if not dm: print(idx, a, 'NO_DEFN', ' '.join(note)); return
    st = dm.start(2); j = text.find('(', dm.end(2)); d = 0
    for i in range(j, len(text)):
        if text[i] == '(': d += 1
        elif text[i] == ')':
            d -= 1
            if d == 0: break
    hparams = [ptype_name(p) for p in split_top(text[j + 1:i]) if ptype_name(p)]
    tail = re.match(r'\s*(const)?', text[i + 1:]); after = i + 1 + tail.end()
    if len(hparams) != len(ptypes): note.append('ARGCOUNT head=%d pdb=%d' % (len(hparams), len(ptypes)))
    names = [h[1] for h in hparams] + [None] * 10
    ps = []
    for n, (ty, _) in enumerate(ptypes):
        nm = names[n] if n < len(hparams) else None
        if nm is None: ps.append(ty)
        else: ps.append(ty + ' ' + nm)
    head = '%s %s::%s(%s)%s' % (ret, X, M, ', '.join(ps), ' const' if const else '')
    old_head = text[st:after]
    text = text[:st] + head + (' ' if text[after:after + 1] == '{' else '') + text[after:]
    # drop old Leaf / defines / includes, re-emit preamble
    out = []; leaf_done = False; hdefs = []; hincs = []
    for l in text.split('\n'):
        if re.match(r'\s*//\s*Leaf:', l): continue
        if re.match(r'^// Stand-in \(owner rule 2026-11-05\): the real method on a stand-in class', l): continue
        if re.match(r'\s*#\s*define\s+FABLE_\w+', l): hdefs.append(l.split('//')[0].rstrip()); continue
        if re.match(r'\s*#\s*include\s', l): hincs.append(l.split('//')[0].rstrip()); continue
        out.append(l)
    key = lambda l: re.sub(r'\s+', ' ', l.strip())
    D = pdef + [l for l in hdefs if key(l) not in {key(x) for x in pdef}]
    I = pinc + [l for l in hincs if key(l) not in {key(x) for x in pinc}]
    eng = '#include "engine/%s.h"' % X
    if key(eng) not in {key(x) for x in I}: I.append(eng)
    # header comments = leading comment block
    k = 0
    while k < len(out) and (out[k].startswith('//') or not out[k].strip()): k += 1
    pro = ['// Leaf: ' + pdb,
           '// Signature port (NF322, 2026-11-23): defined on the real class of engine/%s.h%s with the PDB parameter / return types (was %s);' % (X, (' under ' + ', '.join(re.sub(r'#\s*define\s+', '', x).strip() for x in D)) if D else '', cur),
           '//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.']
    res = out[:k] + pro + D + I + out[k:]
    (A / 'rows' / (a + '.cpp')).write_text('\n'.join(res).rstrip('\n') + '\n', encoding='utf-8')
    print(idx, a, 'OK', head, '|', ' '.join(note), '| old:', ' '.join(old_head.split())[:80])

main()
