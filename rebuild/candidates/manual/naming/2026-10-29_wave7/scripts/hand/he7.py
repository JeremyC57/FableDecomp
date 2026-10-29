# wave 7 hand-edit helpers on top of he.py (HE_OV=<overlay>; files copied from the repo on first touch)
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, path
def _rw(a, k, fn):
    p, rel = path(a, k)
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
    n = fn(t)
    if n is None: print('already', a, k); return
    open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(n); print('ok', a, k)
def edit_n(a, k, old, new, n):
    def f(t):
        c = t.count(old)
        if c == 0 and new in t: return None
        if c != n: raise SystemExit('COUNT %d!=%d in %s %s for %r' % (c, n, a, k, old[:80]))
        return t.replace(old, new)
    _rw(a, k, f)
def word(a, k, old, new, n=None, done=None):
    """whole-word rename old -> new (n = expected count, None = any >0); done = text whose presence means applied"""
    def f(t):
        c = len(re.findall(r'\b%s\b' % re.escape(old), t))
        if c == 0: return None
        if n is not None and c != n: raise SystemExit('WORD %d!=%d in %s %s for %s' % (c, n, a, k, old))
        return re.sub(r'\b%s\b' % re.escape(old), new, t)
    _rw(a, k, f)
def struct_to_class(a, k, name):
    """`struct name {` (first, with optional bases) -> `class name { public:` (keeps layout; the class key mangles V)"""
    def f(t):
        m = re.search(r'\bstruct([ \t]+%s\b[^;{]*)\{' % re.escape(name), t)
        if not m: return None
        if ':' in m.group(1) and not re.search(r':[ \t]*(public|private|protected)\b', m.group(1)):
            raise SystemExit('struct %s has a base without access specifier in %s' % (name, a))
        rest = t[m.end():]
        add = '' if re.match(r'[ \t]*public[ \t]*:', rest) else ' public:'
        return t[:m.start()] + 'class' + m.group(1) + '{' + add + t[m.end():]
    _rw(a, k, f)

def ip_specialize(a, standin, T, k='src'):
    """row-local stand-in struct for CIntelligentPointer<T> -> `template<> class CIntelligentPointer<T>` (explicit
    specialization: members mangle as the template instance, ??4?$CIntelligentPointer@VT@@@@QAEAAV0@ABV0@@Z)"""
    def f(t):
        if re.search(r'template[ \t]*<[ \t]*>[ \t]*class[ \t]+CIntelligentPointer[ \t]*<[ \t]*%s[ \t]*>' % re.escape(T), t) and \
                not re.search(r'\b(?:struct|class)[ \t]+%s\b(?![ \t]*<)[^;{]*\{' % re.escape(standin), t):
            return None
        m = re.search(r'\b(struct|class)[ \t]+%s\b(?![ \t]*<)([^;{]*)\{' % re.escape(standin), t)
        if not m: raise SystemExit('no stand-in %s in %s' % (standin, a))
        # body end: matching brace
        d = 0; i = m.end() - 1
        while True:
            if t[i] == '{': d += 1
            elif t[i] == '}':
                d -= 1
                if d == 0: break
            i += 1
        body = t[m.end():i]
        bases = m.group(2).strip(); cmt = ''
        if '//' in bases:
            bases, cmt = bases.split('//', 1); bases = bases.strip(); cmt = '   //' + cmt.rstrip()
        if bases:
            bm = re.fullmatch(r':[ \t]*(?:public[ \t]+)?(\w+)(.*)', bases, re.S)
            if not bm: raise SystemExit('bases %r in %s' % (bases, a))
            head = 'class CIntelligentPointer<%s> : public %s%s{' % (T, bm.group(1), bm.group(2) if bm.group(2).strip() else ' ')
        else:
            head = 'class CIntelligentPointer<%s> {' % T
        pub = '' if (m.group(1) == 'class' or re.match(r'[ \t]*public[ \t]*:', body)) else ' public:'
        body = re.sub(r'\b%s\b' % re.escape(standin), 'CIntelligentPointer', body)
        pre = t[:m.start()]; post = t[i:]
        rx = r'\b%s\b(?![ \t]*<)' % re.escape(standin)
        pre = re.sub(rx, 'CIntelligentPointer<%s>' % T, pre); post = re.sub(rx, 'CIntelligentPointer<%s>' % T, post)
        post = post.replace('CIntelligentPointer<%s>::CIntelligentPointer<%s>' % (T, T), 'CIntelligentPointer<%s>::CIntelligentPointer' % T)
        post = post.replace('CIntelligentPointer<%s>::~CIntelligentPointer<%s>' % (T, T), 'CIntelligentPointer<%s>::~CIntelligentPointer' % T)
        fwd = '' if re.search(r'template[ \t]*<[ \t]*(?:class|typename)[ \t]+\w+[ \t]*>[ \t\n]*class[ \t]+CIntelligentPointer\b', pre) else 'template <class T> class CIntelligentPointer;\n'
        ls = pre.rfind('\n') + 1
        out = pre[:ls] + fwd + pre[ls:] + 'template <> ' + head + pub + cmt + body + post
        return out.replace('CIntelligentPointer<%s>>' % T, 'CIntelligentPointer<%s> >' % T)
    _rw(a, k, f)
