"""fi_convert.py [--apply] <roots...>: replace bare `__forceinline` code tokens with FABLE_FORCEINLINE (rebuild/include/fable_compiler.h).

Bare = a code token (not in a comment or string) that is not inside an `#if(def) _MSC_VER` true branch, not the macro NAME of a
`#define __forceinline ...` / `#undef __forceinline` shim. Occurrences in a #define replacement list are converted too.
A file that gets a replacement gets `#include "fable_compiler.h"` after its include guard (headers) or before its first code line.
Row-local `#define FABLE_FORCEINLINE __forceinline` lines are dropped (the shared header defines it).
"""
import re, sys, glob, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fi_scan import strip

INC = '#include "fable_compiler.h"'
TOK = re.compile(r'\b__forceinline\b')

def convert(path):
    raw = open(path, encoding='latin-1', newline='').read()
    st = strip(raw)
    lines_raw = raw.split('\n'); lines_st = st.split('\n')
    stack = []; changed = 0; out = []
    for lr, ls in zip(lines_raw, lines_st):
        s = ls.strip()
        m = re.match(r'#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)', s)
        if m:
            k, arg = m.groups()
            if k in ('if', 'ifdef', 'ifndef'): stack.append('_MSC_VER' in arg and k != 'ifndef' and '!' not in arg)
            elif k in ('else', 'elif') and stack: stack[-1] = False
            elif k == 'endif' and stack: stack.pop()
            out.append(lr); continue
        if not TOK.search(ls) or any(stack):
            out.append(lr); continue
        if re.match(r'#\s*(define|undef)\s+__forceinline\b', s):          # shim: leave
            out.append(lr); continue
        if re.match(r'#\s*define\s+FABLE_FORCEINLINE\s+__forceinline\s*$', s):   # row-local copy of the shared macro
            changed += 1; continue
        # replace only at code positions (positions where the stripped text has the token)
        new = []; last = 0
        for mm in TOK.finditer(ls):
            new.append(lr[last:mm.start()]); new.append('FABLE_FORCEINLINE'); last = mm.end(); changed += 1
        new.append(lr[last:]); out.append(''.join(new))
    if not changed: return 0
    if os.path.basename(path) == 'fable_compiler.h': return 0
    text = '\n'.join(out)
    if INC not in text:
        L = text.split('\n'); Ls = strip(text).split('\n')
        code = [i for i, l in enumerate(Ls) if l.strip()]
        at = code[0] if code else 0
        if code:
            first = Ls[code[0]].strip()
            if re.match(r'#\s*pragma\s+once\b', first): at = code[0] + 1
            else:
                g = re.match(r'#\s*ifndef\s+(\w+)', first)
                if g and len(code) > 1 and re.match(r'#\s*define\s+' + g.group(1) + r'\b', Ls[code[1]].strip()): at = code[1] + 1
        L.insert(at, INC + ('\r' if '\r\n' in raw else '')); text = '\n'.join(L)
    return changed, text

if __name__ == '__main__':
    apply = '--apply' in sys.argv; roots = [a for a in sys.argv[1:] if a != '--apply']
    nf = nt = 0; touched = []
    for root in roots:
        files = [root] if os.path.isfile(root) else glob.glob(root + '/**/*', recursive=True)
        for f in sorted(files):
            if not f.endswith(('.h', '.hpp', '.cpp', '.inl')) or not os.path.isfile(f): continue
            r = convert(f)
            if not r: continue
            n, text = r; nf += 1; nt += n; touched.append(f)
            if apply: open(f, 'w', encoding='latin-1', newline='').write(text)
    print(f'files {nf} tokens {nt}' + (' (applied)' if apply else ' (dry run)'))
    open(os.environ.get('FI_TOUCHED', '/dev/null'), 'w').write('\n'.join(touched) + '\n')
