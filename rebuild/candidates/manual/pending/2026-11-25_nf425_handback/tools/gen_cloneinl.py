#!/usr/bin/env python3
"""gen_cloneinl.py [addr...|file]: NF425 generator for the CLONE_INLINE shape (retail: operator new(SZ), null test, the base class's
out-of-line copy constructor, C's vftable store, then C's own members copied by plain moves): `new C(*this)` with C's implicit copy
constructor inlined. The row becomes the real virtual `CCreatureActionBase* C::Clone() const` on a local class derived directly from the
RTTI direct base's shared header (CCombatActionBase: the landed 0x008CAA70 MELEE prelude, copy constructor 0x0062F3B0 by
MELEE_COPYCTOR; CCreatureActionBase: COPYCTOR 0x0062EBC0). C's own members are the Ego_r.pdb members (struct_layouts_egor.tsv) at
their retail offsets (PDB offset - (PDB sizeof base - retail sizeof base)), each one checked against retail's copy (offset and width).
PerformAction is declared when a PDB name set gives C's override. Never a layout-rename (the inlined constructor emits C's vftable).
Symbol check: the leaf bytes == retail (relocations masked); no ??_E / ??_G / RTTI definitions; UND has operator new + the base copy ctor.
Writes work/gci/<a>.cpp, <a>_test.cpp, work/gci.tsv."""
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import capstone
R = L.R; A = L.A
rows = sys.argv[1:]
if len(rows) == 1 and os.path.isfile(rows[0]): rows = open(rows[0]).read().split()
if not rows: rows = sorted(a for a in L.WL if L.SHAPE[a] == 'CLONE_INLINE')
G = A + '/work/gci'; os.makedirs(G, exist_ok=True)
OUT = os.environ.get('GENOUT', A + '/work/gci.tsv')
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
LAY = collections.defaultdict(list); LSZ = {}
for l in open(R + '/ghidra_out/struct_layouts_egor.tsv', errors='replace'):
    f = l.rstrip('\n').split('\t')
    if f[0] == '@STRUCT': LSZ[f[1]] = int(f[2]); continue
    if len(f) >= 5 and f[4] != '_padding_': LAY[f[0]].append((int(f[2]), f[3], f[4]))
BASEINFO = {'CCombatActionBase': (0x114, '??0CCombatActionBase@@QAE@ABV0@@Z', 0x0062F3B0, L.PRE_CCOMBAT),
            'CCreatureActionBase': (0xA8, '??0CCreatureActionBase@@QAE@ABV0@@Z', 0x0062EBC0,
                                    ['#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR'] + L.PRE_CAB)}
BUILTIN = {'long': 4, 'int': 4, 'unsigned long': 4, 'unsigned int': 4, 'float': 4, 'bool': 1, 'char': 1, 'unsigned char': 1,
           'short': 2, 'unsigned short': 2, 'signed char': 1}
ACC = {'U': 'public', 'E': 'private', 'M': 'protected'}
def copies(a):
    c = L.nf.catalog()[a]; ins = list(md.disasm(bytes.fromhex(c['bytes']), int(a, 16)))
    out = []; seen_vt = False; pend = {}
    for i in ins:
        if i.mnemonic == 'mov' and re.match(r'dword ptr \[esi\], 0x', i.op_str): seen_vt = True; continue
        if not seen_vt: continue
        m = re.match(r'(\w+), (byte|word|dword) ptr \[edi \+ (0x[0-9a-f]+)\]$', i.op_str)
        if i.mnemonic == 'mov' and m: pend[m.group(1)] = (int(m.group(3), 16), {'byte': 1, 'word': 2, 'dword': 4}[m.group(2)]); continue
        m = re.match(r'(byte|word|dword) ptr \[esi \+ (0x[0-9a-f]+)\], (\w+)$', i.op_str)
        if i.mnemonic == 'mov' and m:
            o, w = pend.get(m.group(3), (None, None))
            if o != int(m.group(2), 16): return None
            out.append((o, w)); continue
        if i.mnemonic == 'pop' and i.op_str == 'edi': continue
        if (i.mnemonic == 'mov' and i.op_str == 'eax, esi') or i.mnemonic in ('ret', 'xor'): break
        return None
    return out if seen_vt else None
res = []
for a in rows:
    r = L.WL[a]; C = r['klass']; leaf = r['rtti']; base = L.direct(C)
    m = re.match(r'\?Clone@\w+@@([UEM])BEPAV(\w+)@@XZ$', leaf)
    if not m or '::' in C or base not in BASEINFO: res.append([a, 'NOSHAPE', base, leaf]); continue
    acc, rett = ACC[m.group(1)], m.group(2)
    rbase, bctor, bctora, pre = BASEINFO[base]
    sz = int(r['newsize'], 16)
    calls = r['calls'].split()
    if len(calls) != 2 or calls[1].split(':')[1] != '%08x' % bctora:
        res.append([a, 'NOSHAPE', base, 'second call %s is not the base copy constructor 0x%08X' % (calls[1:], bctora)]); continue
    cp = copies(a)
    if cp is None: res.append([a, 'NOSHAPE', base, 'member copies not plain moves']); continue
    delta = LSZ.get(base, 0) - rbase
    pm = sorted(x for x in LAY.get(C, []) if x[0] >= LSZ.get(base, 0))
    mem = []; why = ''
    if len(pm) != len(cp): why = 'PDB members %s v retail copies %s' % ([(hex(o - delta), t, n) for o, t, n in pm], [(hex(o), w) for o, w in cp])
    else:
        for (po, pt, pn), (ro, rw) in zip(pm, sorted(cp)):
            t = pt.strip(); w = BUILTIN.get(t)
            decl = None
            if w is None and t.endswith('*'): w = 4; decl = (t, 'class %s;' % t.rstrip('*').strip().replace('const ', '').split('::')[-1])
            if w is None and re.match(r'^E\w+$', t): w = 4; decl = ('long', None); t = 'long /* %s */' % pt
            if po - delta != ro or w != rw: why = 'member %s %s PDB +0x%X (retail +0x%X) v retail copy +0x%X width %d' % (pt, pn, po, po - delta, ro, rw); break
            mem.append((ro, decl[0] if decl and decl[0] != 'long' else t, pn, pt, decl[1] if decl else None))
    if why: res.append([a, 'NOMEMBERS', base, why]); continue
    perf = '?PerformAction@%s@@UAEXXZ' % C
    fwd = sorted({x[4] for x in mem if x[4]})
    pa = ('    virtual void PerformAction();                                   // %s (%s name set%s)\n' % (perf, ' + '.join(L.pdb_src(perf)), 's' if len(L.pdb_src(perf)) == 2 else '')) if L.pdb_src(perf) else ''
    ml = ''.join('    %s %s;%s// +0x%X (Ego_r.pdb %s %s at +0x%X)\n' % (t, n, ' ' * max(1, 40 - len(t) - len(n)), o, pt, n, o + delta) for o, t, n, pt, _ in mem)
    body = ('\n'.join(pre) + '\n' + ''.join(x + '\n' for x in fwd) + '\nclass %s : public %s\n{\npublic:\n%s%s:\n    virtual %s* Clone() const;\n%s%s};\n'
            % (C, base, pa, acc, rett, 'public:\n' if acc != 'public' and ml else '', ml))
    defn = '\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
    f = G + '/%s_F3.cpp' % a; open(f, 'w').write(body + defn)
    rc, out = L.compile_(f, f[:-4] + '.obj'); open(f[:-4] + '.log', 'w').write(out)
    if rc != 0: res.append([a, 'NOFORM', base, 'rc=%d %s' % (rc, ' '.join(re.findall(r'error C\d+', out)[:3]))]); continue
    dfn, und = L.syms(f[:-4] + '.obj'); lc = L.leafcmp(f[:-4] + '.obj', leaf, a)
    extra = [s for s in dfn if s not in (leaf, '??_7%s@@6B@' % C, '??0%s@@QAE@ABV0@@Z' % C)]
    if lc != 'EQ' or extra or bctor not in und or '??2@YAPAXI@Z' not in und:
        res.append([a, 'NOFORM', base, 'leaf=%s extra=%s und_base=%s' % (lc, extra, bctor in und)]); continue
    ev = ['self %s: RTTI placement + %s' % (leaf, '/'.join(L.pdb_src(leaf)) or 'NO PDB'),
          'base copy constructor %s 0x%08X (%s; MELEE_COPYCTOR / COPYCTOR opt-in)' % (bctor, bctora, '/'.join(L.pdb_src(bctor)) or 'NO PDB')]
    hdrc = L.header_of(C)
    hdrnote = ('%s; a local class over its RTTI direct base %s (engine/%s.h)' % (('engine header %s is not used: the inlined copy constructor emits %s\'s vftable, so a layout-rename would put a made-up class into it, and its own branch is not the retail layout (header debt)' % (hdrc, C)) if hdrc else 'no engine header defines %s' % C, base, base))
    hd = ['// %s::Clone() const -- `return new %s(*this);` with the implicit copy constructor inlined (%s copy constructor, vftable store%s)' % (C, C, base, ', own members' if mem else ''),
          '// Leaf: %s' % leaf,
          '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes: operator new(0x%X) 0x00BFEA1A, null test, %s(const %s&) 0x%08X, C\'s vftable store%s;' % (a.upper(), r['length'], sz, base, base, bctora, (', then %d member cop%s' % (len(mem), 'y' if len(mem) == 1 else 'ies')) if mem else ''),
          '//   a %s const virtual __thiscall returning %s* (0 when the allocation fails).' % (acc, rett)]
    hd += L.wrap('// Identity (NF425, 2026-11-25):', '; '.join(ev) + '.')
    hd += L.wrap('// Respell (NF425):', 'was the stand-in self %s (non-virtual, non-const%s); now the real virtual member, so the RTTI-placed name is defined.' % (r['self_leaf'], ', over a made-up base model' if 'CBase_' in L.PAY[a]['source_cpp'] else ''))
    hd += L.wrap('// Header:', hdrnote + '.')
    if mem: hd += L.wrap('// Members:', 'Ego_r.pdb names at retail offsets (PDB offset - 0x%X: the base shrinks from 0x%X to 0x%X with the retail STLport containers); each width matches retail\'s copy.' % (delta, LSZ.get(base, 0), rbase))
    src = '\n'.join(hd) + '\n' + body + defn
    open(G + '/%s.cpp' % a, 'w').write(src)
    pp = L.PAY[a]['pass_pattern']; failmsg = pp.replace('PASS', 'FAIL') if 'PASS' in pp else 'FAIL_' + a
    # test: links the row object and the vftable it emits; defines operator new, the base copy constructor (copies the 0x%X base bytes)
    # and every vftable slot the object leaves unresolved is not needed (the test never calls through the table).
    tst = ('// Behaviour test for 0x%s %s::Clone (NF425): links the row object; operator new and the base copy constructor are defined here.\n'
           '// Checks: one allocation of 0x%X bytes; the base copy constructor runs once into the block from `*this`; the block gets the class\'s\n'
           '// vftable pointer (the source\'s, as both are %s objects); the class\'s own members are copied; 0 with no construction when new fails.\n'
           '#include <stdio.h>\n#include <string.h>\n#include <stddef.h>\n\n' % (a.upper(), C, sz, C))
    tst += 'static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;\n'
    tst += ''.join(x + '\n' for x in fwd)
    tst += 'class CCreatureActionBase;\nclass %s\n{\npublic:\n    %s(const %s& other);\n    unsigned char _base[0x%X];\n};\n' % (base, base, base, rbase)
    tst += '%s::%s(const %s& other) { g_this = this; g_other = &other; ++g_ctors; memcpy(_base + 4, other._base + 4, 0x%X - 4); }\n' % (base, base, base, rbase)
    tst += 'class %s : public %s\n{\n    friend int main(void);\npublic:\n%s:\n    virtual %s* Clone() const;   // defined in the row object\n%s};\n' % (C, base, acc, rett, ''.join('    %s %s;\n' % (t, n) for o, t, n, pt, _ in mem) and ('public:\n' + ''.join('    %s %s;\n' % (t, n) for o, t, n, pt, _ in mem)))
    tst = tst.replace('    unsigned char _base[0x%X];' % rbase, '    void* vptr_;\n    unsigned char _base[0x%X - 4];' % rbase).replace('memcpy(_base + 4, other._base + 4, 0x%X - 4)' % rbase, 'memcpy(_base, other._base, 0x%X - 4)' % rbase)
    stubs = [s for s in und if s not in (bctor, '??2@YAPAXI@Z')]
    tst += ('// The row object emits the class\'s vftable (its inlined copy constructor stores it); the test never calls through it, so\n'
            '// every slot target the object leaves unresolved is mapped onto one unused stub for the link.\n'
            'extern "C" void nf425_unused_slot(void) { }\n' + ''.join('#pragma comment(linker, "/alternatename:%s=_nf425_unused_slot")\n' % s for s in stubs))
    tst += ('static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;\nstatic long g_block[0x%X / 4 + 4];\n'
            'void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }\n\n'
            'int main(void)\n{\n    static long src_store[0x%X / 4 + 4];\n    unsigned char* s = (unsigned char*)src_store;\n'
            '    for (int i = 4; i < 0x%X; ++i) s[i] = (unsigned char)(i * 7 + 3);\n'
            '    const %s* src = (const %s*)(void*)src_store;\n    %s* r = src->%s::Clone();\n'
            '    unsigned char* d = (unsigned char*)g_block;\n'
            '    int ok = r == (%s*)(void*)g_block && g_news == 1 && g_size == 0x%X && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)src_store\n'
            '        && memcmp(d + 4, s + 4, 0x%X - 4) == 0 && *(void**)d != 0;\n'
            '    g_fail = 1;\n    r = src->%s::Clone();\n    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;\n'
            '    if (!ok) {\n        printf("%s news=%%d ctors=%%d size=%%u\\n", g_news, g_ctors, g_size);\n        return 1;\n    }\n'
            '    printf("%s\\n");\n    return 0;\n}\n') % (sz, sz, sz, C, C, rett, C, rett, sz, sz, C, failmsg, pp)
    open(G + '/%s_test.cpp' % a, 'w').write(tst)
    res.append([a, 'OK', base, ' ; '.join(ev), str(len(mem))])
with open(OUT, 'w') as f:
    for x in res: f.write('\t'.join(x) + '\n')
print(collections.Counter((x[1], x[2]) for x in res))
for x in res:
    if x[1] != 'OK': print(x[0], x[1], x[2], x[3][:300])
