# acc.py: move member declaration lines into a given access section, keeping the access of everything else.
import re
LBL = re.compile(r'^\s*(public|protected|private)\s*:\s*(//.*)?$')
OPEN = re.compile(r'\b(class|struct)\s+(\w+)\b[^;{()]*\{')
def access_at(lines, i):
    """access in force at line i (scan back to the enclosing class/struct head); None if not found"""
    depth = 0
    for j in range(i - 1, -1, -1):
        l = re.sub(r'//.*', '', lines[j])
        m = LBL.match(lines[j])
        if m and depth == 0: return m.group(1)
        depth += l.count('}') - l.count('{')
        if depth < 0:
            o = OPEN.search(l)
            if not o: return None
            # labels written on the head line itself: `class X { public:`
            tail = l[o.end():]
            m2 = re.findall(r'\b(public|protected|private)\s*:', tail)
            if m2: return m2[-1]
            return 'private' if o.group(1) == 'class' else 'public'
    return None
def put(text, pred, want):
    """lines for which pred(line) is true are moved into section `want` (labels inserted around consecutive runs)"""
    lines = text.split('\n'); out = []; i = 0; n = 0
    while i < len(lines):
        if pred(lines[i]):
            acc = access_at(lines, i)
            if acc is None: return None
            j = i
            while j < len(lines) and pred(lines[j]): j += 1
            if acc == want: out.extend(lines[i:j])
            else:
                ind = re.match(r'\s*', lines[i]).group(0)
                pre = ind[:-4] if len(ind) >= 4 else ''
                out.append(pre + want + ':'); out.extend(lines[i:j]); out.append(pre + acc + ':'); n += 1
            i = j
        else: out.append(lines[i]); i += 1
    return '\n'.join(out)
