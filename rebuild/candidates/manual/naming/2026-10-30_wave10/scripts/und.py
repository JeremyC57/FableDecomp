# demangle with llvm-undname (cached in und.json); qname(): the qualified function / object name of a demangled string
import subprocess, json, os, re
P = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'und.json')
_C = json.load(open(P)) if os.path.exists(P) else {}
def get(syms):
    need = sorted({s for s in syms if s not in _C})
    for i in range(0, len(need), 2000):
        ch = need[i:i + 2000]
        out = subprocess.run(['llvm-undname'], input='\n'.join(ch) + '\n', capture_output=True, text=True, errors='replace').stdout
        blocks = out.split('\n\n')
        m = {}
        for b in blocks:
            l = b.strip('\n').split('\n')
            if len(l) >= 2: m[l[0]] = l[1]
        for s in ch: _C[s] = m.get(s, s)
    if need: json.dump(_C, open(P, 'w'))
    return {s: _C.get(s, s) for s in syms}
def qname(d):
    # strip return type / access / calling convention: take the text before the parameter list
    s = re.sub(r'^(public|private|protected): ', '', d)
    s = re.sub(r'^(static|virtual) ', '', s)
    depth = 0; cut = len(s)
    for i, c in enumerate(s):
        if c == '<': depth += 1
        elif c == '>': depth -= 1
        elif c == '(' and depth == 0 and not s[:i].endswith('operator'): cut = i; break
    head = s[:cut]
    # last token containing '::' or the identifier
    toks = []; cur = ''; dp = 0
    for c in head.strip():
        if c == '<': dp += 1
        elif c == '>': dp -= 1
        if c == ' ' and dp == 0: toks.append(cur); cur = ''
        else: cur += c
    toks.append(cur)
    for t in reversed(toks):
        if t and t not in ('__thiscall', '__cdecl', '__fastcall', '__stdcall', 'const', '*', '&'): 
            if t.startswith('__'): continue
            return t
    return head
def leaf(q):
    out = ''; dp = 0
    for c in q:
        if c == '<': dp += 1
        elif c == '>': dp -= 1
        elif dp == 0: out += c
    return out.split('::')[-1]
