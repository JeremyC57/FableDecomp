# mkdecl.py <idx..>: from the demangled PDB spelling, the access label, the in-class declaration and the out-of-line definition head.
# Types: `class X const &` -> `const X&`, `class X *` -> `X*`, `enum E` -> `E`, `void` params dropped. Prints idx, access, decl, head.
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
def ty(t):
    t = t.strip(); t = re.sub(r'\b(class|struct|enum|union)\s+', '', t)
    m = re.match(r'(.*?)\s+const\s*([&*])$', t)
    if m: return 'const %s%s' % (m.group(1), m.group(2))
    m = re.match(r'(.*?)\s*([&*])$', t)
    if m: return '%s%s' % (m.group(1), m.group(2))
    return t
def split_params(s):
    out = []; d = 0; cur = ''
    for ch in s:
        if ch in '<(': d += 1
        if ch in '>)': d -= 1
        if ch == ',' and d == 0: out.append(cur); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur)
    return out
for i in [int(x) for x in sys.argv[1:]]:
    pdb = E[i - 1][3]
    d = subprocess.run(['llvm-undname'], input=pdb + '\n', capture_output=True, text=True).stdout.strip().split('\n')[-1]
    m = re.match(r'(public|protected|private): (virtual )?(static )?(.*?)__thiscall (\w+)::(\w+)\((.*)\)( const)?$', d)
    if not m: print(i, 'UNPARSED', d); continue
    acc, virt, stat, ret, X, M, params, cst = m.groups()
    ps = [] if params.strip() == 'void' else [ty(p) for p in split_params(params)]
    rt = ty(ret)
    decl = '%s%s %s(%s)%s;' % ('virtual ' if virt else '', rt, M, ', '.join(ps), ' const' if cst else '')
    head = '%s %s::%s(%s)%s ' % (rt, X, M, ', '.join(ps), ' const' if cst else '')
    print('%d\t%s\t%s\t%s' % (i, acc, decl, head))
