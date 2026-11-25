#!/usr/bin/env python3
"""gen_copyjmp.py [addr...|file]: NF425 generator for the COPY_JMP shape (retail: one 5-byte `jmp rel32` = a thiscall tail call of an
out-of-line operator=). The row becomes the real virtual `void C::Copy(const CDefClassBase* other)` with the def-family idiom
`*this = *(const C*)other;` (the 100 landed real Copy rows, e.g. 0x0046F4EB CSkyDef), so VC7.1 tail-calls the declared operator=.
Target identity (per row, never by +5 alone): OWN = the jmp target carries C's own ??4 (table / owner Leaf / tracker PDB spelling), or
PLUS5 = the target is C's next body and the owner is an address-named stand-in: C's ??4 from both PDB name sets + this caller binding;
ANC:<B> = the target carries ancestor B's ??4 (C's operator= stays implicit).
Form, in order: F1 = the class's own engine header with a landed prelude (the owner row's, then other landed includers'), only where the
header already declares Copy and operator=; F3 = a local class over the RTTI direct base's header (else the nearest ancestor with one).
Symbol check of the compiled row: DEF == [leaf]; UND contains the target name; no vftable / RTTI / deleting-destructor / ??4C definition.
Writes work/gcj/<a>.cpp, <a>_test.cpp and work/gcj.tsv (address verdict form header kind target target_name owner_self resolve evidence)."""
import sys, os, re, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
R = L.R; A = L.A
rows = sys.argv[1:]
if len(rows) == 1 and os.path.isfile(rows[0]): rows = open(rows[0]).read().split()
if not rows: rows = sorted(a for a in L.WL if L.SHAPE[a] == 'COPY_JMP')
G = A + '/work/gcj'; os.makedirs(G, exist_ok=True)
OUT = os.environ.get('GENOUT', A + '/work/gcj.tsv')
SRCS = None
def includers(hdr):
    global SRCS
    if SRCS is None: SRCS = sorted(glob.glob(R + '/rebuild/src/compiled/**/*.cpp', recursive=True))
    pat = re.compile(r'^\s*#\s*include\s+"%s"' % re.escape(hdr), re.M); out = []
    for p in SRCS:
        t = open(p, errors='replace').read()
        if pat.search(t): out.append(p)
    return out
def prelude_of(path, hdr, C):
    t = open(path, errors='replace').read(); m = re.search(r'^\s*#\s*include\s+"%s"' % re.escape(hdr), t, re.M)
    if not m: return None
    lines = []
    for l in t[:m.end()].splitlines():
        s = re.sub(r'\s*//.*$', '', l).strip()
        if not s.startswith('#'): continue
        s = re.sub(r'^#\s*', '#', s)
        if re.match(r'#(define|undef)\s+%s\b' % re.escape(C), s): return None          # a layout rename: not the class itself
        if re.match(r'#(pragma|if|endif|else|elif|undef)', s): continue
        if s.startswith('#include') or s.startswith('#define'): lines.append(s)
    return lines
res = []
for a in rows:
    r = L.WL[a]; C = r['klass']; leaf = r['rtti']; T = r['calls'].split(':')[1]
    head = L.PAY[a]['source_cpp']; pr, fl = L.head_pragmas(head)
    own = '??4%s@@QAEAAV0@ABV0@@Z' % C
    names = set(L.TAB.get(T, set())); osf, opdb, ost = L.owner_self(T)
    if osf: names.add(osf)
    if opdb: names.add(opdb)
    om = L.OM.get(T, ('', ''))
    kind = ''; tname = ''; anc = ''
    if own in names: kind, tname = 'OWN', own
    else:
        for x in L.ancestors(C):
            if '??4%s@@QAEAAV0@ABV0@@Z' % x in names: kind, tname, anc = 'ANC', '??4%s@@QAEAAV0@ABV0@@Z' % x, x; break
    if not kind and int(T, 16) == int(a, 16) + 5 and re.match(r'\?\?4CBase_[0-9a-f]{8}_Derived_%s@@' % T, osf or ''):
        kind, tname = 'PLUS5', own
    if not kind:
        res.append([a, 'NOIDENT', '-', '-', '-', T, '-', osf, '-', 'target names %s' % sorted(names)]); continue
    selfsrc = L.pdb_src(leaf); tsrc = L.pdb_src(tname)
    ev = ['self %s: RTTI placement + %s' % (leaf, '/'.join(selfsrc) or 'NO PDB')]
    tev = []
    if kind == 'OWN': tev.append('bound at 0x%s (%s)' % (T.upper(), 'owner Leaf' if osf == tname else ('tracker PDB spelling' if opdb == tname else 'table')))
    if kind == 'ANC': tev.append('ancestor %s ??4 bound at 0x%s' % (anc, T.upper()))
    if kind == 'PLUS5': tev.append('the RTTI-proven Copy of %s tail-jumps to 0x%s (its next body; owner is the address-named stand-in %s)' % (C, T.upper(), osf))
    if tsrc: tev.append('/'.join(tsrc) + ' name set')
    if om[0] == tname: tev.append('order_match %s' % om[1])
    ev.append('target %s: %s' % (tname, '; '.join(tev)))
    body_ok = ''
    tc = L.nf.catalog().get(T)
    if tc and kind != 'ANC':
        import capstone
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        fc = [int(i.op_str, 16) for i in md.disasm(bytes.fromhex(tc['bytes']), int(T, 16)) if i.mnemonic == 'call' and i.op_str.startswith('0x')][:1]
        if fc:
            fcn = L.TAB.get('%08x' % fc[0], set()) | {L.owner_self('%08x' % fc[0])[0]}
            hit = [x for x in L.ancestors(C) if '??4%s@@QAEAAV0@ABV0@@Z' % x in fcn]
            if hit: body_ok = hit[0]
    if body_ok: tev.append('its body opens with a call of the ancestor operator= ??4%s (0x%08X): a derived class\'s assignment' % (body_ok, fc[0]))
    ev[-1] = 'target %s: %s' % (tname, '; '.join(tev))
    nsrc = len(selfsrc) + 1 >= 2 and (len(tsrc) + (kind == 'OWN') + (om[0] == tname) + bool(body_ok) + 1) >= 2
    # resolution of the new callee name in a link: owner row defines it / HEAD unresolved too / HEAD resolved through a stand-in
    resolve = 'owner_defines' if osf == tname else 'owner_standin'
    # ---- forms
    cands = []
    hdr = L.header_of(C)
    if hdr:
        pres = []
        osrc = L.nf.src_of(T)
        if osrc and osrc.exists():
            p = prelude_of(str(osrc), hdr, C)
            if p: pres.append(('owner 0x%s' % T.upper(), p))
        if not os.environ.get('NOF1'):
            for p_ in includers(hdr)[:40]:
                p = prelude_of(p_, hdr, C)
                if p and all(p != q for _, q in pres): pres.append((os.path.relpath(p_, R + '/rebuild/src/compiled'), p))
                if len(pres) >= int(os.environ.get('MAXPRE', '4')): break
        for why, p in pres: cands.append(('F1', hdr, why, p))
    base = L.direct(C); bh = L.header_of(base); bwhy = 'RTTI direct base %s' % base
    if not bh:
        for x in L.ancestors(C)[1:]:
            if L.header_of(x): base, bh, bwhy = x, L.header_of(x), 'nearest RTTI ancestor with a header %s (direct base %s has none)' % (x, L.direct(C)); break
    if bh: cands.append(('F3', bh, bwhy, ['#define FABLE_RETAIL_DEF_CLASS_BASE', '#include "rebuild_abi.h"', '#include "%s"' % bh]))
    ok = None; tried = []
    for form, h, why, pre in cands:
        if form == 'F1':
            body = '\n'.join(pre) + '\n'
        else:
            decl = ('    %s& operator=(const %s& other);' % (C, C)) if kind != 'ANC' else ''
            body = '\n'.join(pre) + '\n\nclass %s : public %s\n{\npublic:\n    virtual void Copy(const CDefClassBase* other);\n%s};\n' % (C, base, (decl + '\n') if decl else '')
        defn = '\nvoid %s::Copy(const CDefClassBase* other)\n{\n    *this = *(const %s*)other;\n}\n' % (C, C)
        src = '\n'.join(pr) + ('\n' if pr else '') + body + defn
        f = G + '/%s_%s.cpp' % (a, form); open(f, 'w').write(src)
        rc, out = L.compile_(f, f[:-4] + '.obj')
        open(f[:-4] + '.log', 'w').write(out)
        if rc != 0: tried.append('%s(%s): rc=%d' % (form, why, rc)); continue
        dfn, und = L.syms(f[:-4] + '.obj')
        badd = [s for s in dfn if s != leaf]
        if badd or tname not in und:
            tried.append('%s(%s): defs=%s und_has_target=%s' % (form, why, dfn, tname in und)); continue
        ok = (form, h, why, pre, body, und); break
    if not ok:
        res.append([a, 'NOFORM', '-', '-', kind, T, tname, osf, resolve, ' | '.join(tried)[:600]]); continue
    form, h, why, pre, body, und = ok
    hdrnote = ('the class\'s own header %s with the prelude of %s (the header declares Copy and operator=)' % (h, why) if form == 'F1' else
               ('%s; a local class over its %s %s' % (('engine header %s does not declare Copy / operator= in any landed prelude tried (header debt)' % L.header_of(C)) if L.header_of(C) else 'no engine header defines %s' % C, why, h)))
    hd = ['// %s::Copy(const CDefClassBase* other) -- copy every member of another %s: `*this = *(const %s*)other;`' % (C, C, C),
          '// Leaf: %s' % leaf,
          '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 5 bytes: a thiscall tail call, `jmp` to %s 0x%s.' % (a.upper(), ('%s::operator=' % C) if kind != 'ANC' else ('%s::operator= (C adds no members)' % anc), T.upper())]
    hd += L.wrap('// Identity (NF425, 2026-11-25):', '; '.join(ev) + '.')
    hd += L.wrap('// Respell (NF425):', 'was the stand-in self %s; now the real virtual member, so the RTTI-placed name is defined.' % r['self_leaf'])
    hd += L.wrap('// Header:', hdrnote + '.')
    src = '\n'.join(hd) + '\n' + '\n'.join(pr) + ('\n' if pr else '') + body + '\nvoid %s::Copy(const CDefClassBase* other)\n{\n    *this = *(const %s*)other;\n}\n' % (C, C)
    open(G + '/%s.cpp' % a, 'w').write(src)
    # ---- test: links the row object; records the operator= call (this, &other); qualified call, no vftable needed
    pp = L.PAY[a]['pass_pattern']; failmsg = pp.replace('PASS', 'FAIL') if 'PASS' in pp else 'FAIL_' + a
    tgtc = C if kind != 'ANC' else anc
    tst = ('// Behaviour test for 0x%s %s::Copy (NF425): links the row object; %s::operator= is defined here and records its `this` and\n'
           '// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).\n'
           '#include <stdio.h>\n\nclass CDefClassBase;\n') % (a.upper(), C, tgtc)
    if kind == 'ANC':
        tst += 'class %s\n{\npublic:\n    %s& operator=(const %s& other);\n    char pad[64];\n};\n' % (anc, anc, anc)
        tst += 'class %s : public %s\n{\npublic:\n    virtual void Copy(const CDefClassBase* other);   // defined in the row object\n};\n' % (C, anc)
    else:
        tst += 'class %s\n{\npublic:\n    virtual void Copy(const CDefClassBase* other);   // defined in the row object\n    %s& operator=(const %s& other);\n    char pad[64];\n};\n' % (C, C, C)
    tst += ('\nstatic const void* g_this = 0;\nstatic const void* g_other = 0;\nstatic int g_calls = 0;\n'
            '%s& %s::operator=(const %s& other) { g_this = this; g_other = &other; ++g_calls; return *this; }\n\n'
            'static long g_dst[32];\nstatic long g_src[32];\n\n'
            'int main(void)\n{\n    %s* dst = (%s*)g_dst;\n    const CDefClassBase* src = (const CDefClassBase*)g_src;\n'
            '    dst->%s::Copy(src);\n'
            '    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {\n'
            '        printf("%s calls=%%d\\n", g_calls);\n        return 1;\n    }\n    printf("%s\\n");\n    return 0;\n}\n') % (tgtc, tgtc, tgtc, C, C, C, failmsg, pp)
    open(G + '/%s_test.cpp' % a, 'w').write(tst)
    res.append([a, 'OK' if nsrc else 'ONESRC', form, h, kind, T, tname, osf, resolve, ' ; '.join(ev)])
with open(OUT, 'w') as f:
    for x in res: f.write('\t'.join(x) + '\n')
import collections
print(collections.Counter((x[1], x[2], x[4], x[8]) for x in res))
for x in res:
    if x[1] not in ('OK',): print(x[:9], x[9][:300])
