# h17_gdt.py: naming wave 17, family gdt -- the c675-c676 ??_G scalar deleting destructors landed as free __fastcall
# `*_scalar_deleting_destructor` / `CLandscapeBackgroundPatch_vector_deleting_destructor` / `?ScalarDeletingDestructor@` stand-ins
# are re-authored on the proven ??_G<Class>@@UAEPAXI@Z (QAE for CEngineMap): the compiler emits the ??_G (30 bytes: call ??1,
# test flag bit 0, operator delete, `ret 4`) from the class model -- a public virtual destructor declared out of line (the
# landed ??1 row) and the class's Ego_r/FableWin constructor defined with an empty body (it emits the vtable and the ??_G; the
# constructor body is its own row), as the landed 0x00BB6520 / 0x00F29DC0 emitters do. Classes with no PDB constructor
# (GameCODA-side CASoundManager / CSoundBankLH / CSoundManagerLH) use the landed MakeOne_<addr> `new` emitter; the non-virtual
# CEngineMap uses the landed Kill_<addr> `delete` emitter. Name-only: the recheck proves the bytes unchanged.
import os, sys, re, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import he, und, inputs, struct
def split_args(s):
    out = []; d = 0; cur = ''
    for ch in s:
        if ch in '(<': d += 1
        elif ch in ')>': d -= 1
        if ch == ',' and d == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
R = he.R
W = he.W
BT, BS = inputs.bindings()
NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
CAT = {}
for l in open(he.rows.ROOT + '/rebuild/oracles/auto-re-candidates.tsv'):
    s = l.rstrip('\n').split('\t')
    if len(s) >= 4: CAT[s[0].lower()] = s
Q = [l.rstrip('\n').split('\t') + [''] * 5 for l in open(os.path.join(W, 'QUEUE_W17_input.tsv'))]
# the chosen PDB constructor per class (the simplest of the PDB set: no std::vector / enum-reference parameters)
CTOR = {
 'CRenderManager2D': '??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z',
 'CRenderManager': '??0CRenderManager@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z',
 'CNavQuadTreeNode': '??0CNavQuadTreeNode@@QAE@ABVC2DVector@@EEAAVCAFile@@@Z',
 'CNavNavigableLeafNode': '??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z',
 'CNavSwitchableLeafNode': '??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEEABV?$vector@_KV?$allocator@_K@std@@@std@@@Z',
}
NOPDB = {'CASoundManager', 'CSoundBankLH', 'CSoundManagerLH'}
def ctor_of(cls, mq):
    if cls in CTOR: return CTOR[cls]
    c = sorted(n for n in PDB if n.startswith('??0' + mq + '@@') and '?$vector' not in n and 'W4' not in n)
    return c[0] if c else None
def typ(s):
    s = re.sub(r'\b(class|struct|union) ', '', s.strip())
    for suf, k in ((' const &', '&'), (' const *', '*')):
        if s.endswith(suf): s = 'const ' + s[:-len(suf)] + k
    s = re.sub(r'\s*\*', '*', s); s = re.sub(r'\s*&', '&', s); s = s.replace('>>', '> >')
    return s
def call_target(a):
    bb = bytes.fromhex(CAT[a][3]); i = bb.index(b'\xe8', 3)
    return '%08X' % ((int(a, 16) + i + 5 + struct.unpack('<i', bb[i + 1:i + 5])[0]) & 0xffffffff)
def caddr(sym):
    b = BS.get(sym)
    return (' 0x' + b[0].upper()) if b else ''
FWD_STD = 'namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }\n'
def build(a, gsym, ev, chunk):
    m = re.match(r'\?\?_G(.*?)@@([QU])AEPAXI@Z', gsym); cq = m.group(1).split('@')[::-1]; virt = m.group(2) == 'U'
    cls = cq[-1]; scopes = cq[:-1]; full = '::'.join(cq)
    dsym = '??1%s@@%sAE@XZ' % (m.group(1), 'U' if virt else 'Q')
    dt = call_target(a)
    old = he._rw(a, 'src')[2]
    oldname = re.findall(r'(\w+_(?:scalar|vector)_deleting_destructor|\w+::ScalarDeletingDestructor|ScalarDeletingDestructor)\s*\(', old)
    oldcallee = re.findall(r'\b(\w+_dtor(?:_[0-9a-f]{8})?)\s*\(|this->(Dtor)\(\)', old)
    oldn = oldname[0] if oldname else '?'
    oldc = [x for t in oldcallee for x in t if x]
    if 'ScalarDeletingDestructor' in oldn: oldn = '%s::ScalarDeletingDestructor (member stand-in)' % (re.findall(r'(\w+)::ScalarDeletingDestructor', old) or ['?'])[0]
    else: oldn = 'free __fastcall %s' % oldn
    oldc = (oldc[0] if oldc else '?')
    if oldc == 'Dtor': oldc = '%s::Dtor()' % cls
    fwd = []; body = []
    base = None
    if cls == 'CRenderManager': base = 'CRenderManager2D'
    emit = ''
    ctor = None if (cls in NOPDB or not virt) else ctor_of(cls, m.group(1))
    lines = []
    if ctor:
        d = und.get([ctor])[ctor]
        mm = re.match(r'^(public|private|protected): __thiscall [\w:]+\((.*)\)$', d); acc, ps = mm.group(1), mm.group(2)
        ps = [] if ps in ('void', '') else [p.strip() for p in split_args(ps)]
        tps = []
        for p in ps:
            for kw, nm in re.findall(r'\b(class|struct) ([\w:]+)(?![\w:]*<)', p):
                parts = nm.split('::')
                if parts[0] == 'std': continue
                if parts == cq[:len(parts)]: continue
                fd = '%s %s;' % (kw, parts[-1])
                for n in reversed(parts[:-1]): fd = 'namespace %s { %s }' % (n, fd)
                if fd not in fwd: fwd.append(fd)
            if 'CArray<' in p and 'template <class T> class CArray;' not in fwd: fwd.insert(0, 'template <class T> class CArray;')
            if 'std::vector' in p and FWD_STD.strip() not in fwd: fwd.insert(0, FWD_STD.strip())
            tps.append(typ(p).replace('NScript::', '') if scopes and scopes[0] == 'NScript' else typ(p))
        # nested quest classes: the outer script class reference parameter
        cdecl = '%s(%s);' % (cls, ', '.join(tps))
        cacc = acc
        cdef = '%s::%s(%s)%s { }' % (full, cls, ', '.join(tps), (' : %s(%s)' % (base, ', '.join('a%d' % i for i in range(len(tps))))) if base else '')
        if base:
            cdef = '%s::%s(%s) : %s(%s) { }' % (full, cls, ', '.join('%s a%d' % (t, i) for i, t in enumerate(tps)), base, ', '.join('a%d' % i for i in range(len(tps))))
        ca = caddr(ctor)
        emit = ('// TU emitter: the PDB constructor (%s%s) defined with an empty body emits the vtable and with it this\n'
                '//   ??_G (the constructor body itself is its own row; not part of this graded body).\n' % (ctor, ca)) + cdef + '\n'
    elif virt:
        cdecl = '%s() { }' % cls; cacc = 'public'
        emit = ('void* MakeOne_%s()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body\n'
                '{\n    return new %s;\n}\n' % (a, full))
        fwd.append('void* __cdecl operator new(unsigned int size);       // 0x00BFEA4A')
    else:
        cdecl = None
        emit = ('void Kill_%s(%s* object)                     // forces the scalar deleting destructor into the object; not part of the retail body\n'
                '{\n    delete object;\n}\n' % (a, full))
    # class text
    ind = ''
    def cls_block(name, inner, basecl=None):
        return 'class %s%s {\n%s%s};\n' % (name, (' : public ' + basecl) if basecl else '', '' if inner.startswith('private:') else 'public:\n', inner)
    inner = ''
    if cdecl:
        if cacc != 'public': inner += '%s:\n' % cacc
        inner += '    %s%s\n' % (cdecl, ('   // ' + caddr(ctor).strip()) if ctor and caddr(ctor) else ('   // %s' % ctor if ctor else ''))
        if cacc != 'public': inner += 'public:\n'
    if virt:
        if base: inner += ''   # implicit destructor: retail's ~CRenderManager is the inline implicit one (calls ~CRenderManager2D)
        else: inner += '    virtual ~%s();%s// 0x%s (%s)\n' % (cls, ' ' * max(1, 40 - len(cls)), dt, dsym)
    else:
        inner += '    ~%s();%s// 0x%s (%s)\n' % (cls, ' ' * max(1, 48 - len(cls)), dt, dsym)
    if base:
        bctor = CTOR['CRenderManager2D']
        bd = und.get([bctor])[bctor]; bps = re.match(r'.*\((.*)\)$', bd).group(1)
        btps = [typ(p) for p in split_args(bps)]
        binner = '    %s(%s);   // %s\n    virtual ~%s();%s// 0x%s (??1%s@@UAE@XZ)\n' % (base, ', '.join(btps), (caddr(bctor).strip() or bctor), base, ' ' * 26, dt, base)
        text = cls_block(base, binner) + '\n' + cls_block(cls, inner, base)
    else:
        text = cls_block(cls, inner)
    for sc in reversed(scopes):
        if sc == 'NScript' or (scopes.index(sc) == 0 and sc.startswith('N') and not sc.startswith('CQ')):
            text = 'namespace %s {\n%s}\n' % (sc, text)
        else:
            text = 'class %s {\npublic:\n%s};\n' % (sc, '\n'.join('    ' + l if l else l for l in text.rstrip('\n').split('\n')) + '\n')
    retail = 'Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall, `ret 4`' % (a.upper(), CAT[a][2])
    hdr = ["// %s::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set" % full,
           '// Leaf: %s' % gsym,
           '// Retail: %s.' % retail,
           '// Identity (proven, chunk %s): %s.' % (chunk, ev.rstrip('.')),
           '// Callees: %s 0x%s (%s%s), operator delete 0x00BFE9BC.' % (('%s::~%s' % (base, base)) if base else ('%s::~%s' % (full, cls)), dt,
                ('??1%s@@UAE@XZ' % base) if base else dsym, '; the implicit ~CRenderManager inlines to it' if base else ''),
           '// Naming wave 17: landed as the %s' % oldn,
           '//   calling %s; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).' % oldc]
    out = '\n'.join(hdr) + '\n'
    pg = [l for l in old.split('\n')[:3] if l.startswith('#pragma optimize')]
    if pg: out = pg[0] + '   // kept from the landed row: retail built this TU for size (`pop ecx` after operator delete)\n' + out
    out += 'void __cdecl operator delete(void* p);                   // 0x00BFE9BC\n'
    for f in fwd:
        if 'operator new' in f: out += f + '\n'
    fw = [f for f in fwd if 'operator new' not in f]
    if fw: out += '\n' + '\n'.join(fw) + '\n'
    out += '\n' + text + '\n' + emit
    return out
GQ = [q for q in Q if q[2].startswith('??_G') and q[0] in ('c675', 'c676', 'c677', 'c678', 'c681') and '?$' not in q[2]]   # template classes (cg10 005baa41) are not this emitter's shape
only = set(sys.argv[1:])
for q in GQ:
    a = inputs.na(q[1])
    if only and a not in only: continue
    t = build(a, q[2], q[3], q[0])
    p, rel = he.path(a, 'src')
    cur = open(p, encoding='utf-8', newline='').read()
    if cur == t: print('already', a); continue
    open(p, 'w', encoding='utf-8', newline='').write(t); print('ok', a, 'src')
