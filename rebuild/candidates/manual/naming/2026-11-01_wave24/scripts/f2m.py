# f2m: respell a stand-in callee declaration into the PDB member and rewrite its calls.
import re
def bal(t, i):            # t[i] == '(' -> index after the matching ')'
    d = 0
    for j in range(i, len(t)):
        d += {'(': 1, ')': -1}.get(t[j], 0)
        if d == 0: return j + 1
    return -1
def code_only(t): return re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), t)
def free_to_member(t, fname, cls_decl, cls, call_fmt, ret_new):
    """free `RET __fastcall fname(ARG);` (one declaration, one parameter) -> cls_decl (inserted in its place) and every call
    fname(e) -> call_fmt % e (cast to RET when RET differs from ret_new). Returns (text, ncalls) or (None, reason)."""
    c = code_only(t)
    dm = list(re.finditer(r'^([^\n;{}]*?)\b__fastcall\s+' + re.escape(fname) + r'\s*\(([^()]*)\)\s*;[^\n]*\n', c, re.M))
    if len(dm) != 1: return None, '%d declarations of %s' % (len(dm), fname)
    if re.search(r'\b' + re.escape(fname) + r'\s*\([^()]*\)\s*\{', c): return None, '%s defined here' % fname
    if ',' in dm[0].group(2): return None, 'more than one parameter'
    ret = re.sub(r'\bextern\b(\s*"C")?\s*', '', dm[0].group(1)).strip()
    if not ret: return None, 'no return type'
    if re.search(r'\b(class|struct)\s+' + re.escape(cls) + r'\b[^;]*\{', c): return None, '%s already defined here' % cls
    t = t[:dm[0].start()] + cls_decl + '\n' + t[dm[0].end():]
    out = []; i = 0; n = 0
    c = code_only(t)
    for m in re.finditer(r'(?<![\w:>.])' + re.escape(fname) + r'\s*\(', c):
        if m.start() < i: continue
        j = bal(c, m.end() - 1)
        arg = t[m.end():j - 1].strip()
        if not arg or ',' in re.sub(r'\([^()]*\)', '', arg): return None, 'call with %r' % arg[:40]
        e = call_fmt % ('(' + arg + ')' if not re.match(r'^[\w]+$', arg) else arg)
        if re.sub(r'\s+', '', ret) != re.sub(r'\s+', '', ret_new): e = '((%s)%s)' % (ret, e)
        out.append(t[i:m.start()] + e); i = j; n += 1
    out.append(t[i:])
    if n == 0: return None, 'no calls'
    return ''.join(out), n
