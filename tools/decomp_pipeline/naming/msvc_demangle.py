#!/usr/bin/env python3
"""msvc_demangle.py - MSVC (VC7.1) name demangler good enough for callee naming.

demangle(sym) -> dict(kind, qual, cls, name, tmpl, conv, this_cv, ret, params, ok, raw)
  qual   : fully qualified name  'NScope::CClass<int>::Method'
  cls    : scope part            'NScope::CClass<int>'
  name   : last component        'Method' / '~CClass' / 'operator==' / 'CClass'
  base   : name with template args stripped, for conflict comparison
Handles: template args (types, numeric $0, address $1, nested templates), special names
(?0 ctor, ?1 dtor, ?_7 vftable, ?_E/?_G deleting dtors, ?_C ..), operators, anonymous
namespaces, name/type back-references, extern "C" (_name), stdcall _name@N, fastcall @name@N,
function pointer types, arrays, cv/ref qualifiers.  Never raises: on parse failure returns a
best-effort result with ok=False.
"""
import re

BASIC = {'X': 'void', 'D': 'char', 'C': 'signed char', 'E': 'unsigned char', 'F': 'short', 'G': 'unsigned short',
         'H': 'int', 'I': 'unsigned int', 'J': 'long', 'K': 'unsigned long', 'M': 'float', 'N': 'double',
         'O': 'long double', 'Z': '...'}
BASIC_ = {'N': 'bool', 'J': '__int64', 'K': 'unsigned __int64', 'W': 'wchar_t', 'L': '__int128', 'M': 'unsigned __int128',
          'D': '__int8', 'E': 'unsigned __int8', 'F': '__int16', 'G': 'unsigned __int16', 'H': '__int32', 'I': 'unsigned __int32',
          'S': 'char16_t', 'U': 'char32_t', 'Q': 'char8_t'}
OPS = {'0': '<ctor>', '1': '<dtor>', '2': 'operator new', '3': 'operator delete', '4': 'operator=', '5': 'operator>>',
       '6': 'operator<<', '7': 'operator!', '8': 'operator==', '9': 'operator!=', 'A': 'operator[]', 'B': 'operator <cast>',
       'C': 'operator->', 'D': 'operator*', 'E': 'operator++', 'F': 'operator--', 'G': 'operator-', 'H': 'operator+',
       'I': 'operator&', 'J': 'operator->*', 'K': 'operator/', 'L': 'operator%', 'M': 'operator<', 'N': 'operator<=',
       'O': 'operator>', 'P': 'operator>=', 'Q': 'operator,', 'R': 'operator()', 'S': 'operator~', 'T': 'operator^',
       'U': 'operator|', 'V': 'operator&&', 'W': 'operator||', 'X': 'operator*=', 'Y': 'operator+=', 'Z': 'operator-=',
       '_0': 'operator/=', '_1': 'operator%=', '_2': 'operator>>=', '_3': 'operator<<=', '_4': 'operator&=',
       '_5': 'operator|=', '_6': 'operator^=', '_7': "`vftable'", '_8': "`vbtable'", '_9': "`vcall'",
       '_A': "`typeof'", '_B': "`local static guard'", '_C': "`string'", '_D': "`vbase destructor'",
       '_E': "`vector deleting destructor'", '_F': "`default constructor closure'", '_G': "`scalar deleting destructor'",
       '_H': "`vector constructor iterator'", '_I': "`vector destructor iterator'",
       '_J': "`vector vbase constructor iterator'", '_K': "`virtual displacement map'",
       '_L': "`eh vector constructor iterator'", '_M': "`eh vector destructor iterator'",
       '_N': "`eh vector vbase constructor iterator'", '_O': "`copy constructor closure'",
       '_P': "`udt returning'", '_R': "RTTI", '_S': "`local vftable'", '_T': "`local vftable constructor closure'",
       '_U': 'operator new[]', '_V': 'operator delete[]', '_X': "`placement delete closure'",
       '_Y': "`placement delete[] closure'", '__A': "`managed vector constructor iterator'",
       '__B': "`managed vector destructor iterator'", '__C': "`eh vector copy constructor iterator'",
       '__D': "`eh vector vbase copy constructor iterator'", '__E': "`dynamic initializer'",
       '__F': "`dynamic atexit destructor'", '__G': "`vector copy constructor iterator'",
       '__H': "`vector vbase copy constructor iterator'", '__I': "`managed vector copy constructor iterator'",
       '__J': "`local static thread guard'", '__K': "operator \"\"", }
CONV = {'A': '__cdecl', 'B': '__cdecl', 'C': '__pascal', 'D': '__pascal', 'E': '__thiscall', 'F': '__thiscall',
        'G': '__stdcall', 'H': '__stdcall', 'I': '__fastcall', 'J': '__fastcall', 'M': '__clrcall', 'Q': '__vectorcall'}


class Fail(Exception):
    pass


class P:
    def __init__(self, s):
        self.s = s; self.i = 0; self.names = []; self.types = []

    def peek(self, n=1): return self.s[self.i:self.i + n]
    def eof(self): return self.i >= len(self.s)

    def take(self, n=1):
        if self.i + n > len(self.s): raise Fail('eof')
        r = self.s[self.i:self.i + n]; self.i += n; return r

    def expect(self, c):
        if self.peek(len(c)) != c: raise Fail('expected %r at %d in %s' % (c, self.i, self.s))
        self.i += len(c)

    def ident(self):
        j = self.s.find('@', self.i)
        if j < 0: raise Fail('ident')
        r = self.s[self.i:j]; self.i = j + 1; return r

    # ---- numbers ----
    def number(self):
        neg = False
        if self.peek() == '?': self.take(); neg = True
        c = self.peek()
        if c.isdigit(): self.take(); v = int(c) + 1
        else:
            v = 0
            while self.peek() != '@':
                ch = self.take()
                if not 'A' <= ch <= 'P': raise Fail('num')
                v = v * 16 + (ord(ch) - 65)
            self.take()
        return -v if neg else v

    # ---- names ----
    def template_name(self, fresh=True):
        # after '?$'
        saved = (self.names, self.types)
        if fresh: self.names = []; self.types = []
        if self.peek() == '?':
            self.take()
            if self.peek() == '_':
                code = self.take(3) if self.peek(2) == '__' else self.take(2)
            else: code = self.take(1)
            nm = OPS.get(code, '?' + code)
        else:
            nm = self.ident(); self.names.append(nm)
        args = []
        while self.peek() != '@':
            if self.eof(): raise Fail('targs')
            args.append(self.targ())
        self.take()
        if fresh: self.names, self.types = saved
        full = nm + '<' + ','.join(args) + '>'
        return full

    def targ(self):
        c = self.peek()
        if c == '$':
            c2 = self.peek(2)
            if c2 == '$0': self.take(2); return str(self.number())
            if c2 == '$1': self.take(2); return '&' + self.fullname_as_str()
            if c2 == '$2':  # float
                self.take(2); m = self.number(); e = self.number(); return '%de%d' % (m, e)
            if c2 == '$D': self.take(2); return '`template-parameter-%d\'' % self.number()
            if c2 == '$E': self.take(2); return self.fullname_as_str()
            if c2 == '$F': self.take(2); a = self.number(); b = self.number(); return '{%d,%d}' % (a, b)
            if c2 == '$G': self.take(2); a = self.number(); b = self.number(); cc = self.number(); return '{%d,%d,%d}' % (a, b, cc)
            if c2 == '$H': self.take(2); a = self.fullname_as_str(); b = self.number(); return '{%s,%d}' % (a, b)
            if c2 == '$I': self.take(2); a = self.fullname_as_str(); b = self.number(); cc = self.number(); return '{%s,%d,%d}' % (a, b, cc)
            if c2 == '$J': self.take(2); a = self.fullname_as_str(); b = self.number(); cc = self.number(); d = self.number(); return '{%s,%d,%d,%d}' % (a, b, cc, d)
            if c2 == '$S': self.take(2); return ''
            if c2 == '$Q':  # template parameter pack / rvalue ref types handled in type()
                pass
            if c2 == '$$':
                return self.type()
            return self.type()
        if c == '?':  # ?N template parameter index
            self.take(); return '`template-parameter-%d\'' % self.number()
        return self.type()

    def fullname_as_str(self):
        self.expect('?')
        comps = self.qualname()
        # skip the type info for the referenced symbol (best effort)
        if not self.eof():
            try:
                self.decl_rest(comps)
            except Fail:
                pass
        return '::'.join(comps)

    def namecomp(self, first=False):
        c = self.peek()
        if c.isdigit():
            self.take()
            k = int(c)
            if k >= len(self.names): raise Fail('backref')
            return self.names[k]
        if c == '?':
            c2 = self.peek(2)
            if c2 == '?$':
                self.take(2)
                if first and self.i == 3:
                    return self.template_name()  # top-level template: not in the outer back-ref table
                nm = self.template_name(); self.names.append(nm); return nm
            if c2 == '?A' and first is False:  # anonymous namespace ?A0x1234@
                self.take(2); self.ident(); nm = '`anonymous namespace\''; self.names.append(nm); return nm
            if first:
                self.take()
                if self.peek() == '_':
                    if self.peek(2) == '__': code = self.take(3)
                    else: code = self.take(2)
                else:
                    code = self.take(1)
                if code == '_C':  # string literal
                    return "`string'"
                if code == 'B':
                    return 'operator <cast>'
                if code == '_R':  # RTTI descriptors
                    sub = self.take(1)
                    if sub == '0':
                        t = self.type(); return "%s `RTTI Type Descriptor'" % t
                    if sub == '1':
                        a = [self.number() for _ in range(4)]; return "`RTTI Base Class Descriptor at (%d,%d,%d,%d)'" % tuple(a)
                    return {'2': "`RTTI Base Class Array'", '3': "`RTTI Class Hierarchy Descriptor'",
                            '4': "`RTTI Complete Object Locator'"}.get(sub, "`RTTI'")
                return OPS.get(code, '?' + code)
            # numbered local scope: ?N inside scopes, e.g. ?1??func@@YAXXZ@ -> `func'::`2'
            if c2[1].isdigit() or c2 == '??':
                self.take()
                n = self.number() if self.peek() != '?' else 0
                if self.peek() == '?':
                    self.take()  # nested full symbol follows until matching @
                    inner = self.fullname_as_str()
                    self.expect('@') if self.peek() == '@' else None
                    return "`%s'::`%d'" % (inner, n + 1)
                return "`%d'" % (n + 1)
            if c2 == '?A':
                self.take(2); self.ident(); nm = '`anonymous namespace\''; self.names.append(nm); return nm
            raise Fail('namecomp ?')
        nm = self.ident(); self.names.append(nm); return nm

    def qualname(self):
        """returns [outermost ... innermost] components; consumes trailing '@'"""
        first = self.namecomp(first=True)
        comps = [first]
        while self.peek() != '@':
            if self.eof(): raise Fail('qual eof')
            comps.append(self.namecomp())
        self.take()
        return list(reversed(comps))

    # ---- types ----
    def cv(self):
        c = self.take()
        if c == 'E' or c == 'F' or c == 'I':  # __ptr64 / __unaligned / __restrict prefixes
            return self.cv()
        return {'A': '', 'B': 'const', 'C': 'volatile', 'D': 'const volatile',
                'Q': '', 'R': 'const', 'S': 'volatile', 'T': 'const volatile',
                'U': '', 'V': 'const', 'W': 'volatile', 'X': 'const volatile',
                'Y': '', 'Z': 'const', '0': 'volatile', '1': 'const volatile',
                '2': '', '3': 'const', '4': 'volatile', '5': 'const volatile'}.get(c, '')

    def functype(self):
        conv = CONV.get(self.take(), '?')
        ret = self.rettype()
        params = self.params()
        return '%s %s(%s)' % (ret, conv, ','.join(params))

    def rettype(self):
        if self.peek() == '?':  # cv-qualified return (class by value)
            self.take(); self.cv()
        return self.type()

    def params(self):
        ps = []
        if self.peek() == 'X':
            self.take(); ps = ['void']
        else:
            while self.peek() != '@' and self.peek() != 'Z':
                if self.eof(): raise Fail('params')
                c = self.peek()
                if c.isdigit():
                    self.take(); k = int(c)
                    if k >= len(self.types): raise Fail('tbackref')
                    ps.append(self.types[k]); continue
                st = self.i
                t = self.type()
                if self.i - st > 1: self.types.append(t)
                ps.append(t)
            if self.peek() == '@': self.take()
        self.expect('Z')
        return ps

    def ptr(self, kind):
        # kind: '*' or '&' ; after P/Q/R/S or A/B
        if self.peek() == '6':  # function pointer
            self.take(); ft = self.functype()
            return ft.replace('(', '(%s)(' % kind, 1) if False else '%s (%s)' % (ft.split(' (')[0], kind) + '(' + ft.split('(', 1)[1]
        if self.peek() == '8':  # member function pointer: 8 + class + conv ...
            self.take(); cls = '::'.join(self.qualname()); self.cv(); ft = self.functype()
            return '%s (%s::%s)(%s' % (ft.split(' (')[0], cls, kind, ft.split('(', 1)[1])
        q = self.cv()
        t = self.type()
        return t + (' ' + q if q else '') + kind

    def type(self):
        c = self.take()
        if c in BASIC: return BASIC[c]
        if c == '_':
            c2 = self.take()
            if c2 in BASIC_: return BASIC_[c2]
            if c2 == '$':  # _$$ ... (rare)
                return '?_$'
            raise Fail('type _' + c2)
        if c in 'UVTW':
            if c == 'W':
                self.take()  # enum base type digit
                return 'enum ' + '::'.join(self.qualname())
            return {'U': 'struct ', 'V': 'class ', 'T': 'union '}[c] + '::'.join(self.qualname())
        if c in 'PQRS':
            q = {'P': '', 'Q': ' const', 'R': ' volatile', 'S': ' const volatile'}[c]
            return self.ptr('*') + q
        if c in 'AB':
            return self.ptr('&') + (' volatile' if c == 'B' else '')
        if c == 'Y':  # array
            n = self.number(); dims = [self.number() for _ in range(n)]
            t = self.type(); return t + ''.join('[%d]' % d for d in dims)
        if c == '?':  # cv-qualified type (template args)
            q = self.cv(); t = self.type(); return (t + ' ' + q).strip()
        if c == '$':
            c2 = self.take()
            if c2 == '$':
                c3 = self.take()
                if c3 == 'Q': return self.ptr('&&')
                if c3 == 'R': return self.ptr('&&') + ' volatile'
                if c3 == 'C': q = self.cv(); t = self.type(); return (t + ' ' + q).strip()
                if c3 == 'A':  # function type
                    self.expect('6'); return self.functype()
                if c3 == 'B': return self.type()
                if c3 == 'T': return 'std::nullptr_t'
                if c3 == 'V': return ''
                if c3 == 'Z': return ''
                if c3 == 'Y': return '::'.join(self.qualname())  # alias template
                if c3 == 'S': return ''
                raise Fail('$$' + c3)
            if c2 == '0': return str(self.number())
            if c2 == '1': return '&' + self.fullname_as_str()
            raise Fail('$' + c2)
        raise Fail('type ' + c)

    # ---- declaration tail ----
    def decl_rest(self, comps):
        """parse what follows the qualified name; returns dict"""
        r = {'kind': 'func', 'conv': '', 'this_cv': '', 'ret': '', 'params': [], 'storage': ''}
        c = self.take()
        if c == '$':
            c2 = self.take()
            if c2 == '$':  # $$F / $$H  managed etc.
                self.take(); c = self.take()
            elif c2 in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789':
                # vtordisp / thunk kinds: $0,$1.. $B, $R...
                if c2 == 'B': self.number(); c = 'A'
                elif c2 == 'R':
                    self.take(); [self.number() for _ in range(4)]; c = 'A'
                elif c2 in '0123456789':
                    self.number(); self.number(); c = 'A'
                else: c = c2
        if c in '0123456789':  # data: access + type + storage
            r['kind'] = 'data'
            if c in '67':
                r['kind'] = 'vftable'; r['storage'] = self.cv()
                if self.peek() == '@': self.take()
                else: r['for'] = '::'.join(self.qualname())
            elif c in '01234567':
                r['ret'] = self.type(); r['storage'] = self.cv() if not self.eof() else ''
            elif c in '67':
                r['kind'] = 'vftable'; r['storage'] = self.cv()
                if self.peek() == '@': self.take()
                else: r['for'] = '::'.join(self.qualname())
            elif c == '8': r['kind'] = 'vftable_like'; r['storage'] = ''
            else:  # 9 : extern C-ish / anonymous
                r['kind'] = 'data'
            return r
        if c in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
            idx = ord(c) - 65
            # member functions: A..V (pairs: normal, static, virtual ...) ; Y/Z = global
            if c in 'YZ':
                r['conv'] = CONV.get(self.take(), '?')
            elif c in 'ABCDEFGHIJKLMNOPQRSTUVWX':
                is_static = c in 'CDKLST'
                # thunk types G/H/O/P/W/X carry an adjustor number
                if c in 'GHOPWX': self.number()
                if not is_static:
                    r['this_cv'] = self.cv()
                r['conv'] = CONV.get(self.take(), '?')
                if c in 'EFMNUV': r['virtual'] = True
                if is_static: r['static'] = True
            if self.peek() == '@':
                self.take(); r['ret'] = ''
            else:
                r['ret'] = self.rettype()
            r['params'] = self.params()
            return r
        raise Fail('decl ' + c)


def _fix_special(comps):
    cls = comps[-2] if len(comps) > 1 else ''
    last = comps[-1]
    plain = re.sub(r'<.*', '', cls)
    m = re.match(r'^<(ctor|dtor)>(<.*>)?$', last)
    if m:
        comps[-1] = ('~' if m.group(1) == 'dtor' else '') + plain + (m.group(2) or '')
    return comps


def demangle(sym):
    raw = sym
    out = {'raw': raw, 'ok': True, 'kind': 'func', 'conv': '', 'this_cv': '', 'ret': '', 'params': [], 'c': False}
    s = sym
    if s.startswith('__imp_'):
        out['import'] = True; s = s[6:]
    if not s.startswith('?'):
        # C-style: _name, _name@N (stdcall), @name@N (fastcall), plain
        out['c'] = True
        m = re.match(r'^@([^@]+)@(\d+)$', s)
        if m: out['conv'] = '__fastcall'; nm = m.group(1)
        else:
            m = re.match(r'^_([^@]+)@(\d+)$', s)
            if m: out['conv'] = '__stdcall'; nm = m.group(1)
            else:
                nm = s[1:] if s.startswith('_') else s   # __cdecl: exactly one leading underscore
                out['conv'] = '__cdecl'
        out.update(qual=nm, cls='', name=nm, comps=[nm])
        return out
    p = P(s)
    try:
        p.take()  # '?'
        comps = p.qualname()
        comps = _fix_special(comps)
        rest = p.decl_rest(comps)
        out.update(rest)
    except Fail as e:
        out['ok'] = False; out['err'] = str(e)
        try:
            comps  # noqa
        except NameError:
            comps = [s]
        comps = _fix_special(comps) if comps else [s]
    except Exception as e:  # noqa
        out['ok'] = False; out['err'] = 'crash ' + repr(e)
        comps = [s]
    out['comps'] = comps
    out['qual'] = '::'.join(comps)
    out['cls'] = '::'.join(comps[:-1])
    out['name'] = comps[-1]
    return out


def strip_tmpl(n):
    """remove template argument lists (balanced) from a qualified name"""
    o = []; depth = 0
    for ch in n:
        if ch == '<': depth += 1
        elif ch == '>': depth -= 1
        elif depth == 0: o.append(ch)
    return ''.join(o)


def pretty(sym):
    d = demangle(sym)
    if d['c']: return d['qual']
    sig = ''
    if d['kind'] == 'func' and d['ok']:
        sig = '(' + ','.join(d['params']) + ')' + ((' ' + d['this_cv']) if d['this_cv'] else '')
        if d['ret']: sig = d['ret'] + ' ' + d['qual'] + sig
        else: sig = d['qual'] + sig
        return sig
    return d['qual']


if __name__ == '__main__':
    import sys
    for a in sys.argv[1:]:
        d = demangle(a); print(a, '->', pretty(a), '' if d['ok'] else '[PARTIAL %s]' % d.get('err'))
