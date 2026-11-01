# back_expr / split_args / depth_at / top_start (from fam_mem.py)
import re
def back_expr(c, i):
    # c[i] is the char right before '->' or '.': scan back over a postfix expression
    j = i
    while j >= 0:
        ch = c[j]
        if ch in ')]':
            d = 0; op = '(' if ch == ')' else '['
            while j >= 0:
                if c[j] in ')]': d += 1
                elif c[j] in '([': d -= 1
                if d == 0: break
                j -= 1
            j -= 1
            # a cast / call: identifier before '(' belongs to it
            while j >= 0 and (c[j].isalnum() or c[j] == '_'): j -= 1
        elif ch.isalnum() or ch == '_':
            while j >= 0 and (c[j].isalnum() or c[j] == '_'): j -= 1
        else: break
        k = j
        while k >= 0 and c[k] in ' \t': k -= 1
        if k >= 1 and c[k - 1:k + 1] == '->': j = k - 2; continue
        if k >= 0 and c[k] == '.': j = k - 1; continue
        if k >= 1 and c[k - 1:k + 1] == '::': j = k - 2; continue
        break
    return j + 1
def split_args(s):
    out = []; d = 0; cur = ''
    for ch in s:
        if ch in '([': d += 1
        elif ch in ')]': d -= 1
        if ch == ',' and d == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
def depth_at(c, pos):
    d = 0
    for ch in c[:pos]:
        if ch == '{': d += 1
        elif ch == '}': d -= 1
    return d
def top_start(c, pos):
    # start of the line of the top-level (depth 0) construct containing pos
    d = 0; last0 = 0
    for k, ch in enumerate(c[:pos]):
        if ch == '{':
            if d == 0: pass
            d += 1
        elif ch == '}': d -= 1
        elif ch == ';' and d == 0: last0 = k + 1
        if ch == '}' and d == 0: last0 = k + 1
    p = c.find('\n', last0)
    while p >= 0 and p < pos and not c[p + 1:c.find('\n', p + 1)].strip(): p = c.find('\n', p + 1)
    return (p + 1) if 0 <= p < pos else last0
