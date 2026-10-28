#!/usr/bin/env python3
"""Respell CCharString / CWideString assignment operators to the retail PDB form.

Retail (Ego_r.pdb, FableWin.pdb) declares every overload public, non-virtual, returning
`const T&`:
    ??4CCharString@@QAEABV0@ABV0@@Z   operator=(const CCharString&)   0x0099EFB0
    ??4CCharString@@QAEABV0@PBD@Z     operator=(const char*)          0x0099EFE0
    ??4CCharString@@QAEABV0@D@Z       operator=(char)                 0x0099F040
    ??4CWideString@@QAEABV0@ABV0@@Z   operator=(const CWideString&)   0x0099B7D0
    ??4CWideString@@QAEABV0@PB_W@Z    operator=(const wchar_t*)       0x0099B800
    ??4CWideString@@QAEABV0@_W@Z      operator=(wchar_t)              0x0099B860

Edits (all byte-neutral):
  * `T& operator=(...)` -> `const T& operator=(...)` (insert `const ` before a non-const `T&`
    return) for an operator= declared inside a `class T`/`struct T` body, or defined out of
    line as `<ret> T::operator=(`, T in {CCharString, CWideString};
  * `T* operator=` -> `const T& operator=`, and `return this;` -> `return *this;` inside an
    out-of-line body of that operator (one row, 0x00992630, and its test).
Comments, string literals, qualified calls (`x.CCharString::operator=(y)`) and every other class
are left alone. Idempotent. The return type does not change the emitted code (the reference is
returned in eax either way); only the mangled name moves AAV0/PAV0 -> ABV0.

Left alone (`blocker` in --report):
  WCHAR_FLAG    wide pointer/char parameter that mangles PBG/G with the row's flags (no
                /Zc:wchar_t; the row's CompilerFlags are read from rebuild/build_candidates.ps1
                relative to $FABLE_ROOT or the cwd; tests always compile without it)
  STRUCT_KEY    `struct T` model (mangles U0, retail is class V0)
  NAMESPACED    model inside a namespace (mangles T@<ns>@@)
  MACRO_ALIAS   `#define T Alias` with `Alias::operator=` spelled out in the file
  ACCESS_*      not public (AAE/IAE)
  UNKNOWN_PARAM not one of the six retail overloads (test fakes)
  WITH_<x>      same (class, parameter) as a blocked declaration in the file, so a declaration
                and its definition never disagree

usage: transform.py [--report] [--flags "<compiler flags>"] <file>...
  default: rewrite the files in place (prints `edited <file> <n>` per changed file)
  --report: print one JSON object per file with every operator= decl found, no writes
"""
import json, re, sys

CLASSES = ("CCharString", "CWideString")


def mask(text):
    """Same-length copy of text with comments, string and char literals blanked."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            j = text.find('\n', i)
            j = n if j < 0 else j
            for k in range(i, j): out[k] = ' '
            i = j
        elif c == '/' and i + 1 < n and text[i + 1] == '*':
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != '\n': out[k] = ' '
            i = j
        elif c in '"\'':
            # raw prefix L"..." is fine: the L stays, the literal is blanked
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            j = min(j + 1, n)
            for k in range(i + 1, j - 1): out[k] = ' '
            i = j
        else:
            i += 1
    return ''.join(out)


def match_brace(m, pos):
    """pos is at '{'; return index of the matching '}' (or len)."""
    d = 0
    for k in range(pos, len(m)):
        if m[k] == '{': d += 1
        elif m[k] == '}':
            d -= 1
            if d == 0: return k
    return len(m)


def match_paren(m, pos):
    d = 0
    for k in range(pos, len(m)):
        if m[k] == '(': d += 1
        elif m[k] == ')':
            d -= 1
            if d == 0: return k
    return len(m)


CLASS_HEAD = re.compile(r'\b(class|struct)\s+(CCharString|CWideString)\b\s*(?::[^;{]*)?\{')
NS = re.compile(r'\bnamespace\s*(\w*)\s*\{')
OPEQ = re.compile(r'\boperator\s*=\s*\(')


def class_bodies(m):
    out = []
    for h in CLASS_HEAD.finditer(m):
        # skip `template <...> class`? not used for these classes; skip `friend class X {`
        lb = h.end() - 1
        rb = match_brace(m, lb)
        out.append((h.group(1), h.group(2), lb, rb))
    return out


def access_at(m, key, lb, pos):
    """Access in effect at pos inside the body starting at lb (depth-1 labels only)."""
    acc = 'public' if key == 'struct' else 'private'
    d = 0
    k = lb + 1
    lab = re.compile(r'\b(public|private|protected)\s*:(?!:)')
    while k < pos:
        ch = m[k]
        if ch == '{': d += 1
        elif ch == '}': d -= 1
        elif d == 0 and ch in 'ppp' and (k == 0 or not (m[k - 1].isalnum() or m[k - 1] == '_')):
            mm = lab.match(m, k)
            if mm:
                acc = mm.group(1)
                k = mm.end()
                continue
        k += 1
    return acc


def norm(s):
    return re.sub(r'\s+', ' ', s).strip()


TYPEDEF = re.compile(r'\btypedef\s+((?:unsigned\s+short|wchar_t|__wchar_t|unsigned\s+char|char)(?:\s+int)?)\s+([A-Za-z_]\w*)\s*;')


def typedefs(m):
    return {t.group(2): norm(t.group(1)).replace(' int', '') for t in TYPEDEF.finditer(m)}


def param_code(cls, ptxt, zc, tdefs=None):
    """Mangled parameter code for the operator=, or None if not a retail overload shape."""
    p = norm(ptxt)
    for k, v in (tdefs or {}).items():
        p = re.sub(r'\b%s\b' % re.escape(k), v, p)
    # drop a parameter name and default argument
    p = re.sub(r'\s*=.*$', '', p)
    p = re.sub(r'\s*\b[A-Za-z_]\w*$', lambda mm: '' if mm.group(0).strip() not in (
        'char', 'wchar_t', 'short', 'CCharString', 'CWideString', 'wchar16', '__wchar_t') else mm.group(0), p)
    p = p.replace(' &', '&').replace(' *', '*').strip()
    q = re.sub(r'\s+', ' ', p)
    if q in ('const %s&' % cls, '%s const&' % cls):
        return 'ABV0@'
    if cls == 'CCharString':
        if q in ('const char*', 'char const*'): return 'PBD'
        if q in ('char',): return 'D'
        return None
    wptr = {'const wchar_t*', 'wchar_t const*'}
    uptr = {'const unsigned short*', 'unsigned short const*', 'const wchar16*', 'wchar16 const*'}
    if q in wptr: return 'PB_W' if zc else 'PBG'
    if q in ('const __wchar_t*', '__wchar_t const*'): return 'PB_W'
    if q in uptr: return 'PBG'
    if q in ('wchar_t', 'const wchar_t'): return '_W' if zc else 'G'
    if q in ('__wchar_t',): return '_W'
    if q in ('unsigned short', 'wchar16', 'const unsigned short', 'const wchar16'): return 'G'
    return None


PDB_PARAMS = {'CCharString': {'ABV0@', 'PBD', 'D'}, 'CWideString': {'ABV0@', 'PB_W', '_W'}}

RET_TAIL = re.compile(
    r'(?P<ret>(?:\bconst\s+)?\b(?P<cls>CCharString|CWideString)\s*(?:\bconst\s*)?(?P<ref>[&*])|\bvoid)\s*$')


def analyze(text, zc=False):
    """Return (edits, decls). edits: list of insertion offsets for 'const '."""
    m = mask(text)
    tdefs = typedefs(m)
    bodies = class_bodies(m)
    decls = []
    for mo in OPEQ.finditer(m):
        pos = mo.start()
        lp = mo.end() - 1
        rp = match_paren(m, lp)
        ptxt = m[lp + 1:rp]
        cls = None; key = None; acc = None; outofline = False
        # out-of-line:  <ret> T::operator=(
        q = re.search(r'\b(CCharString|CWideString)\s*::\s*$', m[max(0, pos - 80):pos])
        before_end = pos
        if q:
            cls = q.group(1)
            before_end = max(0, pos - 80) + q.start()
            pre = m[max(0, before_end - 120):before_end]
            # a definition has its return type right before `T::`; a qualified call does not
            r = RET_TAIL.search(pre)
            if not r:
                continue
            outofline = True
        else:
            inside = [b for b in bodies if b[2] < pos < b[3]]
            if not inside:
                continue
            b = max(inside, key=lambda b: b[2])  # innermost
            # only direct members (no nested braces between body open and pos at deeper depth)
            seg = m[b[2] + 1:pos]
            if seg.count('{') != seg.count('}'):
                continue
            key, cls = b[0], b[1]
            acc = access_at(m, key, b[2], pos)
            pre = m[max(0, pos - 120):pos]
            r = RET_TAIL.search(pre)
        d = {'offset': pos, 'line': text.count('\n', 0, pos) + 1, 'class': cls, 'key': key,
             'access': acc, 'out_of_line': outofline, 'param': norm(text[lp + 1:rp]),
             'code': param_code(cls, ptxt, zc, tdefs),
             'param_type': param_code(cls, ptxt, True, tdefs) or norm(ptxt)}
        if r is None:
            d['ret'] = None; d['status'] = 'UNKNOWN_RET'
        else:
            rs = pre[r.start():r.end()]
            d['ret'] = norm(rs)
            # absolute offset of the return-type start
            base = (max(0, before_end - 120) if outofline else max(0, pos - 120))
            d['ret_off'] = base + r.start()
            d['ret_end'] = base + r.end()
            d['rparen'] = rp
            if r.group('cls') is None:
                d['status'] = 'VOID_RET'
            elif r.group('cls') != cls:
                d['status'] = 'OTHER_RET'
            elif r.group('ref') == '*':
                d['status'] = 'PTR_RET'
            elif re.search(r'\bconst\b', rs):
                d['status'] = 'OK'
            else:
                d['status'] = 'FIX'
        if d['code'] is None:
            d['blocker'] = 'UNKNOWN_PARAM'
        elif d['code'] not in PDB_PARAMS[cls]:
            d['blocker'] = 'WCHAR_FLAG'   # PBG / G: PDB spelling unreachable with these flags
        elif acc not in (None, 'public'):
            d['blocker'] = 'ACCESS_' + acc
        elif key == 'struct':
            d['blocker'] = 'STRUCT_KEY'
        d['symbol'] = ('??4%s@@QAEABV0@%s@Z' % (cls, d['code'] + ('@' if d['code'] == 'ABV0@' else ''))
                       if d['code'] else None)
        if d['code'] == 'ABV0@':
            d['symbol'] = '??4%s@@QAEABV0@ABV0@@Z' % cls
        decls.append(d)
    # namespace-enclosed models mangle `CCharString@N@@`: the PDB spelling needs the class moved
    # out of the namespace, which is not this transform's business
    nsr = [(n.end() - 1, match_brace(m, n.end() - 1)) for n in NS.finditer(m)]
    for d in decls:
        if 'blocker' not in d and any(a < d['offset'] < b for a, b in nsr):
            d['blocker'] = 'NAMESPACED'
    # `#define CWideString CWideStringCore` and `CWideStringCore::operator=` spelled out: the
    # alias-qualified definitions are invisible here, so leave that class alone in this file
    for al in re.finditer(r'#[ \t]*define[ \t]+(CCharString|CWideString)[ \t]+(\w+)', m):
        if re.search(r'\b%s\s*::\s*operator\s*=\s*\(' % re.escape(al.group(2)), m):
            for d in decls:
                if d['class'] == al.group(1) and 'blocker' not in d:
                    d['blocker'] = 'MACRO_ALIAS'
    # a declaration and its out-of-line definition must stay in agreement: if any spelling of
    # (class, parameter) is blocked, none of them is edited
    blocked = {(d['class'], d['code'], d['param_type']): d['blocker'] for d in decls if 'blocker' in d}
    for d in decls:
        k = (d['class'], d['code'], d['param_type'])
        if 'blocker' not in d and k in blocked:
            d['blocker'] = 'WITH_' + blocked[k]
    edits = {}
    for d in decls:
        if 'blocker' in d:
            continue
        if d['status'] == 'FIX':
            edits[d['ret_off']] = (0, 'const ')
        elif d['status'] == 'PTR_RET':
            # `T *operator=` / `T* T::operator=` -> `const T& ...`; an out-of-line body's
            # `return this;` becomes `return *this;` (same register value, retail's shape)
            span = text[d['ret_off']:d['ret_end']]
            new = re.sub(r'^(?:const\s+)?(\w+)\s*(?:const\s*)?\*\s*$', r'const \1& ', span)
            edits[d['ret_off']] = (d['ret_end'] - d['ret_off'], new)
            k = d['rparen'] + 1
            while k < len(m) and m[k] in ' \t\r\n': k += 1
            if k < len(m) and m[k] == '{':
                e = match_brace(m, k)
                for rt in re.finditer(r'\breturn\s+this\s*;', m[k:e]):
                    edits[k + rt.start()] = (rt.end() - rt.start(), 'return *this;')
    return sorted(edits.items()), decls


def apply(text, edits):
    for off, (ln, new) in sorted(edits, reverse=True):
        text = text[:off] + new + text[off + ln:]
    return text


_FLAGS = None


def row_flags(path):
    """CompilerFlags of the landed row a source file belongs to (tests compile /Od /W3)."""
    global _FLAGS
    mm = re.search(r'_([0-9a-f]{8})(_test)?\.cpp$', path)
    if not mm or mm.group(2):
        return ''
    if _FLAGS is None:
        import os
        root = os.environ.get('FABLE_ROOT', '.')
        _FLAGS = {}
        try:
            bc = open(os.path.join(root, 'rebuild', 'build_candidates.ps1'), encoding='utf-8').read()
        except OSError:
            bc = ''
        for b in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", bc, re.S):
            a = re.search(r"Address = '([0-9a-fA-F]{8})'", b.group(1))
            fl = re.search(r"CompilerFlags = '([^']*)'", b.group(1))
            if a: _FLAGS[a.group(1).lower()] = fl.group(1) if fl else ''
    return _FLAGS.get(mm.group(1), '')


def main(argv):
    report = False; zc = None; files = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == '--report': report = True
        elif a == '--flags': i += 1; zc = '/Zc:wchar_t' in argv[i]
        else: files.append(a)
        i += 1
    for f in files:
        raw = open(f, 'rb').read()
        text = raw.decode('utf-8', errors='surrogateescape')
        z = zc if zc is not None else ('/Zc:wchar_t' in row_flags(f))
        edits, decls = analyze(text, z)
        if report:
            print(json.dumps({'file': f, 'decls': decls, 'edits': len(edits)}))
            continue
        if edits:
            new = apply(text, edits)
            open(f, 'wb').write(new.encode('utf-8', errors='surrogateescape'))
            print('edited', f, len(edits))


if __name__ == '__main__':
    main(sys.argv[1:])
