#!/usr/bin/env python3
"""gen_cloneool.py [addr...|file]: NF425 generator for the CLONE_OOL shape (retail: push esi; push SZ; mov esi,ecx; call operator new
0x00BFEA1A; test; je; push esi; mov ecx,eax; call <C's copy constructor>; pop esi; ret / xor eax,eax; pop esi; ret).
The row becomes the real virtual `CCreatureActionBase* C::Clone() const { return new C(*this); }` (access from the RTTI symbol: U public,
E private, M protected) with C's copy constructor declared out of line.
Callee identity (two sources): the PDB name set(s) spell ??0C@@QAE@ABV0@@Z (V or U key as the PDB has it) and the retail callee body
stores C's own vftable (rtti-vtables.tsv) first.
Form, in order: F1 = the class's own engine header with the prelude of the copy-constructor row (then other landed includers), only
where it compiles and the leaf bytes equal retail (sizeof == SZ); F3 = a local class over the RTTI direct base's header with the
class's own members as one opaque block sized from retail (`unsigned char _body[SZ - sizeof(Base)]`), plus the PerformAction override
both PDB name sets give the class (the base's is pure). Symbol check: DEF == [leaf]; UND == {??2@YAPAXI@Z, ctor}; leaf bytes == retail
(relocation fields masked). Writes work/gco/<a>.cpp, <a>_test.cpp, work/gco.tsv."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import capstone
R = L.R; A = L.A
rows = sys.argv[1:]
if len(rows) == 1 and os.path.isfile(rows[0]): rows = open(rows[0]).read().split()
if not rows: rows = sorted(a for a in L.WL if L.SHAPE[a] == 'CLONE_OOL')
G = A + '/work/gco'; os.makedirs(G, exist_ok=True)
OUT = os.environ.get('GENOUT', A + '/work/gco.tsv')
VT = collections.defaultdict(set)
for r in L.tsv(R + '/rebuild/manifest/rtti-vtables.tsv')[1:]: VT[r[0]].add(int(r[1], 16))
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
def first_vt_store(T):
    c = L.nf.catalog().get(T)
    if not c: return None
    for i in md.disasm(bytes.fromhex(c['bytes']), int(T, 16)):
        m = re.match(r'dword ptr \[(\w+)\], (0x[0-9a-f]{7,8})$', i.op_str)
        if i.mnemonic == 'mov' and m: return int(m.group(2), 16)
    return None
ACC = {'U': ('public', 'UBE'), 'E': ('private', 'EBE'), 'M': ('protected', 'MBE')}
def prelude_of(path, hdr, C):
    t = open(path, errors='replace').read(); m = re.search(r'^\s*#\s*include\s+"%s"' % re.escape(hdr), t, re.M)
    if not m: return None
    lines = []
    for l in t[:m.end()].splitlines():
        s = re.sub(r'\s*//.*$', '', l).strip()
        if not s.startswith('#'): continue
        s = re.sub(r'^#\s*', '#', s)
        if re.match(r'#(define|undef)\s+%s\b' % re.escape(C), s): return None
        if re.match(r'#(pragma|if|endif|else|elif|undef)', s): continue
        if s.startswith('#include') or s.startswith('#define'): lines.append(s)
    return lines
res = []
for a in rows:
    r = L.WL[a]; C = r['klass']; leaf = r['rtti']; calls = r['calls'].split()
    sz = int(r['newsize'], 16); T = calls[1].split(':')[1]
    m = re.match(r'\?Clone@\w+@@([UEM])BEPAV(\w+)@@XZ$', leaf)
    if not m or '::' in C: res.append([a, 'NOSHAPE', '-', '-', T, '-', '-', leaf]); continue
    acc, rett = ACC[m.group(1)][0], m.group(2)
    ctor = None
    for key in ('V', 'U'):
        cand = '??0%s@@QAE@AB%s0@@Z' % (C, key)
        if L.pdb_src(cand): ctor = cand; break
    ckw = 'class' if ctor and ctor[-5] == 'V' else 'struct'
    vt = first_vt_store(T); own_vt = vt in VT.get(C, set())
    osf, opdb, ost = L.owner_self(T)
    ev = ['self %s: RTTI placement + %s' % (leaf, '/'.join(L.pdb_src(leaf)) or 'NO PDB')]
    if not ctor:
        res.append([a, 'NOCTOR', '-', '-', T, '-', osf, ev[0] + '; no ??0%s copy constructor in either PDB name set' % C]); continue
    cev = ['/'.join(L.pdb_src(ctor)) + ' name set']
    if own_vt: cev.append('retail 0x%s stores %s\'s vftable 0x%08X first' % (T.upper(), C, vt))
    if L.OM.get(T, ('',))[0] == ctor: cev.append('order_match ' + L.OM[T][1])
    if osf == ctor: cev.append('the landed row at 0x%s defines it' % T.upper())
    ev.append('callee %s: %s' % (ctor, '; '.join(cev)))
    nsrc = len(L.pdb_src(leaf)) >= 1 and len(cev) >= 2 and own_vt
    resolve = 'owner_defines' if osf == ctor else 'owner_other'
    perf = '?PerformAction@%s@@UAEXXZ' % C
    cands = []
    hdr = L.header_of(C)
    if hdr and not os.environ.get('NOF1'):
        pres = []
        osrc = L.nf.src_of(T)
        if osrc and osrc.exists():
            p = prelude_of(str(osrc), hdr, C)
            if p: pres.append(('copy constructor row 0x%s' % T.upper(), p))
        for why, p in pres: cands.append(('F1', hdr, why, p))
    base = L.direct(C); bh = L.header_of(base)
    if base in ('CCreatureActionBase', 'CCombatActionBase'):
        pre = list(L.PRE_CAB)
        if base == 'CCombatActionBase': pre = list(L.PRE_CCOMBAT)
        cands.append(('F3', 'engine/%s.h' % base, 'RTTI direct base %s' % base, pre))
    elif bh:
        cands.append(('F3', bh, 'RTTI direct base %s' % base, L.PRE_CAB[:4] + ['#include "%s"' % bh]))
    ok = None; tried = []
    for form, h, why, pre in cands:
        if form == 'F1':
            body = '\n'.join(pre) + '\n'
        else:
            pa = ('    virtual void PerformAction();                                   // %s (%s name set%s)\n' % (perf, ' + '.join(L.pdb_src(perf)), 's' if len(L.pdb_src(perf)) == 2 else '; the base\'s is pure and retail instantiates the class')) if L.pdb_src(perf) else ''
            body = ('\n'.join(pre) + '\n\n' + ckw + ' %s : public %s\n{\npublic:\n    %s(const %s& other);%s\n%s%s:\n    virtual %s* Clone() const;\n%s'
                    '    unsigned char _body[0x%X - sizeof(%s)];   // %s\'s own members (not modelled: the copy constructor is out of line); sizeof 0x%X\n};\n'
                    % (C, base, C, C, '   // 0x%s' % T.upper(), pa, acc, rett, 'public:\n' if acc != 'public' else '', sz, base, C, sz))
        defn = '\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
        src = body + defn
        f = G + '/%s_%s.cpp' % (a, form); open(f, 'w').write(src)
        rc, out = L.compile_(f, f[:-4] + '.obj'); open(f[:-4] + '.log', 'w').write(out)
        if rc != 0: tried.append('%s(%s): rc=%d %s' % (form, why, rc, ' '.join(re.findall(r'error C\d+', out)[:3]))); continue
        dfn, und = L.syms(f[:-4] + '.obj'); lc = L.leafcmp(f[:-4] + '.obj', leaf, a)
        if dfn != [leaf] or sorted(und) != sorted(['??2@YAPAXI@Z', ctor]) or lc != 'EQ':
            tried.append('%s(%s): defs=%s und=%s leaf=%s' % (form, why, dfn, und, lc)); continue
        ok = (form, h, why, body); break
    if not ok:
        res.append([a, 'NOFORM', '-', '-', T, ctor, osf, ' | '.join(tried)[:600]]); continue
    form, h, why, body = ok
    hdrnote = ('the class\'s own header %s with the prelude of the %s (it declares the copy constructor and Clone; sizeof == 0x%X)' % (h, why, sz) if form == 'F1' else
               '%s; a local class over its %s %s' % (('engine header %s does not give the class at sizeof 0x%X with Clone declared in the prelude tried (header debt)' % (L.header_of(C), sz)) if L.header_of(C) else 'no engine header defines %s' % C, why, h))
    hd = ['// %s::Clone() const -- `return new %s(*this);` through the out-of-line copy constructor' % (C, C),
          '// Leaf: %s' % leaf,
          '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes: operator new(0x%X) 0x00BFEA1A, null test, then the copy constructor 0x%s on `*this`;' % (a.upper(), r['length'], sz, T.upper()),
          '//   a %s const virtual __thiscall returning %s* (0 when the allocation fails).' % (acc, rett)]
    hd += L.wrap('// Identity (NF425, 2026-11-25):', '; '.join(ev) + '.')
    hd += L.wrap('// Respell (NF425):', 'was the stand-in self %s (non-virtual, non-const); now the real virtual member, so the RTTI-placed name is defined.' % r['self_leaf'])
    hd += L.wrap('// Header:', hdrnote + '.')
    src = '\n'.join(hd) + '\n' + body + '\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
    open(G + '/%s.cpp' % a, 'w').write(src)
    pp = L.PAY[a]['pass_pattern']; failmsg = pp.replace('PASS', 'FAIL') if 'PASS' in pp else 'FAIL_' + a
    key = ctor[-5]   # V / U in ...AB?0@@Z
    tst = ('// Behaviour test for 0x%s %s::Clone (NF425): links the row object; operator new and the copy constructor are defined here and\n'
           '// record their arguments: one allocation of 0x%X bytes, one copy construction into that block from `*this`, the block returned;\n'
           '// and 0 with no construction when the allocation fails.\n#include <stdio.h>\n#include <stddef.h>\n\n'
           'class %s;\n%s %s\n{\n    friend int main(void);\npublic:\n    %s(const %s& other);\n%s:\n    virtual %s* Clone() const;   // defined in the row object\n};\n\n'
           'static unsigned int g_size = 0;\nstatic int g_news = 0, g_ctors = 0, g_fail = 0;\nstatic const void* g_this = 0;\nstatic const void* g_other = 0;\n'
           'static long g_block[0x%X / 4 + 4];\n'
           'void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }\n'
           '%s::%s(const %s& other) { g_this = this; g_other = &other; ++g_ctors; }\n\n'
           'static long g_src[64];\n\nint main(void)\n{\n    const %s* src = (const %s*)g_src;\n'
           '    %s* r = src->%s::Clone();\n'
           '    int ok = r == (%s*)(void*)g_block && g_news == 1 && g_size == 0x%X && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)g_src;\n'
           '    g_fail = 1;\n    r = src->%s::Clone();\n    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;\n'
           '    if (!ok) {\n        printf("%s news=%%d ctors=%%d size=%%u\\n", g_news, g_ctors, g_size);\n        return 1;\n    }\n'
           '    printf("%s\\n");\n    return 0;\n}\n') % (a.upper(), C, sz, rett, 'class' if key == 'V' else 'struct', C, C, C, acc, rett, sz, C, C, C, C, C, rett, C, rett, sz, C, failmsg, pp)
    open(G + '/%s_test.cpp' % a, 'w').write(tst)
    res.append([a, 'OK' if nsrc else 'ONESRC', form, h, T, ctor, osf, resolve, ' ; '.join(ev)])
with open(OUT, 'w') as f:
    for x in res: f.write('\t'.join(x) + '\n')
print(collections.Counter((x[1], x[2], x[7] if len(x) > 8 else '') for x in res))
for x in res:
    if x[1] != 'OK': print(x[:7], x[-1][:400])
