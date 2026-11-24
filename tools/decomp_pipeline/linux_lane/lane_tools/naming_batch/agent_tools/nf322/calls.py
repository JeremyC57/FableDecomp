# calls.py <addr> <Name>: every non-comment line of the row (landed source) that USES Name( as a call / temporary / mem-initializer,
# i.e. not a declaration (no type-and-param list): prints line number + text, and the next line when the call spans lines
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF322/tools'); import nf
a, name = sys.argv[1], sys.argv[2]
L = nf.src_of(a).read_text(encoding='utf-8', errors='replace').split('\n')
for n, l in enumerate(L, 1):
    c = l.split('//')[0]
    for m in re.finditer(r'(?<![\w~])' + re.escape(name) + r'\s*\(', c):
        rest = c[m.end():]
        if re.match(r'\s*(const\s+)?[A-Z]\w*(<[^>]*>)?\s*[*&]\s*\w+\s*[,)=]', rest) or re.match(r'\s*\)\s*[;{:]?\s*$', rest) and 'class' in c: continue
        if re.match(r'\s*(const\s+)?(unsigned\s+)?(long|float|bool|int|char)\s+\w+', rest): continue
        print('%s L%d: %s' % (a, n, l.strip()[:200]))
        if c.count('(') > c.count(')'): print('%s L%d+ %s' % (a, n + 1, L[n].strip()[:200]))
