# vecify.py: turn a row-local 12-byte vector<long> stand-in class (CLongVector / CRetailVector_* / CLongArray ..., with a push_back
#  that calls the 0x00440990 grow-insert) into STLport 4.6 std::vector<_Tp, allocator<_Tp> > with the protected
#  _M_insert_overflow(_Tp*, const _Tp&, const __true_type&, size_type = unsigned int, bool) -- the spelling the own row 0x00440990
#  defines (?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z). Name-only: member layout,
#  inline bodies and any same-TU stand-in body are kept; the class becomes the class template (element type _Tp) in namespace std
#  and every other use of the stand-in name becomes std::vector<long>.
import re, he
STD = ('namespace std {\ntemplate <class _Tp> class allocator { public: allocator() { } };\n'
       'struct __true_type { __true_type() { } };\n')
def vecify(a, cls, method, tag, elem='long', note='', stdtag='__true_type'):
    p, rel, t = he._rw(a, 'src')
    if 'template <class _Tp, class _Alloc = allocator<_Tp> > class vector' in t: print('already', a, 'src'); return
    # 1. the global tag struct goes (std::__true_type replaces it)
    t, n = re.subn(r'\nstruct ' + re.escape(tag) + r' \{ ' + re.escape(tag) + r'\(\) \{\s*\} \};[^\n]*', '', t)
    assert n == 1, (a, 'tag', n)
    STDX = STD.replace('__true_type', stdtag)
    # 2. the class block
    m = re.search(r'\n(?:struct|class) ' + re.escape(cls) + r'\s*\{', t)
    assert m, (a, 'class')
    e = he.block_end(t, m.start() + 1)
    blk = t[m.start() + 1:e]
    body = blk[blk.index('{') + 1:]
    head_comment = ''
    nl = body.find('\n')
    if body[:nl].strip().startswith('//'): head_comment = ' ' + body[:nl].strip(); body = body[nl:]
    body = re.sub(r'~' + re.escape(cls) + r'\(', '~vector(', body)
    body = re.sub(r'\b' + re.escape(cls) + r'\b', 'vector', body)
    body = re.sub(r'(?<!unsigned )\b' + elem + r'\b', '_Tp', body)
    body = re.sub(r'\b' + re.escape(method) + r'\(', '_M_insert_overflow(', body)
    body = re.sub(r'\b' + re.escape(tag) + r'\b', stdtag, body)
    # the grow-insert declaration: protected, size_type unsigned int
    dm = re.search(r'\n([ \t]*)void _M_insert_overflow\(([^;]*)\);([^\n]*)', body)
    assert dm, (a, 'decl')
    params = re.sub(r'unsigned long\b', 'unsigned int', dm.group(2))
    ind = dm.group(1)
    decl = '\nprotected:\n%svoid _M_insert_overflow(%s);%s\npublic:' % (ind, params, dm.group(3))
    body = body[:dm.start()] + decl + body[dm.end():]
    newblk = ('template <class _Tp, class _Alloc = allocator<_Tp> > class vector {%s\npublic:' % head_comment) + body
    t = t[:m.start() + 1] + STDX + newblk + '\n}\n' + t[e:]
    # 3. out-of-class definitions of the grow-insert
    def fx(mm):
        ps = re.sub(r'unsigned long\b', 'unsigned int', mm.group(3))
        ps = re.sub(r'\b' + re.escape(tag) + r'\b', 'std::' + stdtag, ps)
        ps = re.sub(r'(?<!unsigned )\b' + elem + r'\b', '_Tp', ps)
        # a member template is instantiated where auto_inline is back on: keep the row's out-of-line body out of line explicitly
        return 'template <class _Tp, class _Alloc>\n__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(%s)' % ps
    t, n = re.subn(r'((?:__declspec\(noinline\) )?)void ' + re.escape(cls) + r'::' + re.escape(method) + r'\(()([^)]*)\)', fx, t)
    # 4. every other use of the stand-in names
    i0 = t.index(STDX); i1 = i0 + len(STDX) + len(newblk) + 3
    rest = t[i1:]
    rest = re.sub(r'\b' + re.escape(cls) + r'\b', 'std::vector<' + elem + '>', rest)
    rest = re.sub(r'(?<!::)\b' + re.escape(tag) + r'\b', 'std::' + stdtag, rest)
    t = t[:i1] + rest
    if note:
        L = t.split('\n'); lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
        L.insert((lf[0] + 1) if lf else 1, note); t = '\n'.join(L)
    he._w(p, t); print('ok', a, 'src')

def stdvec(a, tag='__true_type', note=''):
    # a row whose std::vector is already a namespace-std class template without the allocator parameter (public QAE grow-insert):
    # add allocator<_Tp>, make the grow-insert protected (IAE), size_type unsigned int, the tag std::__true_type
    p, rel, t = he._rw(a, 'src')
    if 'class _Alloc = allocator<' in t: print('already', a, 'src'); return
    m = re.search(r'\ntemplate ?<class (\w+)> class vector \{\npublic:\n', t)
    assert m, (a, 'class')
    T = m.group(1)
    alloc = '' if re.search(r'class allocator\b', t) else 'template <class %s> class allocator { public: allocator() { } };\n' % T
    t = t[:m.start()] + '\n' + alloc + 'template <class %s, class _Alloc = allocator<%s> > class vector {\npublic:\n' % (T, T) + t[m.end():]
    if tag != '__true_type':
        t, n = re.subn(r'\nstruct ' + re.escape(tag) + r' \{ ' + re.escape(tag) + r'\(\) \{ \} \};[^\n]*', '', t); assert n == 1
        t = t.replace('namespace std {\n', 'namespace std {\nstruct __true_type { __true_type() { } };\n', 1)
        t = re.sub(r'\b' + re.escape(tag) + r'\b', '__true_type', t)
    # the in-class declaration: protected, unsigned int
    c0 = t.index('class _Alloc = allocator<'); c1 = he.block_end(t, c0)
    blk = t[c0:c1]
    dm = re.search(r'\n([ \t]*)((?:__declspec\(noinline\) )?)void _M_insert_overflow\(([^;]*)\);([^\n]*)', blk)
    assert dm, (a, 'decl')
    ps = re.sub(r'unsigned long\b', 'unsigned int', dm.group(3))
    blk = blk[:dm.start()] + '\nprotected:\n%s%svoid _M_insert_overflow(%s);%s\npublic:' % (dm.group(1), dm.group(2), ps, dm.group(4)) + blk[dm.end():]
    t = t[:c0] + blk + t[c1:]
    # out-of-class definitions
    def fx(mm):
        return 'template <class %s, class _Alloc>%s%svoid vector<%s, _Alloc>::_M_insert_overflow(%s)' % (mm.group(1), mm.group(2), mm.group(3) or '', mm.group(1), re.sub(r'unsigned long\b', 'unsigned int', mm.group(4)))
    t = re.sub(r'template ?<class (\w+)>(\s*\n)((?:__declspec\(noinline\) )?)void vector<\1>::_M_insert_overflow\(([^)]*)\)', fx, t)
    if note:
        L = t.split('\n'); lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
        L.insert((lf[0] + 1) if lf else 1, note); t = '\n'.join(L)
    he._w(p, t); print('ok', a, 'src')
